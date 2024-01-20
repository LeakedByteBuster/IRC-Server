#include <time.h> // time()
#include <poll.h> // poll()
#include <unistd.h> // write()
#include <netdb.h> // getaddinfo()
#include <arpa/inet.h> // inet_aton(), htons()
#include <iostream> // std::cout...
#include "registrationCommands.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include "Command.hpp"


/* -------------------------------------------------------------------------- */
/*                          Server helper functions                           */
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

bool    isError(int revents, int fd, int listenFd)
{
    return (((revents & POLLHUP) == POLLHUP 
                || (revents & POLLERR) == POLLERR) && fd != listenFd);
}

//  Parse PASS, NICK, USER commands
bool    parseRegistrationCommands(std::map<int, Client> &clients, 
            std::vector<std::string> &str, Client &client, const std::string &pass)
{   
    for (unsigned long i = 0; i < str.size(); i++) {
        switch (i + 1)
        {
        case 1:
            parsePass(client, str[i], pass);
            break;
        case 2:
            parseNick(clients, client, str[i]);
            break;
        case 3:
            parseUser(client, str[i]);
            break;
        }
    }
    return (0);
}

//  type=0 : <client> || type=1 : nick!~user@hostname
std::string getId(Client &clt, int type)
{
    std::string str;

    switch (type)
    {
    case 0 :    /* <client> */
        str = clt.nickname.append(" ");
        break;
    case 1 :    /* [<nick> '!' <user> ] ['@' <host> ] */
        str = clt.nickname.append("!~");
        str.append(clt.username.append("@"));
        str.append(inet_ntoa(clt.hints.sin_addr));
        break;
    }
    std::cout << "string ==> " << str << std::endl;
    return (str);
}

int  parseInput(const char *port, std::string pass)
{
    char    *endptr = NULL;
    long res = strtol(port, &endptr, 10);
    if ((endptr && *endptr != '\0') || (res > USHRT_MAX)
        || (res < 0)) {
        throw std::invalid_argument("Error : invalid port number");
    }

    if (pass.length() == 0) {
        throw std::invalid_argument("Error : empty password");
    }
    return (0);
}

void    deleteClient(std::map<int, std::string> &map, std::vector<struct pollfd> &fds, 
            std::map<int, Client> &clients, std::map<int, std::vector<std::string> > &gbuff,
            nfds_t &nfds, unsigned long i, int &fdsLeft)
{
    #if defined(LOG)
        std::cout << geTime() << " | client disconnected " << std::endl;
    #endif // LOG

    // tmp buff used for registration
    gbuff.erase(fds[i].fd);
    //  Close client file descriptor
    close(fds[i].fd);
    // remove client from list clients that may have a buff not complete
    map.erase(fds[i].fd);
    // delete client from list of clients in server
    clients.erase(fds[i].fd);
    // delete client from vector given to poll()
    fds.erase(fds.begin() + i);
    // decrement number of file descriptors in pollfd
    nfds--;
    // decrement number of file descriptors handeled
    fdsLeft--;
}

bool    readIncomingMsg(char ptr[], const int id)
{
    ssize_t                     bytes;

    memset(ptr, 0, BYTES_TO_READ);
    bytes = recv(id, (void *)ptr, BYTES_TO_READ - 1, 0);
    if (bytes == -1) {
        std::cerr << "Error recv(): an error occured" << std::endl;
    }
    return (bytes > 0 ? 1 : 0);
}

/* -------------------------------------------------------------------------- */
/*                             Printing functions                             */
/* -------------------------------------------------------------------------- */

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

int whichCommand(const std::string &first_argument)
{
    int ret =   0;
    ret =   (first_argument.compare("SENDFILE") == 0) * SENDFILE \
            + (first_argument.compare("GETFILE") == 0)  * GETFILE \

            + (first_argument.compare("NICK") == 0)     * NICK \
            + (first_argument.compare("nick") == 0)     * NICK \

            + (first_argument.compare("PASS") == 0)     * PASS_USER \
            + (first_argument.compare("pass") == 0)     * PASS_USER \

            + (first_argument.compare("USER") == 0)     * PASS_USER \
            + (first_argument.compare("user") == 0)     * PASS_USER \

            + (first_argument.compare("PRVMSG") == 0)   * PRVMSG \

            + (first_argument.compare("PONG") == 0)     * PONG \

            + (first_argument.compare("/DATE") == 0)    * IRCBOT \
            + (first_argument.compare("/date") == 0)    * IRCBOT \

            + (first_argument.compare("/JOKE") == 0)    * IRCBOT \
            + (first_argument.compare("/joke") == 0)    * IRCBOT \

            + (first_argument.compare("/whoami") == 0) * IRCBOT \
            + (first_argument.compare("/WHOAMI") == 0)   * IRCBOT;

    return (ret);
}