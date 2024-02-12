#if !defined(CLIENT_HPP)
#define CLIENT_HPP

#include <iostream>
#include <arpa/inet.h>
#include <vector>
#include <utility>
#include <map>
#include "TFile.hpp"
#include "Modes.hpp"




class Channel;


class   Client : public ClientOperator {

public :

    std::map<std::string, Channel> ChannelIn; // map of channel that the client memmber of them
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

