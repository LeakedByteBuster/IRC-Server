#if !defined(ERRORS_HPP)
#define ERRORS_HPP

#include <string>
#include "Client.hpp"




//password 
#define ERR_PASSWDMISMATCH ":Password incorrect"
#define ERR_NEEDMOREPARAMS ":Not enough parameters"
#define ERR_ALREADYREGISTERED ":You may not reregister"

// nick_name

#define ERR_NONICKNAMEGIVEN ":No nickname given"
#define ERR_ERRONEUSNICKNAME ":Erroneus nickname"
#define ERR_NICKNAMEINUSE ":Nickname is already in use"


//PRVMSG

#define ERR_NOSUCHNICK ":No such nick/channel"
#define ERR_NORECIPIENT ":No recipient given"
#define ERR_NOTEXTTOSEND ":No text to send"
#define ERR_NOSUCHNICK " :No such nick/channel"

#define IRC_NAME    ":ircCamel.localhost "

class   LogError {
public :
    enum    PASS {
        /*
            S <-   :irc.example.com 464 chris :Password Incorrect
            S <-   ERROR :Closing Link: localhost (Bad Password)
            <connection gets terminated by the server>
        */
        PASS_NOT_SUPLLIED,
        INCORRECT_PASS,

        /*
            S <-   :irc.example.com 001 <nick> :Welcome to the ExampleNet Internet Relay Chat Network <nick>
        */
       CORRECT_PASS

    };

    static std::string    passErrors(const std::string &clt, short type);

};

#endif // ERRORS_HPP