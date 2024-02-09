#include <time.h> // time()
#include <poll.h> // poll()
#include <unistd.h> // write()
#include <netdb.h> // getaddinfo()
#include <arpa/inet.h> // inet_aton(), htons()
#include <iostream> // std::cout...
#include "registrationCommands.hpp"
#include "utils.hpp"
#include "Command.hpp"
#include "Messages.hpp"
#include "Server.hpp"

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

std::pair<std::string, bool>    parseInput(std::string buff, std::map<int, std::string> &map,
                            const std::vector<struct pollfd>  &fds, unsigned long &i)
{
    //  if buff doesn't have '\n' at the end
    if (buff.rfind("\n") == std::string::npos) {
        std::pair<std::map<int, std::string>::iterator,bool> itRet;
        itRet = map.insert(std::pair<int, std::string>(fds[i].fd, buff));
        if (itRet.second == false) {
            map[fds[i].fd].append(buff); // join buff
        }
        return (make_pair(static_cast<std::string>(""), 0));
    } 
    // if client sent a '\n' but he has already a buff stored in map
    if ( !map.empty() && (buff.find("\n") != std::string::npos)
                && !map[fds[i].fd].empty() ) {
        buff = map[fds[i].fd].append(buff);
        map.erase(fds[i].fd);
    }

    return (make_pair(buff, 1));
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

int whichCommand(const std::string &first_argument)
{
    int ret =   0;
    ret =   (first_argument.compare("SENDFILE") == 0)   * SENDFILE \
            + (first_argument.compare("GETFILE") == 0)  * GETFILE \

            + (first_argument.compare("NICK") == 0)     * NICK \
            + (first_argument.compare("nick") == 0)     * NICK \

            + (first_argument.compare("PASS") == 0)     * PASS_USER \
            + (first_argument.compare("pass") == 0)     * PASS_USER \

            + (first_argument.compare("USER") == 0)     * PASS_USER \
            + (first_argument.compare("user") == 0)     * PASS_USER \

            + (first_argument.compare("PRIVMSG") == 0)   * PRVMSG \
            + (first_argument.compare("privmsg") == 0)   * PRVMSG \

            + (first_argument.compare("PONG") == 0)     * PONG \

            + (first_argument.compare("/DATE") == 0)    * DATE \
            + (first_argument.compare("/date") == 0)    * DATE \

            + (first_argument.compare("BOT") == 0)    * IRCBOT \
            + (first_argument.compare("bot") == 0)    * IRCBOT \

            + (first_argument.compare("/JOKE") == 0)    * JOKE \
            + (first_argument.compare("/joke") == 0)    * JOKE \

            + (first_argument.compare("/whoami") == 0)  * WHOAMI \
            + (first_argument.compare("/WHOAMI") == 0)  * WHOAMI \
            
            + (first_argument.compare("join") == 0 ||
                first_argument.compare("JOIN") == 0)  * JOIN;
    return (ret);
}

std::string Server::postRegistration(const Client &clt)
{
    std::string str;
    
    // RPL_WELCOME
    str = static_cast<std::string>(":") + SERVER_PREFIX + "001 " + clt.nickname;
    str.append(
        " :Welcome to the Camel Internet Relay Chat Network " + clt.nickname
    );
    Server::sendMsg(clt, str);
    str.clear();
    
    // RPL_YOURHOST
    str = static_cast<std::string>(":") + SERVER_PREFIX + "002 " + clt.nickname;
    str.append(
        static_cast<std::string>(" :Your host is ") 
        + SERVER_PREFIX
        + ", running version " 
        + SERVER_VERSION
    );
    Server::sendMsg(clt, str);
    str.clear();

    // RPL_CREATED
    str = static_cast<std::string>(":") + SERVER_PREFIX + "003 " + clt.nickname;
    str.append(
        " :This server was created on " + serverCreationDate
    );
    Server::sendMsg(clt, str);
    str.clear();

    // RPL_MYINFO
    /*
        <client> <servername> <version>
        <available user modes>              : ol
        <available channel modes>           : itkol
        <channel modes with a parameter>    : lok
    */
    str = static_cast<std::string>(":") + SERVER_PREFIX + "004 " + clt.nickname + " ";
    str.append(
        static_cast<std::string>(":")
        + SERVER_PREFIX
        + SERVER_VERSION 
        + " ol "
        + "itkol " 
        + "lok"
    );
    Server::sendMsg(clt, str);
    str.clear();

    return (str);
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
