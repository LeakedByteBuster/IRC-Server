#if !defined(ERRORS_HPP)
#define ERRORS_HPP

#include <string>
#include "Client.hpp"

#define IRC_NAME    "ircCamel.localhost "
#define SERVER_VERSION    "ircCamel 1.0"

class   Message {
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
        ERR_NOSUCHFILE,
        ERR_NOSUCHNICK,
        ERR_NOSUCHFILENAME,
        ERR_NOFILEFROMSENDER,
        ERR_NOTEXTTOSEND,
        ERR_CANNOTSENDTOCHAN
    };

    static std::string  getError(const std::string &clt, short type);
    static std::string  rplAwayMsg(Client &clt, std::string str);

};

#endif // ERRORS_HPP
