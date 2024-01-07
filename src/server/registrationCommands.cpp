#include <iostream>
#include <sstream>
#include "registrationCommands.hpp"


void    parsePass(Client &, std::string str, const std::string &)
{
    std::cout << "PASS : " << str << std::endl;

    std::stringstream   ss(str);
    std::string         token;
    ss >> token;
    if (token.compare("PASS") != 0) {
        throw std::invalid_argument("Password Incorrect");
    }
}

void    parseNick(Client &, std::string str)
{
        std::cout << "NICK : " << str << std::endl;
    
}

void    parseUser(Client &, std::string str)
{
        std::cout << "USER : " << str << std::endl;
}