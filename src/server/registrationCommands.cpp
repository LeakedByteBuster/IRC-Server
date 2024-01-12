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

void    parseNick(std::map<int, Client> &clients, Client &clt, std::string str)
{
    /*
        <nick>       ::= <letter> { <letter> | <number> | <special> }
        <letter>     ::= 'a' ... 'z' | 'A' ... 'Z'
        <number>     ::= '0' ... '9'
        <special>    ::= '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}'
    */
    std::cout << "parseNick(): " << str << std::endl;

    std::vector<std::string>    tokens = splitBySpace(str);
    if (tokens.size() != 2) {
        Server::sendMsg(clt, LogError::nickErrors(clt.nickname, LogError::INVALID_NICKNAME));
        throw std::invalid_argument("");
    }

    if (tokens[0].compare("NICK") != 0) {
        Server::sendMsg(clt, LogError::nickErrors(str, LogError::INVALID_NICKNAME));
        throw std::invalid_argument("");
    }
    //  checks if first character is a letter
    str = tokens[1];
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

    // set nickname
    std::map<int, Client>::iterator it = clients.begin();
    for (; it != clients.end(); it++) {
        if (str.compare(it->second.nickname) == 0) {
            Server::sendMsg(clt, LogError::nickErrors(str, LogError::ERR_NICKNAMEINUSE));
            throw std::invalid_argument("");
        }
    }

    clt.nickname = str;
    Server::sendMsg(clt, LogError::passErrors(clt.nickname, LogError::CORRECT_PASS));
}

void    parseUser(Client &, std::string str)
{
    std::cout << "parsePass(): " << str << std::endl;

    // client sent all the information correctly, sending welcome message
}