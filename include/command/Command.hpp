#pragma once 

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include "TFile.hpp"
#include "Messages.hpp"
#include "channel.hpp"
#include <utility>

class Server;

enum    Commands {
    UNKNOWN,
    SENDFILE,
    GETFILE,
    NICK,
    PASS_USER,
    PRVMSG,
    PONG,
    IRCBOT
};

// compare first string in vector too see if is a valid command
void                        execute_commmand(std::map<int,Client> &clients, std ::vector<std ::string> &commands, int id);

//  FTP
void                        send_file(std::map<int,Client> &clients,std :: vector<std :: string> & commands,Client &cl);
void                        get_file(std::map<int,Client> &clients,std :: vector<std :: string> &command,Client &cl);
int                         search_a_file(Client clt,std :: string sender);
void                        creat_file(Client clt,std :: string sender,std :: string filename);
const char *                getDownMsg(void);



// void                        prv_msg(std::map<int,channel> &channels,std::vector<std :: string>command,Client clt,std::map<int,Client> Clients);
// void                        check_targets(std::map<int,channel> channels,std::vector<std::string>commmand,Client clt,size_t position,std::map<int,Client> clients);
// int                        search_in_channels(std::map<int,channel> channels,std::string name,Client clt);
// int                        search_client_inChannel(Client clt,channel channel);
// void                       sendPrvmsg(Client sender,std::string msg,Client recv);
