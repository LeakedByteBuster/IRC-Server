#include "Server.hpp"
#include <cstring>
#include "registrationCommands.hpp"

//  holds all the channels created in the server : map<channel name, channel class>
std::map<std::string, Channel>  Server::ChannelsInServer; // all channels

/* -------------------------------------------------------------------------- */
/*                            Server constructors                             */
/* -------------------------------------------------------------------------- */

//   Creates a TCP, IPv4, Passive socket
Server::Server(std::string portNum, std::string password) : password(password), listenPort(htons(atoi(portNum.data())))
{
    // For setsockopt(), the parameter should be non-zero to enable a boolean option
    int optVal = 1;

    struct addrinfo hints, *res, *res0;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE | AI_CANONNAME;
    hints.ai_socktype = SOCK_STREAM;
    int error = 0;

    if ((error = getaddrinfo(NULL, portNum.data(), (const struct addrinfo *)&hints, &res0)) != 0)
    {
        throw std::runtime_error("Error getaddrinfo() : " + static_cast<std::string>(gai_strerror(error)));
        std::cerr.flush();
    }
    int check = 1;
    for (res = res0; res != NULL; res = res->ai_next)
    {
        //  creates server socket and store fd
        int sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sfd == -1)
        {
            std::cerr.flush();
            continue;
        }
        // set socket option. SO_REUSEADDR : enables local address reuse
        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)) == -1)
        {
            close(sfd);
            continue;
        }

        // bind the listenning socket to a specified port and address
        if (bind(sfd, res->ai_addr, res->ai_addrlen) == -1)
        {
            close(sfd);
            continue;
        }
        listenFd = sfd; //  listenFd variable is used to close fd in destructor
        //  Makes all I/O non blocking
        if (fcntl(listenFd, F_SETFL, O_NONBLOCK) == -1)
        {
            throw std::runtime_error("Error fcntl() : " +
                                     static_cast<std::string>(strerror(errno)));
        }

        // Marks the socket as a passive socket
        if (listen(listenFd, BACKLOG) == -1)
        {
            throw std::runtime_error("Error listen() : " +
                                     static_cast<std::string>(strerror(errno)));
        }

        #if defined(LOG)
            std::cout << geTime() << std::endl;
            std::cout << "Canonical name : " 
                << res->ai_canonname << std::endl;
            serverWelcomeMessage( *( (sockaddr_in *)res->ai_addr ), sfd );
        #endif // LOG

        check = 0;
        break;
    }

    freeaddrinfo(res0);
    if (check)
        throw std::runtime_error("Error getaddrinfo()");
    
    serverCreationDate = geTime();
    Message::setErrorsDatabase();
}

Server::Server(Server &rhs) : password(rhs.password), listenPort(rhs.listenPort),
                              listenFd(rhs.listenFd)
{
    *this = rhs;
}

Server &Server::operator=(Server &rhs)
{
    if (this == &rhs)
        return (*this);
    this->listenFd = rhs.listenFd;
    return (*this);
}

Server::~Server()
{
    //  Close server fd
    if (close(listenFd) == -1)
    {
        std::cerr << "Error close() : " << strerror(errno) << std::endl;
    }
    //  Close all clients fd
    std::map<int, Client>::iterator it = clients.begin();
    for (; it != clients.end(); it++)
    {
        if (close(it->second.fd) == -1)
            std::cerr << "Error close() : " << strerror(errno) << std::endl;
    }
}

/* -------------------------------------------------------------------------- */
/*                             Setters & Getters                              */
/* -------------------------------------------------------------------------- */

const unsigned short &Server::getListenPort() const
{
    return (this->listenPort);
}

const int &Server::getListenFd() const
{
    return (this->listenFd);
}

const std::string &Server::getPassword() const
{
    return (this->password);
}

/* -------------------------------------------------------------------------- */
/*                               Server Methods                               */
/* -------------------------------------------------------------------------- */

//  Adds the socket fd to the vector<struct pollfd>
void Server::addNewPollfd(int fd, std::vector<struct pollfd> &fds, nfds_t &nfds)
{
    struct pollfd newSock;

    memset(&newSock, 0, sizeof(newSock));
    newSock.fd = fd;
    newSock.events = POLLIN;
    fds.push_back(newSock);
    nfds++;
}

int Server::isPollReady(std::vector<struct pollfd> &fds, nfds_t &nfds)
{
    int ret = 0;
    if ((ret = poll(fds.data(), nfds, POLL_TIMEOUT)) == -1) {
        throw std::runtime_error("Error poll() : "
            + static_cast<std::string>(strerror(errno)));
    }
    return (ret);
}

//  returns 0 if connection is done successfully, otherwise 0 is returned
bool Server::addNewClient(std::vector<struct pollfd> &fds, nfds_t *nfds, int &fdsLeft)
{
    Client clt(0);
    int clientFd = 0;
    socklen_t sockLen = 0; // used for accept()

    //  Accepting incoming connection
    memset(&clt.hints, 0, sizeof(clt.hints));
    sockLen = sizeof(clt.hints);
    if ((clientFd = accept(listenFd, (sockaddr *)&clt.hints, &sockLen)) == -1)
    {
        fdsLeft--;
        std::cerr << "Error accept() : " << strerror(errno) << std::endl;
        return (EXIT_FAILURE);
    }
    //  Setting the client fd to NON_BLOCKING mode
    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "Error fcntl() : undefined error" << std::endl;
        fdsLeft--;
        return (EXIT_FAILURE);
    }
    //  Add client fd to the poll of file descriptors
    Server::addNewPollfd(clientFd, fds, *nfds);

    #if defined(LOG)
        printNewClientInfoOnServerSide(clt.hints);
        clientWelcomeMessage(clientFd);
    #endif // LOG

    //  Push the new client to map<> in the server
    Client  toPush(clientFd);
    toPush = clt;
    clients.insert(std::pair<int, Client>(clientFd, toPush));
    // Decrement number of fds returned by poll()
    fdsLeft--;

    return (EXIT_SUCCESS);
}

