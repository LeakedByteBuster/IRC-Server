#include "Server.hpp"
#include <cstring>
#include "registrationCommands.hpp"


/* -------------------------------------------------------------------------- */
/*                            Server constructors                             */
/* -------------------------------------------------------------------------- */

//   Creates a TCP, IPv4, Passive socket
Server::Server(std::string portNum, std::string password) : 
                                password(password), listenPort(htons(atoi(portNum.data())))
{
    // For setsockopt(), the parameter should be non-zero to enable a boolean option
    int optVal = 1;
    
    struct addrinfo  hints, *res, *res0;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE | AI_CANONNAME;
    hints.ai_socktype = SOCK_STREAM;
    int error = 0;

    if ((error = getaddrinfo(NULL, portNum.data(), (const struct addrinfo *)&hints, &res0)) != 0) {
        throw std::runtime_error("Error getaddrinfo() : " 
            + static_cast<std::string>(gai_strerror(error)));
        std::cerr.flush();
    }
    int check = 1;
    for (res = res0; res != NULL; res = res->ai_next) {
        //  creates server socket and store fd
        int sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sfd == -1) {
            std::cerr.flush();
            continue ;
        }
        // set socket option. SO_REUSEADDR : enables local address reuse
        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)) == -1) {
            close(sfd);
            continue ;
        }

        // bind the listenning socket to a specified port and address
        if (bind(sfd, res->ai_addr, res->ai_addrlen) == -1) {
            close(sfd);
            continue ;
        }
        listenFd = sfd; //  listenFd variable is used to close fd in destructor
        //  Makes all I/O non blocking
        if (fcntl(listenFd, F_SETFL, O_NONBLOCK) == -1) {
            throw std::runtime_error("Error fcntl() : " + 
                static_cast<std::string>(strerror(errno)));
        }

        // Marks the socket as a passive socket
        if (listen(listenFd, BACKLOG) == -1) {
            throw std::runtime_error("Error listen() : " + 
                static_cast<std::string>(strerror(errno)));
        }

        #if defined(LOG)
            std::cout   << geTime() << std::endl;
            std::cout << "Canonical name : " << res->ai_canonname << std::endl;
            serverWelcomeMessage(*(reinterpret_cast<sockaddr_in*>(res->ai_addr)), sfd);
        #endif  // LOG

        check = 0;
        break ;
    }

    freeaddrinfo(res0);
    if (check)
        throw std::runtime_error("Error getaddrinfo");

}

Server::Server(Server &rhs) : password(rhs.password), listenPort(rhs.listenPort),
                            listenFd(rhs.listenFd) {
    *this = rhs;
}

Server& Server::operator=(Server &rhs)
{
    if (this == &rhs)
        return (*this);
    this->listenFd = rhs.listenFd;
    return (*this);
}

Server::~Server()
{
    //  Close server fd
    if (close(listenFd) == -1) {
        std::cerr << "Error close() : " << strerror(errno) << std::endl;
    }
    //  Close all clients fd
    std::map<int, Client>::iterator it = clients.begin();
    for (; it != clients.end(); it++) {
        if (close(it->second.fd) == -1)
            std::cerr << "Error close() : " << strerror(errno) << std::endl;
    }
}

/* -------------------------------------------------------------------------- */
/*                             Setters & Getters                              */
/* -------------------------------------------------------------------------- */

const unsigned short &  Server::getListenPort() const {
    return (this->listenPort);
}

const int &             Server::getListenFd() const {
    return (this->listenFd);
}

const std::string &     Server::getPassword() const {
    return (this->password);

}

// std::map<std::vector<std::string>, std::vector<Client> > Server::getChannelMap()const{
//     return (this->channelMap);
// };   

/* -------------------------------------------------------------------------- */
/*                               Server Methods                               */
/* -------------------------------------------------------------------------- */

