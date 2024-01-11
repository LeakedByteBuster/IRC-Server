#include <iostream>
#include <sstream>
#include <vector>
#include "registrationCommands.hpp"
#include "Client.hpp"
#include "Errors.hpp"


void    parsePass(Client &clt, std::string str, const std::string &pass)
{
    std::cout << "PASS : '" << str << "'" << std::endl;

    std::stringstream           ss(str);
    std::vector<std::string>    tokens;
    std::string                 token;
    unsigned long               i = 0;
    while (ss >> token) {
        tokens.push_back(token);
    }

    /*
        THE SENDMSG() FUNCTION SHOULD BE STATIC
    */
    if (tokens[0].compare("PASS") != 0) {
        clt.sendMsg(clt, LogError::passErrors(clt.nickname, LogError::PASS_NOT_SUPLLIED));
        return ;
    }
    
    for (; i < tokens.size(); i++)
        ;
    if (!tokens[i-1].empty() && tokens[i-1].compare(pass) != 0) {
        clt.sendMsg(clt, LogError::passErrors(clt.nickname, LogError::INCORRECT_PASS));
        return ;
    }

    clt.sendMsg(clt, LogError::passErrors(clt.nickname, LogError::CORRECT_PASS));
}

void    parseNick(Client &, std::string str)
{
        std::cout << "NICK : " << str << std::endl;
    
}

void    parseUser(Client &, std::string str)
{
        std::cout << "USER : " << str << std::endl;
}