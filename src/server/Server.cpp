#include "Server.hpp"
#include <poll.h>

/* -------------------------------------------------------------------------- */
/*                            Server constructors                             */
/* -------------------------------------------------------------------------- */

//   Creates a TCP, IPv4, Passive socket
Server::Server(const in_port_t portNum, std::string password) : 
                                password(password), listenPort(portNum)
{
    // used for bind function
    struct sockaddr_in  sockAddr;
    // For setsockopt(), the parameter should be non-zero to enable a boolean option
    int optVal = 1;
    //  creates server socket and store fd
    int sfd = socket(SOCK_DOMAIN, SOCK_STREAM, 0);
    if (sfd == -1) {
        throw std::runtime_error("Error socket() : " + static_cast<std::string>
            (strerror(errno)));
    }
    listenFd = sfd; //  listenFd variable is used to close fd in destructor
    //  Makes all I/O non blocking
    if (fcntl(listenFd, F_SETFL, O_NONBLOCK) == -1) {
        std::cerr << "Error fcntl() : " << std::endl;
    }

    // set socket option. SO_REUSEADDR : enables local address reuse
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal))) {
        throw std::runtime_error("Error setsocketopt() : " + 
            static_cast<std::string>(strerror(errno)));
    }
    // bind the listenning socket to a specified port and address
    initSockAddrStruct(&sockAddr, listenPort);
    if (bind(sfd, (sockaddr *)&sockAddr, sizeof(sockAddr)) == -1) {
       throw std::runtime_error("Error setsocketopt() : " + 
            static_cast<std::string>(strerror(errno)));
    }
    // Marks the socket as a passive socket
    if (listen(sfd, BACKLOG) == -1) {
        throw std::runtime_error("Error listen() : " + 
            static_cast<std::string>(strerror(errno)));
    }
    serverWelcomeMessage(sockAddr, sfd);
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
    close(listenFd);

#define IT_ELEMENT std::vector<std::pair<int, struct sockaddr_in> >::iterator

    for (IT_ELEMENT it = clientsFds.begin(); it != clientsFds.end(); it++) {
            if (close(it->first) == -1)
                throw std::runtime_error("Error close()");
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

const std::vector<std::pair<int, struct sockaddr_in> >  &
        Server::getClientsFds() const {
    return (this->clientsFds);
}

/* -------------------------------------------------------------------------- */
/*                               Server Methods                               */
/* -------------------------------------------------------------------------- */

//  Adds the socket fd to a vector<struct pollfd>
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

//  Accepts incoming connections
void            Server::handleIncomingConnections()
{
    std::vector<struct pollfd>  fds; // holds all connection accepted
    struct  sockaddr_in         cltAddr; // used for accept()
    std::string                 buff; // for recv()
    ssize_t                     bytes; // for recv()
    nfds_t                      nfds = 0; // size of fds vector
    socklen_t                   sockLen = 0; // used for accept()
    int                         cltFd = 0; // used for fd returned by accept

    buff.resize(1024);

    //  add the server socket fd to the pollfd vector
    Server::addNewPollfd(listenFd, fds, nfds);

    while (1) {
        if (isPollReady(fds, nfds)) {

            //  Checks if there is a new connection to accept
            if (isNewConnection(fds[0], listenFd)) {
                memset(&cltAddr, 0, sizeof(cltAddr));
                sockLen = sizeof(cltAddr);
                if ((cltFd = accept( // error case
                                listenFd,
                                (sockaddr *)&cltAddr,
                                &sockLen)) == -1 ) {
                    std::cerr << "Error accept() : " << strerror(errno)
                        << std::endl;
                }
                if (fcntl(cltFd, F_SETFL, O_NONBLOCK) == -1) {
                    std::cerr << "Error fcntl() : undefined error" << std::endl;
                }
                Server::addNewPollfd(cltFd, fds, nfds);
                printNewClientInfoOnServerSide(cltAddr);
                clientWelcomeMessage(cltFd);

                clientsFds.push_back(std::make_pair(cltFd, cltAddr));
            }

            for (unsigned long i = 1; i < nfds; i++) {
   
                if (((fds[i].revents & POLLIN) == POLLIN)
                    && (fds[i].revents & POLLHUP) != POLLHUP) {
   
                    memset((void *)buff.data(), 0, sizeof(buff));

                    bytes = recv(fds[i].fd, (void *)buff.data(), sizeof(buff), 0);
                    if (bytes == 0) {
                        std::cout << "Error recv(): 0 byte" << std::endl;
                        std::cout.flush();
                    } else {
                        // check if '\n' exists
                        std::cout << "buff is : " << buff << std::endl;
                    }
                } else if (((fds[i].revents & POLLHUP) == POLLHUP
                            || (fds[i].revents & POLLERR) == POLLERR)
                            && fds[i].fd != listenFd) {
                    std::cout << geTime() << " | client disconnected " << std::endl;
                    close(fds[i].fd);
                    fds.erase(fds.begin() + i);
                    nfds--; // decrement number of file descriptors in pollfd
                }
            }
        }
    }
}

// /*
//   int ret = 0;
//         if ((ret = poll(fds.data(), nfds, 1000)) == -1) {
//                 std::cerr << "Error poll() : " << std::endl;
//         }
//         struct sockaddr_in  cltAddr;
//         socklen_t cltAddrLen = sizeof(cltAddr);
//         memset(&cltAddr, 0, sizeof(cltAddr));
        
//         int cfd = accept(listenFd, (sockaddr *)&cltAddr, &cltAddrLen);
//         if (cfd != -1) {
//             fcntl(cfd, F_SETFL, O_NONBLOCK);

//             /* Sets fd and events for poll */
//             memset(&newConnection, 0, sizeof(newConnection));
//             newConnection.fd = cfd;
//             newConnection.events = POLLIN | POLLOUT;
//             fds.push_back(newConnection);

//             fcntl(fds.back().fd, F_SETFL, O_NONBLOCK);
            
//             /* Storing  */
//             clientsFds.push_back(std::make_pair(cfd, cltAddr));
//             clientWelcomeMessage(cltAddr, listenPort, cfd);
//             printNewClientInfoOnServerSide(cltAddr);
//             nfds++; // number of pollfd struct +1
//             continue ;
//         }
//         for (unsigned long i = 0; i < fds.size(); i++) {
//             if (((fds[i].revents & POLLIN) == POLLIN)
//                 && (fds[i].revents & POLLHUP) != POLLHUP) {
//                 memset(buff, 0, sizeof(buff));
                
//                 recv(fds[i].fd, buff, sizeof(buff), 0);
//                 // read(fds[i].fd, buff, BUFFER_SIZE);
//                 // if (bytes == 0) {
//                 //     std::cout << "read() : Client closed connection" << std::endl;
//                 //     fds[i].revents = POLLERR;
//                 // } else {
//                     std::cout << "buff is : " << buff << std::endl;
//             } 
//         }
// */