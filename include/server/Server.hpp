#if !defined(SERVER_HPP)
#define SERVER_HPP

/*  Containers  */
#include <vector> // std::vector
#include <utility> // std::pair

#include <sys/socket.h> // socket(), bind(), listen(), connect(), accept()
#include <sys/types.h> // bind()
#include <arpa/inet.h> // inet_aton(), htons()

#include "InputOutput.hpp"

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

//  configure address, port, ai_flags, protocol... of the listening socket.
void    initSockAddrStruct(struct sockaddr_in *sock, unsigned short lport);

#endif // SERVER_HPP