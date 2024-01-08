#pragma once 

#include <iostream>
#include <vector>
#include <map>
#include <sstream>

#include "Errors.hpp"

class Server;
// handle the buffer that's comming from server and parse it into vector<string>
std :: vector<std :: string> HandleIncomingMsg(std :: vector<std :: string> & commands,std :: string msg);
// compare first string in vector too see if is a valid command
void execute_commmand(Server *sev,std :: vector<std :: string> & commands,int id);
void send_file(Server *sev,std :: vector<std :: string> & commands,int id);
#include "Server.hpp"