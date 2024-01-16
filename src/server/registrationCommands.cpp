#include <iostream>
#include <sstream>
#include "registrationCommands.hpp"
#include "Errors.hpp"
#include "Server.hpp"

static std::vector<std::string> splitBySpace(std::string str) {
    std::stringstream           ss(str);
    std::vector<std::string>    tokens;
    std::string                 token;

    while (ss >> token) {
        tokens.push_back(token);
    }
    return (tokens);
}

void    parsePass(Client &clt, std::string str, const std::string &pass)
{
    std::cout << "parsePass(): " << str << std::endl;

    std::vector<std::string>    tokens = splitBySpace(str);

    if (tokens.size() != 2) {
        Server::sendMsg(clt, LogError::passErrors(clt.nickname, LogError::PASS_NOT_SUPLLIED));
        throw std::invalid_argument("");
    }
    // if (clt.isPass && tokens[0].compare("PASS") == 0) {
    //     Server::sendMsg(clt, LogError::passErrors(clt.nickname, LogError::ERR_ALREADYREGISTRED));
    //     throw std::invalid_argument("");
    // }
    if (tokens[0].compare("PASS") != 0) {
        Server::sendMsg(clt, LogError::passErrors(clt.nickname, LogError::PASS_NOT_SUPLLIED));
        throw std::invalid_argument("");
    }
    if (tokens[1].compare(pass) != 0) {
        Server::sendMsg(clt, LogError::passErrors(clt.nickname, LogError::INCORRECT_PASS));
        throw std::invalid_argument("");
    }

    // clt.isPass = 1;
}

/*
    <nick>       ::= <letter> { <letter> | <number> | <special> }
    <letter>     ::= 'a' ... 'z' | 'A' ... 'Z'
    <number>     ::= '0' ... '9'
    <special>    ::= '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}'
*/
void    parseNick(std::map<int, Client> &clients, Client &clt, std::string str)
{
    #if defined(LOG)
        std::cout << "parseNick(): " << str << std::endl;
    #endif  // LOG

    // checks number of parameters and command
    std::vector<std::string>    tokens = splitBySpace(str);
    if (tokens.size() != 2) {
        Server::sendMsg(clt, LogError::nickErrors(clt.nickname, LogError::INVALID_NICKNAME));
        throw std::invalid_argument("");
    }
    str = tokens[1];
    //  check command
    if (tokens[0].compare("NICK") != 0) {
        Server::sendMsg(clt, LogError::nickErrors(str, LogError::INVALID_NICKNAME));
        throw std::invalid_argument("");
    }
    //  checks if first character is a letter
    if (!isalpha(str[0])) {
        Server::sendMsg(clt, LogError::nickErrors(str, LogError::INVALID_NICKNAME));
        throw std::invalid_argument("");
    }
    //  checks if rest of characters is valid
    std::string  special = "-[]\\`^{}";
    for (unsigned long i = 1; i < str.size(); i++) {
        if (isalnum(str[i])) {
            continue ;
        } else if (special.find(str[i]) == std::string::npos) {
            Server::sendMsg(clt, LogError::nickErrors(str, LogError::INVALID_NICKNAME));
            throw std::invalid_argument("");
        }
    }
    //  checks if nickname is already in use
    std::map<int, Client>::iterator it = clients.begin();
    for (; it != clients.end(); it++) {
        if (str.compare(it->second.nickname) == 0) {
            Server::sendMsg(clt, LogError::nickErrors(str, LogError::ERR_NICKNAMEINUSE));
            throw std::invalid_argument("");
        }
    }
    // truncates nickname
    if (str.length() > 9)
        str = str.substr(0, 9);
    // set nickname
    clt.nickname = str;
    Server::sendMsg(clt, LogError::passErrors(clt.nickname, LogError::REGISTERED_SUCCESS));
}

/*
    <user>       ::= <nonwhite> { <nonwhite> }
    <nonwhite>   ::= <any 8bit code except SPACE (0x20), NUL (0x0), CR
                        (0xd), and LF (0xa)>
*/
void    parseUser(Client &, std::string str)
{

    std::cout << "parsePass(): " << str << std::endl;

    // client sent all the information correctly, sending welcome message
}