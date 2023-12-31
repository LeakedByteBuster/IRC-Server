#include "Server.hpp"
#include "InputOutput.hpp"
#include <poll.h>

/* -------------------------------------------------------------------------- */
/*                            Server constructors                             */
/* -------------------------------------------------------------------------- */

/* 
*   Creates a TCP, IPv4, Passive socket.  * it doesn't accept any connection
*   call handleIncomingConnections() 
*/
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
    fcntl(sfd, F_SETFL, O_NONBLOCK);

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

    for (std::vector<std::pair<int, struct sockaddr_in> >::iterator it = clientsFds.begin();
        it != clientsFds.end(); it++) {
            if (close(it->first) == -1)
                throw std::runtime_error("Error close()");
    }

    // for (int i = 0; i < clientsFds.size(); i++) {
    //     close(clientsFds.at(i));
    // }
}

/* -------------------------------------------------------------------------- */
/*                               Server Methods                               */
/* -------------------------------------------------------------------------- */

/*
*   Accepts incoming connections
*
*/
void            Server::handleIncomingConnections()
{
    while (1) {
        struct sockaddr_in  cltAddr;
        socklen_t cltAddrLen = sizeof(cltAddr);
        memset(&cltAddr, 0, sizeof(cltAddr));
        int cfd = accept(listenFd, (sockaddr *)&cltAddr, &cltAddrLen);
        if (cfd != -1) {
            char    buff[4096];
            memset(buff, 0, sizeof(buff));
            // while (std::getline() != '\n') {
            //     std::cout << buff << std::flush;
            // }
            /* Storing  */
            clientsFds.push_back(std::make_pair(cfd, cltAddr));
            clientWelcomeMessage(cltAddr, listenPort, cfd);
            printNewClientInfoOnServerSide(cltAddr);
            continue ;
        }
        else if (errno != EAGAIN) // 
            std::cerr << "Error accept(): " << strerror(errno) << std::endl;

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

const std::vector<std::pair<int, struct sockaddr_in> >  &    Server::getClientsFds() const {
    return (this->clientsFds);
}
















/*
    struct sockaddr_in  sockAddr;
    struct sockaddr_in  cltAddr;
    socklen_t addrLen  = sizeof(cltAddr);
    int clientFd = 0;
    memset(&cltAddr, 0, sizeof(cltAddr));
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(9500);
    sockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    // if (fcntl(sockFd, F_SETFL, O_NONBLOCK) == -1)
    //     return (printError("fcntl()", sockFd));

    int sockOpt = 1;
    if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &sockOpt, sizeof(sockOpt)) == -1)
        return (printError("setsockopt()", sockFd));

    if (bind(sockFd, (sockaddr *)&sockAddr, sizeof(sockAddr)) == -1)
        return (printError("bind()", sockFd));
    if (listen(sockFd, SOMAXCONN) == -1)
        return (printError("listen()", sockFd));

    char buff[4096];
    memset(&buff, 0, sizeof(buff));

    for (;;){
        if ((clientFd = accept(sockFd, (sockaddr *)&cltAddr, &addrLen)) == -1)
            return (printError("accept()", sockFd));
        int byteRead = 0;
        if ((byteRead = read(clientFd, buff, BUFF_SIZE)) == -1)
            printError("recv()", sockFd);
        else if (byteRead == 0)
            return (close(clientFd), 0);
        else
            std::cout << "{"<<buff <<"}"<< std::flush;
    } 

    close(sockFd);
    close(clientFd);

*/
