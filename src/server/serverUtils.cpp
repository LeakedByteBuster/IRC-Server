#include <iostream>
#include <string>
#include <time.h> // time()
#include "Server.hpp"

static const char *    getBigMsg();

void    initSockAddrStruct(struct sockaddr_in *sock, unsigned short lport) 
{
    memset(sock, 0, sizeof(*sock));
    sock->sin_family = SOCK_DOMAIN;
    sock->sin_port = lport;
    sock->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

//  Returns local current time
std::string  geTime()
{
    time_t  t;
    time(&t);
    std::string currentTime = asctime(localtime(&t));
    currentTime.pop_back(); // removes '\n' character from the string
    return (currentTime);
}

void    serverWelcomeMessage(const struct sockaddr_in &srvSock, int)
{
    char    buff[512];

    if (gethostname(buff, sizeof(buff)) == -1){
        std::cerr << "Warning gethostname() : " << strerror(errno) << std::endl;
    }

    std::cout   << geTime() << std::endl
                << "host : " << buff << std::endl
                << "IP   : " << inet_ntoa(srvSock.sin_addr) << std::endl
                << "Port : " << ntohs(srvSock.sin_port)
                << std::endl << std::endl;
}

void    clientWelcomeMessage(unsigned short cfd)
{

    std::string s(
        getBigMsg() + 
        static_cast<std::string>("\n\n") + 
        geTime() +
        static_cast<std::string>("\n")
    );
    write(cfd, s.data(), s.size());
}

void    printNewClientInfoOnServerSide(const struct sockaddr_in &cltAddr)
{
    std::cout   << geTime() << " |"
                << " new connection ==> IP : " << inet_ntoa(cltAddr.sin_addr)
                << " | port : " << ntohs(cltAddr.sin_port)
                << std::endl;
}

bool    isNewConnection(const struct pollfd &fd, int srvfd)
{
    if (((fd.revents & POLLIN) == POLLIN) && (fd.fd == srvfd))
        return (1);
    return (0);
}

bool    isReadable(const struct pollfd &fd)
{
    if ((fd.revents & POLLIN) == POLLIN)
        return (1);
    return (0);
}


static const char *    getBigMsg()
{
        const char    *bigMsg =

  "\n$$$$$$\\ $$$$$$$\\   $$$$$$\\  \n"
        "\\_$$  _|$$  __$$\\ $$  __$$\\  \n"
        "  $$ |  $$ |  $$ |$$ /   \n"
        "  $$ |  $$$$$$$  |$$ | \n"
        "  $$ |  $$  __$$< $$ |  \n"
        "  $$ |  $$ |  $$ |$$ | \n"
        "$$$$$$\\ $$ |  $$ |\\$$$$$$  |     \n"
        "\\______|\\__|  \\__| \\______/   \n";
    return (bigMsg);
}