//  Adds the socket fd to the vector<struct pollfd>
void    Server::addNewPollfd(int fd, std::vector<struct pollfd> &fds, nfds_t &nfds)
{
    struct pollfd   newSock;

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
        throw std::runtime_error("Error poll() : " + 
            static_cast<std::string>(strerror(errno)));
    } else if (ret > OPEN_MAX) {
        std::cerr 
            << "Warning poll() : max open files per process is reached"
            << std::endl;
        return (0);
    }
    return (ret);
}

//  returns 0 if connection is done successfully, otherwise 0 is returned
bool    Server::addNewClient(std::vector<struct pollfd> &fds, nfds_t *nfds, int &fdsLeft)
{
    Client      clt;
    socklen_t   sockLen = 0; // used for accept()

    //  Accepting incoming connection
    memset(&clt.hints, 0, sizeof(clt.hints));
    sockLen = sizeof(clt.hints);
    if ((clt.fd = accept(listenFd, (sockaddr *)&clt.hints, &sockLen)) == -1) {
        fdsLeft--;
        std::cerr << "Error accept() : " << strerror(errno) << std::endl;
        return (EXIT_FAILURE);
    }
    //  Setting the client fd to NON_BLOCKING mode
    if (fcntl(clt.fd, F_SETFL, O_NONBLOCK) == -1) {
        std::cerr << "Error fcntl() : undefined error" << std::endl;
        fdsLeft--;
        return (EXIT_FAILURE);
    }
    //  Add client fd to the poll of file descriptors
    Server::addNewPollfd(clt.fd, fds, *nfds);

    #if defined(LOG)
        printNewClientInfoOnServerSide(clt.hints);
        clientWelcomeMessage(clt.fd);
    #endif  // LOG

    //  Push the new client to map<> in the server
    clients.insert(std::pair<int, Client>(clt.fd, clt));
    // Decrement number of fds returned by poll()
    fdsLeft--;

    return (EXIT_SUCCESS);
}

// used for capturing all 3 lines or more at once (for registration)
std::map<int, std::vector<std::string> >    gbuff;
void    Server::userRegistration(int fd, std::vector<std::string> string)
{
    std::pair<std::map<int, std::vector<std::string> >::iterator, bool>  it;
    it = gbuff.insert(std::pair<int, std::vector<std::string> >(fd, string));

    if ((it.second == false) || (string.size() >= 3)) {
        
        for (size_t i = 0; i < string.size(); i++) {
            if (!string[i].empty())
                gbuff[fd].push_back(string[i]);
            }

        if (gbuff[fd].size() >= 3) {
            try  {
                parseRegistrationCommands(clients, gbuff[fd], clients[fd], password);
                clients[fd].isRegistred = 1;
                gbuff.erase(fd);

            } catch (std::exception &e) {
                // std::cout << e.what() << std::endl;
                clients[fd].nickname.clear();
                clients[fd].realname.clear();
                gbuff.erase(fd);
            }
        }

    }

    return ;
}

