#if !defined(SERVER_HPP)
#define SERVER_HPP

/*  Containers  */
#include <utility>

/* Sockets */
#include <sys/socket.h> // socket(), bind(), listen(), connect(), accept()
#include <sys/types.h> // bind()
#include <arpa/inet.h> // inet_aton(), htons()
#include <sys/errno.h> // errno()
#include <poll.h> // poll()
#include <netdb.h> // getaddinfo()

/* I/O */
#include <unistd.h> // write()
#include <fcntl.h> // fcntl()

/* Exceptions */
#include <exception>

/* static headers */
#include "Client.hpp"
#include "Command.hpp"
#include "channel.hpp"
#include "utils.hpp"

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

    static void    sendMsg(const Client &target, std::string msg);
    // void    sendMsg(const Channels &target, const std::string &msg);

    std::string postRegistration(const Client &clt);

    //  list of clients connected to the server || Nickname, Client class
    std::map<int, Client>       clients;
    std::string                 serverCreationDate;
    //  list of channels in the server
    static std::map<std::string, channel>  ChannelsInServer;

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

std::map<std::string, channel>  Server::ChannelsInServer;

#endif // SERVER_HPP
