#if !defined(SERVER_HPP)
#define SERVER_HPP

/*  Containers  */
#include <vector> // std::vector
#include <string>
#include <map>
#include <utility>

/* Sockets */
#include <sys/socket.h> // socket(), bind(), listen(), connect(), accept()
#include <sys/types.h> // bind()
#include <arpa/inet.h> // inet_aton(), htons()
#include <sys/errno.h> // errno()
#include <poll.h> // poll()
#include <netdb.h> // getaddinfo()

/* I/O */
#include <iostream> // std::cout...
#include <unistd.h> // write()
#include <fcntl.h> // fcntl()

/* Exceptions */
#include <exception>

/* static headers */
#include "Client.hpp"
#include "Command.hpp"
#include "channel.hpp"

#define SOCK_DOMAIN AF_INET
#define BACKLOG SOMAXCONN
#define POLL_TIMEOUT    0 // timeout used for poll(2) || NON_BLOCKING mode
#define BUFFER_SIZE     4096 // buffer size used for tmp reading variables

class   Server {

public :

    //	creates a TCP, IPv4, Passive socket
    Server(std::string portNum, std::string password);
    Server(Server &rhs);
    Server& operator=(Server &rhs);
    ~Server();

    const unsigned short &      getListenPort() const;
    const int &                 getListenFd() const;
    const std::string &         getPassword() const;

            //  Accepts clients connections
    void    handleIncomingConnections();
            //  Add socket fd to the vector and increment nfds by 1
    void    addNewPollfd(int fd, std::vector<struct pollfd> &fds, nfds_t &nfds);
            //  checks if checks if there is a revents in one of the fds
    int     isPollReady(std::vector<struct pollfd> &fds, nfds_t &nfds);
            //  returns 0 if connection is done successfully, otherwise 0 is returned
    bool    addNewClient(std::vector<struct pollfd> &fds, nfds_t *nfds, int &fdsLeft);

    void    userRegistration(int fd, std::vector<std::string> string);

            //	checks if msg revceived has a '\n'
    std::pair<std::string, bool>   ReadIncomingMsg(std::string buff, std::map<int, std::string> &map,
                                const std::vector<struct pollfd>  &fds, unsigned long &i);

    static void    sendMsg(const Client &target, std::string msg);
    // void    sendMsg(const Channels &target, const std::string &msg);

    //  list of clients connected to the server || Nickname, Client class
    std::map<int, Client>       clients;

    //  list of channels in the server
    std::map<int, channel>       channles;
private :

	// server's password
    const std::string           password;
	// the port number on which the server is listenning
    const in_port_t             listenPort;
	// listenning fd (not const because it is set after socket is created)
    int                         listenFd;

    // make class command a friend to server class to get client id from class command
    friend class command;
};

                //  prints date, time, host, ip and port in STDOUT
void            serverWelcomeMessage(const struct sockaddr_in &srvSock, int sfd);
                //  prints on client side
void            clientWelcomeMessage(unsigned short cfd);
                //  print IP and host of connected client on server side
void            printNewClientInfoOnServerSide(const struct sockaddr_in &cltAddr);
                //  returns current local time
std::string     geTime();
                //  Checks if fd.revents == POLLIN
bool            isReadable(const struct pollfd &fd);
                //  checks if fd.revents == POLLERR | POLLHUP
bool            isError(int revents, int fd, int listenFd);
                //  checks if (revents == POLLIN) && (fd == server fd)
bool            isNewConnection(const struct pollfd &fd, int srvfd);
                //  type=0 : <client> || type=1 : nick!~user@hostname
std::string     getId(Client &clt, int type);


std::vector<std::string>     splitByLines(std::string buff);

#endif // SERVER_HPP
