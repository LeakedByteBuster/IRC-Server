#if !defined(REGISTRATIONCOMMANDS_HPP)
#define REGISTRATIONCOMMANDS_HPP

#include <string>
#include <map>
// #include "Client.hpp"
class Client;

void    parsePass(Client &clt, std::string str, const std::string &pass);
void    parseNick(std::map<int, Client> &clients, Client &client, std::string str);
void    parseUser(Client &client, std::string str);

#endif // REGISTRATIONCOMMANDS_HPP
