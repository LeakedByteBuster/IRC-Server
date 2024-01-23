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

    std::vector <std::string> inChannel;
    std :: vector<TFile> Files;
    struct sockaddr_in  hints;
    std::string         username;
    std::string         realname;
    std::string         nickname;
    const int           fd;
    bool                isOperator; // has operator privilige
    bool                isRegistred; // Successfully registered

public :
    Client();
    Client(const int fd);
    Client(const Client &rhs);
    Client& operator=(const Client &rhs);
    std::vector <std::string> &get_clients_in_channel();
    virtual ~Client();

    void    sendMsg(const Client &target, std::string msg);
    // void    sendMsg(const Channels &target, const std::string &msg);
};

#endif // CLIENT_HPP
