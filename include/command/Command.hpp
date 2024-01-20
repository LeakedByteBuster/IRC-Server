#pragma once 

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include "TFile.hpp"
#include "Errors.hpp"

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
void execute_commmand(Server *sev,std :: vector<std :: string> & commands,int id);
void send_file(Server *sev,std :: vector<std :: string> & commands,Client cl);
void get_file(Server *srv,std :: vector<std :: string> command,Client cl);
int search_a_file(Client clt,std :: string sender);
void creat_file(Client clt,std :: string sender,std :: string filename);
void prv_msg(Server *srv,std::vector<std :: string>command,int id);