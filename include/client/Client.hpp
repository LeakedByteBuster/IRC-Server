#if !defined(CLIENT_HPP)
#define CLIENT_HPP

#include "InputOutput.hpp"

class   Client {
private :

public :
    Client();
    Client(std::string port, std::string password);
    Client& operator=(Client &rhs);
    ~Client();
};

#endif // CLIENT_HPP