// used for capturing all 3 lines or more at once (for registration)
std::map<int, std::vector<std::string> > gbuff;
void Server::userRegistration(int fd, std::vector<std::string> string)
{
    std::pair<std::map<int, std::vector<std::string> >::iterator, bool> it;
    it = gbuff.insert(std::pair<int, std::vector<std::string> >(fd, string));

    if ((it.second == false) || (string.size() >= 3)) {
        for (size_t i = 0; i < string.size(); i++)
        {
            if (!string[i].empty())
                gbuff[fd].push_back(string[i]);
        }
        if (gbuff[fd].size() >= 3) {
            try  {
                parseRegistrationCommands(clients, gbuff[fd], clients[fd], password);
                clients[fd].isRegistred = 1;
                // client sent all the information correctly, sending welcome message
                Server::postRegistration(clients[fd]);
                gbuff.erase(fd);

            } catch (std::exception &e) {
                clients[fd].nickname.clear();
                clients[fd].realname.clear();
                gbuff.erase(fd);
            }
        }
    }
}

//  Accepts incoming connections
void            Server::handleIncomingConnections()
{
    std::map<int, std::string>  map; // used as a buff when "\n" is not found
    std::vector<struct pollfd>  pollFds; // holds all connection accepted
    std::string                 buff; // for recv()
    nfds_t                      nfds = 0; // size of pollFds vector
    int                         fdsLeft = 0;

    buff.resize(BYTES_TO_READ);
    //  add the server socket fd to the pollfd vector
    Server::addNewPollfd(listenFd, pollFds, nfds);
    while (1) {
        if ((fdsLeft = isPollReady(pollFds, nfds))) {
            //  Checks if there is a new connection to accept
            if (isNewConnection(pollFds[0], listenFd)) {
                addNewClient(pollFds, &nfds, fdsLeft);
            }
            for (size_t i = 1; (i < nfds) && (fdsLeft > 0); i++) {
                const int id = pollFds[i].fd; // id of client in map
                if (isReadable(pollFds[i])) {
                    char    ptr[BYTES_TO_READ];
                    if (readIncomingMsg(ptr, id)) {
                            memset((void *)buff.data(), 0, sizeof(buff));
                            buff = ptr;
                            std::pair<std::string, bool>    str = parseInput(buff, map, pollFds, i);
                            if (str.second == 1) { // '\n' is in the message
                                std::cout.flush();
                                std::vector<std::string> commandStrings = splitByLines(str.first);
                                if (commandStrings.size() > 0) {
                                    if (clients[id].isRegistred == 0) {
                                        userRegistration(id, commandStrings);
                                        fdsLeft--;
                                        continue;
                                    }
                                    for (size_t n = 0; n < commandStrings.size(); n++) {
                                        std::cout << "in --> " << commandStrings[n] << std::endl;
                                        std::vector<std::string> commands = splitBySpace(commandStrings[n]);
                                        execute_commmand(clients, commands, pollFds[i].fd);
                                    }
                                }
                            }
                    }
                    fdsLeft--;

                } else if (isError(pollFds[i].revents, id, listenFd)) {
                    deleteClient(map, pollFds, clients, gbuff, nfds, i, fdsLeft);
                }
            }
        }
    }
}

void    Server::sendMsg(const Client &target, std::string msg)
{
    std::cout << "rpl --> " << msg << std::endl;
    if (msg.size() > 0) {
        char    buff[BYTES_TO_READ];
        ssize_t bytes;

        msg.append("\r\n");
        std::strcpy(buff, msg.data());
        if ((bytes = send(target.fd, (const void *)buff, msg.size(), 0)) == -1) {
            std::cerr << "Error sendMsg(): " << strerror(errno) << std::endl;
        } else if (static_cast<size_t>(bytes) != msg.size()) {
            std::cerr << "Warning sendMsg: data loss : input = " << msg.size() 
                << " sent = " << bytes << std::endl;
        }
    } else {
        std::cerr << "Error sendMsg() : empty message" << std::endl;
    }
}

void    Server::sendMsg(const Channel &ch, std::string msg)
{
    const std::map<int, Client> &clts = ch.clientsInChannel;
    std::map<int, Client>::const_iterator it = clts.begin();

    for (; it != clts.end(); it++) {
        Server::sendMsg(it->second, msg);
    }
}

void    Server::sendMsg(const Channel &ch, const Client &except, std::string msg)
{
    const std::map<int, Client> &clts = ch.clientsInChannel;
    std::map<int, Client>::const_iterator it = clts.begin();

    for (; it != clts.end(); it++) {
        if (it->second.fd != except.fd)
            Server::sendMsg(it->second, msg);
    }
}
