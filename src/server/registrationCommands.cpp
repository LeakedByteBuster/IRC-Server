#include <iostream>
#include <sstream>
#include "registrationCommands.hpp"
#include "Errors.hpp"
#include "Server.hpp"


void    parsePass(Client &clt, std::string str, const std::string &pass)
{
    std::cout << "parsePass(): " << str << std::endl;

    std::stringstream           ss(str);
    std::vector<std::string>    tokens;
    std::string                 token;

    while (ss >> token) {
        tokens.push_back(token);
    }
    if (tokens.size() != 2) {
        Server::sendMsg(clt, LogError::passErrors(clt.nickname, LogError::PASS_NOT_SUPLLIED));
        return ;
    }
    if (tokens[0].compare("PASS") != 0) {
        Server::sendMsg(clt, LogError::passErrors(clt.nickname, LogError::PASS_NOT_SUPLLIED));
        return ;
    }
    if (tokens[1].compare(pass) != 0) {
        Server::sendMsg(clt, LogError::passErrors(clt.nickname, LogError::INCORRECT_PASS));
        return ;
    }
    // client sent all the information correctly, sending welcome message
    Server::sendMsg(clt, LogError::passErrors(clt.nickname, LogError::CORRECT_PASS));
}
/*
   <nick>       ::= <letter> { <letter> | <number> | <special> }
   <letter>     ::= 'a' ... 'z' | 'A' ... 'Z'
   <number>     ::= '0' ... '9'
   <special>    ::= '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}'
*/
void    parseNick(Client &, std::string str)
{
        std::cout << "parseNick(): " << str << std::endl;
    
}

void    parseUser(Client &, std::string str)
{
        std::cout << "parsePass(): " << str << std::endl;
}