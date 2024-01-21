#include <iostream>
#include <sstream>
#include "registrationCommands.hpp"
#include "Message.hpp"
#include "Server.hpp"

void    parsePass(Client &clt, std::string str, const std::string &pass)
{    
    #if defined(DEBUG)
        std::cout << "parsePass(): " << str << std::endl;
    #endif  // DEBUG

    std::vector<std::string>    tokens = splitBySpace(str);
    // Checks if pass command is already given
    if (clt.isRegistred == 1) {
        Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::ERR_ALREADYREGISTRED));
        throw std::invalid_argument("");
    }
    if (tokens.size() != 2) {
        Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::ERR_NEEDMOREPARAM));
        throw std::invalid_argument("");
    }
    if (tokens[0].compare("PASS") != 0) {
        Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::ERR_NEEDMOREPARAM));
        throw std::invalid_argument("");
    }
    if (tokens[1].compare(pass) != 0) {
        Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::INCORRECT_PASS));
        throw std::invalid_argument("");
    }
}

/*
    <nick>       ::= <letter> { <letter> | <number> | <special> }
    <letter>     ::= 'a' ... 'z' | 'A' ... 'Z'
    <number>     ::= '0' ... '9'
    <special>    ::= '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}'
*/
void    parseNick(std::map<int, Client> &clients, Client &clt, std::string str)
{
    std::map<int, Client>::iterator it;

    #if defined(DEBUG)
        std::cout << "parseNick(): " << str << std::endl;
    #endif  // DEBUG

    // checks number of parameters and command
    std::vector<std::string>    tokens = splitBySpace(str);
    if (tokens.size() != 2) {
        Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::ERR_NONICKNAMEGIVEN));
        throw std::invalid_argument("");
    }
    //  Assigning given nick to str
    str = tokens[1];
    //  check command
    if (tokens[0].compare("NICK") != 0) {
        Server::sendMsg(clt, LogError::getError(str, LogError::ERR_ERRONEUSNICKNAME));
        throw std::invalid_argument("");
    }
    //  checks if first character is a letter
    if (!isalpha(str[0])) {
        Server::sendMsg(clt, LogError::getError(str, LogError::ERR_ERRONEUSNICKNAME));
        throw std::invalid_argument("");
    }
    //  checks if rest of characters is valid
    std::string  special = "-[]\\`^{}";
    for (unsigned long i = 1; i < str.size(); i++) {
        if (isalnum(str[i])) {
            continue ;
        } else if (special.find(str[i]) == std::string::npos) {
            Server::sendMsg(clt, LogError::getError(str, LogError::ERR_ERRONEUSNICKNAME));
            throw std::invalid_argument("");
        }
    }
    //  checks if nickname is already in use
    it = clients.begin();
    for (; it != clients.end(); it++) {
        if (str.compare(it->second.nickname) == 0) {
            Server::sendMsg(clt, LogError::getError(str, LogError::ERR_NICKNAMEINUSE));
            throw std::invalid_argument("");
        }
    }
    // truncates nickname
    if (str.length() > 9)
        str = str.substr(0, 9);
    // set nickname
    clt.nickname = str;
}

/*
    USER <username> 0 * <realname>

    <user>       ::= <nonwhite> { <nonwhite> }
    <nonwhite>   ::= <any 8bit code except SPACE (0x20), NUL (0x0), CR
                        (0xd), and LF (0xa)>
*/
void    parseUser(Client &clt, std::string str)
{
    #if defined(DEBUG)
        std::cout << "parseUser(): " << str << std::endl;
    #endif  // DEBUG

    std::vector<std::string> tokens = splitBySpace(str);
    
    // checks if parameter number is > 5
    if (tokens.size() < 5) {
        Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::ERR_NEEDMOREPARAM));
        throw std::invalid_argument("");
    }
    // checks USER
    if (tokens[0].compare("USER") != 0) {
        Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::ERR_NEEDMOREPARAM));
        throw std::invalid_argument("");
    }
    // checks if username has some invalid characters
    std::string  special = " \n\0\r";
    for (unsigned long i = 0; i < tokens[1].size(); i++) {
        if (special.find(tokens[1][i]) == std::string::npos) {
            continue ;
        } else {
            Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::ERR_ERRONEUSUSERNAME));
            throw std::invalid_argument("");
        }
    }
    // checks '0' and '*'
    if (tokens[2].compare("0") || tokens[3].compare("*")) {
        Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::ERR_ERRONEUSUSERNAME));
        throw std::invalid_argument("");
    }
    //  looks for first character in realname
    std::string rname = str.substr(str.find('*') + 1, str.size());
    unsigned long   i = 0;
    for (; i < rname.size(); i++) {
        if (isascii(rname[i]) && rname[i] != ' ')
            break ;
    }
    //  checks if a realname is given
    if (i == rname.size()) {
        Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::ERR_ERRONEUSUSERNAME));
        throw std::invalid_argument("");
    }
    rname = rname.substr(i, rname.size());
    //  set username and realname for the client
    if (tokens[1].size() > 9)
        str = tokens[1].substr(0, 9);
    if (rname.size() > 9)
        rname = rname.substr(0, 15);
    clt.username = tokens[1];
    clt.realname = rname;
    // client sent all the information correctly, sending welcome message
    Server::sendMsg(clt, LogError::registrationSuccess(clt.nickname));
}
