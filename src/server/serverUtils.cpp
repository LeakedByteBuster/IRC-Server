#include <time.h> // time()
#include "Server.hpp"

static const char *    getBigMsg();

/* -------------------------------------------------------------------------- */
/*                          Sockets helper functions                          */
/* -------------------------------------------------------------------------- */

bool    isNewConnection(const struct pollfd &fd, int srvfd)
{
    if (((fd.revents & POLLIN) == POLLIN) && (fd.fd == srvfd))
        return (1);
    return (0);
}

bool    isReadable(const struct pollfd &fd)
{
    return (((fd.revents & POLLIN) == POLLIN) 
                && ((fd.revents & POLLHUP) != POLLHUP));
}

bool    isError(int revents, int fd, int listenFd) {
    return (((revents & POLLHUP) == POLLHUP 
                || (revents & POLLERR) == POLLERR) && fd != listenFd);
}

/* splits the buff, '\n' is the delim */
std::vector<std::string>    splitByLines(std::string buff) {
    std::stringstream   ss(buff);
    std::string         line;
    std::vector<std::string> strings;

    while (std::getline(ss, line)) {
        strings.push_back(line);
        #if defined(LOG)
            std::cout << "line in isNewLine() : '" << line << "' | " << line.size() << std::endl;
        #endif // LOG
    }
    return (strings);
}

/* -------------------------------------------------------------------------- */
/*                             Printing functions                             */
/* -------------------------------------------------------------------------- */

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
    // struct protoent *p = getprotobyname(inet_ntoa(srvSock.sin_addr));
    struct hostent *d = gethostbyname(inet_ntoa(srvSock.sin_addr));

    if (d != NULL){
        std::cout   << "host : " << d->h_name << std::endl;
        for (int i = 0; d->h_addr_list[i] != NULL; ++i) {
            char *ip = inet_ntoa(*((struct in_addr *)d->h_addr_list[i]));
            std::cout << "IP   : " << ip << std::endl;
        }
        std::cout << "Port : " << ntohs(srvSock.sin_port) << std::endl;
        return ;
    }
    std::cerr << "Warning gethostbyname() : " << strerror(errno) << std::endl;
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
                << " new connection ==> IP : " 
                << inet_ntoa(cltAddr.sin_addr)
                << " | port : "
                << ntohs(cltAddr.sin_port)
                << std::endl;
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