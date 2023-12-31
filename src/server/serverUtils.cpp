#include <time.h> // time()
#include <iostream>
#include "Server.hpp"

static const char *    getBigMsg();

void    initSockAddrStruct(struct sockaddr_in *sock, unsigned short lport) {
    memset(sock, 0, sizeof(*sock));
    sock->sin_family = SOCK_DOMAIN;
    sock->sin_port = lport;
    sock->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

//  Returns local current time
static const char * geTime() 
{
    time_t  t;
    time(&t);
    const char *currentTime = asctime(localtime(&t));
    return (currentTime);
}

void    serverWelcomeMessage(const struct sockaddr_in &srvSock, int)
{
    char    buff[512];

    if (gethostname(buff, sizeof(buff)) == -1){
        std::cerr << "Warning gethostname() : " << strerror(errno) << std::endl;
    }

    std::cout   << geTime()
                << "host : " << buff << std::endl
                << "IP   : " << inet_ntoa(srvSock.sin_addr) << std::endl
                << "Port : " << ntohs(srvSock.sin_port)
                << std::endl << std::endl;
}

void    clientWelcomeMessage(
    const struct sockaddr_in &, in_port_t , int cfd)
{
    std::string s(getBigMsg() + static_cast<std::string>("\n\n") + geTime());
    write(cfd, s.data(), s.size());
}

void    printNewClientInfoOnServerSide(const struct sockaddr_in &cltAddr)
{
    std::cout   << geTime()
                << " new client | IP : " << inet_ntoa(cltAddr.sin_addr)
                << " | port : " << ntohs(cltAddr.sin_port)
                << std::endl;

}

static const char *    getBigMsg()
{
        const char    *bigMsg =

  "\n$$$$$$\\ $$$$$$$\\   $$$$$$\\         $$$$$$\\ $$$$$$$$\\         $$$$$$\\                                   \n"
        "\\_$$  _|$$  __$$\\ $$  __$$\\       $$  __$$\\__$$  __|       $$ ___$$\\                                  \n"
        "  $$ |  $$ |  $$ |$$ /  \\__|      $$ /  $$ |  $$ | $$$$$$\\  \\_/   $$ |                                 \n"
        "  $$ |  $$$$$$$  |$$ |            \\$$$$$$$ |  $$ |$$  __$$\\   $$$$$ /                                  \n"
        "  $$ |  $$  __$$< $$ |             \\____$$ |  $$ |$$$$$$$$ |  \\___$$\\                                  \n"
        "  $$ |  $$ |  $$ |$$ |  $$\\       $$\\   $$ |  $$ |$$   ____|$$\\   $$ |                                 \n"
        "$$$$$$\\ $$ |  $$ |\\$$$$$$  |      \\$$$$$$  |  $$ |\\$$$$$$$\\ \\$$$$$$  |                                 \n"
        "\\______|\\__|  \\__| \\______/        \\______/   \\__| \\_______| \\______/                                  \n"
        "                                                                                                       \n"
        "                                                                                                       \n"
        "                                                                                                       \n"
        "$$\\                                $$\\ $$\\                 $$\\       $$\\       $$\\       $$\\           \n"
        "$$ |                               $$ |\\__|                $$ |      $$ |      $$ |      \\__|          \n"
        "$$ |      $$$$$$\\  $$\\   $$\\  $$$$$$$ |$$\\  $$$$$$\\        $$ |      $$ |  $$\\ $$$$$$$\\  $$\\  $$$$$$\\  \n"
        "$$ |      \\____$$\\ $$ |  $$ |$$  __$$ |$$ |$$  __$$\\       $$ |      $$ | $$  |$$  __$$\\ $$ |$$  __$$\\ \n"
        "$$ |      $$$$$$$ |$$ |  $$ |$$ /  $$ |$$ |$$ |  \\__|      $$ |      $$$$$$  / $$ |  $$ |$$ |$$ |  \\__|\n"
        "$$ |     $$  __$$ |$$ |  $$ |$$ |  $$ |$$ |$$ |            $$ |      $$  _$$<  $$ |  $$ |$$ |$$ |      \n"
        "$$$$$$$$\\$$$$$$$ |\\$$$$$$$ |\\$$$$$$$ |$$ |$$ |            $$$$$$$\\ $$ | \\$$\\ $$ |  $$ |$$ |$$ |      \n"
        "\\________|\\____$$ | \\____$$ | \\_______|\\__|\\__|            \\________|\\__|  \\__|\\__|  \\__|\\__|\\__|      \n"
        "                   $$\\   $$ |                                                                              \n"
        "                   \\$$$$$$  |                                                                              \n"
        "                    \\______/                                                                               \n";
    return (bigMsg);
}