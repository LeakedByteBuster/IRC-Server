#if !defined(SERVER_HPP)
#define SERVER_HPP

#include <sys/socket.h> // socket(), bind(), listen(), connect(), accept()
#include <sys/types.h> // bind()
#include <arpa/inet.h> // inet_aton(), htons()

#include "InputOutput.hpp"

class   Server {
private :

public :
    Server();
    Server(std::string port, std::string password);
    Server& operator=(Server &rhs);
    ~Server();
};

#endif // SERVER_HPP