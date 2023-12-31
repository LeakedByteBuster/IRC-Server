#if !defined(SERVER_HPP)
#define SERVER_HPP

/*  Containers  */
#include <vector> // std::vector
#include <utility> // std::pair

#include <sys/socket.h> // socket(), bind(), listen(), connect(), accept()
#include <sys/types.h> // bind()
#include <arpa/inet.h> // inet_aton(), htons()
#include <sys/errno.h> // errno

#include "InputOutput.hpp"

#define SOCK_DOMAIN AF_INET
#define BACKLOG SOMAXCONN


class   Server {

public :
    /* 
     * creates a TCP, IPv4, Passive socket.  * it doesn't accept any connection
     * call handleIncomingConnections() 
     */
    Server(const in_port_t portNum, std::string password);
    Server(Server &rhs);
    Server& operator=(Server &rhs);
    ~Server();

    const unsigned short &      getListenPort() const;
    const int &                 getListenFd() const;
    const std::string &         getPassword() const;
    const std::vector<std::pair<int, struct sockaddr_in> >  &    getClientsFds() const;

    // Accepts clients connections
    void    handleIncomingConnections();


private :
    const std::string   password; // password to the server
    const in_port_t     listenPort; // the port number on which the server is listenning
    int                 listenFd; // not const because it is set after socket is created
    /*
     * int is 'fd' of client socket, and sockaddr_in is 'the result returned by
     * accept(2)' 
    */
    std::vector<std::pair<int, struct sockaddr_in> >    clientsFds;

    //static std::unordred_map<std::string, std::string>   channels;
    //static std::unordred_map<std::string, std::string>   users;
};

//  configure address, port, protocol... of the listening socket.
void    initSockAddrStruct(struct sockaddr_in *sock, unsigned short lport);
//  prints date, time, host, ip and port in STDOUT
void    serverWelcomeMessage(const struct sockaddr_in &srvSock, int sfd);
//  prints on client side
void    clientWelcomeMessage(
                const struct sockaddr_in &cltSock,
                in_port_t srvPort,
                int cfd
        );
//  print IP and host of connected client on server side
void    printNewClientInfoOnServerSide(const struct sockaddr_in &cltAddr);



#endif // SERVER_HPP