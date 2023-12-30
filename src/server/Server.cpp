#include "Server.hpp"
#include "InputOutput.hpp"

Server::Server() 
{
    std::cout << "Error [Server class]: use parameterized constructor" << std::endl;
}

Server::Server(std::string , std::string )
{

}

Server& Server::operator=(Server &)
{
    return (*this);
}

Server::~Server()
{

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