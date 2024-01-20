#if !defined(ERRORS_HPP)
#define ERRORS_HPP

#include <string>
#include "Client.hpp"

#define ERR_NEEDMOREPARAMS ":Not enough parameters"
#define ERR_NORECIPIENT ":No recipient given"
#define ERR_NOTEXTTOSEND ":No text to send"
#define ERR_NOSUCHNICK " :No such nick/channel"

#define IRC_NAME    ":ircCamel.localhost "

class   LogError {
public :
    enum    ErrorsTypes {
        
        INCORRECT_PASS,
        ERR_ERRONEUSNICKNAME,
        ERR_ERRONEUSUSERNAME,
        ERR_NICKNAMEINUSE,
        ERR_NONICKNAMEGIVEN,
        ERR_NEEDMOREPARAM,
        ERR_ALREADYREGISTRED,
        ERR_UNKNOWNCOMMAND,
        REGISTERED_SUCCESS
    };

    static std::string  getError(const std::string &clt, short type);
    static std::string  registrationSuccess(const std::string &nick);

};

#endif // ERRORS_HPP