std::pair<std::string, bool>    Server::ReadIncomingMsg(std::string buff, std::map<int, std::string> &map,
                            const std::vector<struct pollfd>  &fds, unsigned long &i)
{

    //  if buff doesn't have '\n' at the end
    if (buff.rfind('\n') == std::string::npos) {
        std::pair<std::map<int, std::string>::iterator,bool> itRet;
        itRet = map.insert(std::pair<int, std::string>(fds[i].fd, buff));
        if (itRet.second == false) {
            map[fds[i].fd].append(buff); // join buff
        }
        return (make_pair(static_cast<std::string>(""), 0));
    } 
    // if client sent a '\n' but he has already a buff stored in map
    if ( !map.empty() && (buff.find('\n') != std::string::npos)
                && !map[fds[i].fd].empty() ) {
        buff = map[fds[i].fd].append(buff);
        map.erase(fds[i].fd);
    }

    #if defined(LOG)
        std::cout << "buff in ReadIncomingMsg() : " << buff;
        std::cout.flush();
    #endif // LOG

    return (make_pair(buff, 1));
}
//  Accepts incoming connections
void            Server::handleIncomingConnections()
{
    #define BYTES_TO_READ   4096

    std::map<int, std::string>  map; // used as a buff when "\n" is not found
    std::vector<struct pollfd>  fds; // holds all connection accepted
    std::string                 buff; // for recv()
    ssize_t                     bytes; // for recv()
    nfds_t                      nfds = 0; // size of fds vector

    buff.resize(BYTES_TO_READ);

    //  add the server socket fd to the pollfd vector
    Server::addNewPollfd(listenFd, fds, nfds);

    int fdsLeft = 0;
    while (1) {
        if ((fdsLeft = isPollReady(fds, nfds))) {

            //  Checks if there is a new connection to accept
            if (isNewConnection(fds[0], listenFd)) {
                addNewClient(fds, &nfds, fdsLeft);
            }

            for (unsigned long i = 1; (i < nfds) && (fdsLeft > 0); i++) {
                if (isReadable(fds[i])) {
                    //  Read from client file descriptor
                    memset((void *)buff.data(), 0, sizeof(buff));
                    char ptr[BYTES_TO_READ];
                    memset(ptr, 0, sizeof(ptr));
                    bytes = recv(fds[i].fd, (void *)ptr, sizeof(ptr) - 1, 0);
                    ptr[bytes] = 0;
                    if (bytes == -1) {
                        std::cout << "Error recv(): an error occured" << std::endl;
                        std::cout.flush();
                    } else if (bytes > 0) {
                            buff = ptr;
                            std::pair<std::string, bool>    str;
                            str = ReadIncomingMsg(buff, map, fds, i);
                            if (str.second == 1) { // '\n' is in the message
                                std::vector<std::string> strings = splitByLines(str.first);
                                if ((clients[fds[i].fd].isRegistred == 0)
                                        && strings.size() > 0) {
                                    userRegistration(fds[i].fd, strings);
                                } else if (clients[fds[i].fd].isRegistred == 1 
                                    && strings.size() > 0) {
                                    std :: vector<std :: string> commands;
                                    HandleIncomingMsg(commands, str.first);
                                    execute_commmand(this, commands, fds[i].fd);
                                }
                            }
                    }
                    fdsLeft--;

                } else if (isError(fds[i].revents, fds[i].fd, listenFd)) {

                    puts("2");
                    #if defined(LOG)
                        std::cout << geTime() << " | client disconnected " << std::endl;
                    #endif // LOG

                    gbuff.erase(fds[i].fd);
                    //  Close client file descriptor
                    close(fds[i].fd);
                    // remove client from list clients that may have a buff not complete
                    map.erase(fds[i].fd);
                    // delete client from vector given to poll()
                    fds.erase(fds.begin() + i);
                    // delete client from list of clients in server
                    clients.erase(fds[i].fd);
                    // decrement number of file descriptors in pollfd
                    nfds--;
                    // decrement number of file descriptors handeled
                    fdsLeft--;
                }
            }
        }
    }
    #undef BYTES_TO_READ // BYTES_TO_READ
}

void    Server::sendMsg(const Client &target, std::string msg)
{
    if (msg.size() > 0) { 
        msg.append("\r\n");

        ssize_t bytes;
        if ((bytes = send(target.fd, msg.data(), msg.size(), 0)) == -1) {
            std::cerr << "Error sendMsg(): " << strerror(errno) << std::endl;
        }
        else if (static_cast<unsigned long>(bytes) != msg.size()) {
            std::cerr << "Warning sendMsg: data loss : input = " << msg.size() 
                << " sent = " << bytes << std::endl;
        }
    } else {
        std::cerr << "Error sendMsg() : sending an empty message" << std::endl;
    }
}

// void    Client::sendMsg(const Channels &, const std::string &) 
// {
    /*
    for (all clients in channel) {
        if (bytes = send(target.fd, msg.data(), msg.size(), 0) == -1) {
            std::cerr << "Error : " << strerror(errno) << std::endl;
        }
        if (bytes )
    }
    */
// }