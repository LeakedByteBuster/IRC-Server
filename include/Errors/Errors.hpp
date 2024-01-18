#if !defined(ERRORS_HPP)
#define ERRORS_HPP

#include <string>
#include "Client.hpp"




// //password 
// #define ERR_PASSWDMISMATCH ":Password incorrect"
#define ERR_NEEDMOREPARAMS ":Not enough parameters"
// #define ERR_ALREADYREGISTERED ":You may not reregister"

// //PRVMSG

#define ERR_NORECIPIENT ":No recipient given"
#define ERR_NOTEXTTOSEND ":No text to send"


/****************************   PASS    ***********************************/
/* PASS_NOT_SUPLLIED, INCORRECT_PASS

    S <-   :irc.example.com 464 chris :Password Incorrect
    S <-   ERROR :Closing Link: localhost (Bad Password)
    <connection gets terminated by the server>
*/
/* REGISTERED_SUCCESS

    S <-   :irc.example.com 001 <nick> :Welcome to the ExampleNet Internet Relay Chat Network <nick>
*/

/****************************   NICK    ***********************************/
// NOTES:
//  Silently truncated nickname during, and after registration

/* nick-in-use

    C  ->  NICK ben
    C  ->  USER b 0 * :Benno!
    S <-   :irc.example.com 433 * ben :Nickname is already in use.
    C  ->  NICK ben-
    S <-   :irc.example.com 001 ben- :Welcome to the ExampleNet Internet Relay Chat Network ben-
*/
/* Invalid nickname

    C  ->  nick 345gman!
    S <-   :irc.example.com 432 george 345gman! :Erroneous Nickname
*/
/* Series of nickname changes seen by another user on the network :

    S <-   :ben-!~b@localhost NICK :dan
    S <-   :dan!~b@localhost NICK :danny
    S <-   :danny!~b@localhost NICK :george
*/

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
        REGISTERED_SUCCESS,
        ERR_NOSUCHFILE,
        ERR_NOSUCHNICK,
        ERR_NOSUCHFILENAME,
        ERR_NOFILEFROMSENDER
    };

    static std::string  getError(const std::string &clt, short type);
    static std::string  registrationSuccess(const std::string &nick);

};

#endif // ERRORS_HPP
