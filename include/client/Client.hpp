#if !defined(CLIENT_HPP)
#define CLIENT_HPP

#include <iostream>
#include <arpa/inet.h>
#include "TFile.hpp"
#include <vector>

class   Operator {

public :
    /* Eject a client from the channel */
    void    kick(){}
    /* invite a client to a channel */
    void    invite(){}
    /* Change or view the channel topic */
    void    topic(){}
    /* Change the channel’s mode */
    void    mode(/* int mode */){}
};

class   Client : public Operator {

public :
    /* doe */
    bool                isRegistred;
    /* operator priviligies */
    bool                isOperator;
    /* client info */
    std::string         nickname;
    std::string         realname;
    std::string         username;
    std::string         hostname;
    // /* Connection infos */
    struct sockaddr_in  hints;
    int                 fd;

    std :: vector<TFile> Files;

public :
    Client();
    Client(std::string port, std::string password);
    Client& operator=(Client &rhs);
    virtual ~Client();

    void    sendMsg(const Client &target, std::string msg);
    // void    sendMsg(const Channels &target, const std::string &msg);
};

#endif // CLIENT_HPP
