#if !defined(QUIT_HPP)
#define QUIT_HPP

#include "Client.hpp"

void Client_Quit(Client & clt,std::vector<std::string> command,std::map<int,Client> clients);
void DeleteClt(Channel &Chnl,Client clt);

#endif