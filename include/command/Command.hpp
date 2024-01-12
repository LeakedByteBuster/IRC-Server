#pragma once 

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include "Server.hpp"
class Server ;
// handle the buffer that's comming from server and parse it into vector<string>
std :: vector<std :: string> HandleIncomingMsg(std :: vector<std :: string> & commands,std :: string msg);
void execute_commmand(Server srv ,std :: vector<std :: string> & commands,int id);