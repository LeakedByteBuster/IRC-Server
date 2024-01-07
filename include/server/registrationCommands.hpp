#if !defined(REGISTRATIONCOMMANDS_HPP)
#define REGISTRATIONCOMMANDS_HPP

#include <string>
// #include "Client.hpp"
class Client;

void    parsePass(Client &, std::string str, const std::string &pass);
void    parseNick(Client &client, std::string str);
void    parseUser(Client &client, std::string str);

#endif // REGISTRATIONCOMMANDS_HPP
