#if !defined(ERRORS_HPP)
#define ERRORS_HPP

#include <string>
#include "Client.hpp"

// #define ERR_NEEDMOREPARAMS ":Not enough parameters"
#define ERR_NORECIPIENT ":No recipient given"
// #define ERR_NOTEXTTOSEND ":No text to send"

#define IRC_NAME    ":ircCamel.localhost "

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
        REGISTERED_SUCCESS,
        ERR_NOSUCHFILE,
        ERR_NOSUCHNICK,
        ERR_NOSUCHFILENAME,
        ERR_NOFILEFROMSENDER,
        ERR_NOTEXTTOSEND,
        ERR_CANNOTSENDTOCHAN,
        ERR_BADCHANNELKEY,
        ERR_BADCHANMASK,
        ERR_NEEDMOREPARAMS,
        ERR_INVITEONLYCHAN,
        ERR_CHANOPRIVSNEEDED
    };

    static std::string  getError(const std::string &clt, short type);
    static std::string  registrationSuccess(const std::string &nick);
    static std::string  rplAwayMsg(Client &clt,std :: string str);

};

#endif // ERRORS_HPP
