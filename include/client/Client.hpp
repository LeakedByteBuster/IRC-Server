#if !defined(CLIENT_HPP)
#define CLIENT_HPP

#include <iostream>
#include <arpa/inet.h>
#include "TFile.hpp"
#include "Modes.hpp"
#include <vector>

class   Client : public ClientOperator {

public :
    std :: vector<TFile> Files;
    struct sockaddr_in  hints;
    std::string         username;
    std::string         realname;
    std::string         nickname;
    const int           fd;
    bool                isRegistred; // Successfully registered

public :
    Client();
    Client(const int fd);
    Client(const Client &rhs);
    Client& operator=(const Client &rhs);
    virtual ~Client();
    

};

#endif // CLIENT_HPP

