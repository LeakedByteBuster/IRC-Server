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
#define ERR_NOSUCHNICK " :No such nick/channel"

// //errors...
// #define ERR_NOSUCHCHANNEL 	":No such channel"
// #define ERR_TOOMANYCHANNELS ":You have joined too many channels"
// #define ERR_BADCHANNELKEY 	":Cannot join channel (+k)"
// #define ERR_BANNEDFROMCHAN 	":Cannot join channel (+b)"
// #define ERR_CHANNELISFULL   ":Cannot join channel (+l)"
// #define ERR_INVITEONLYCHAN 	":Cannot join channel (+i)"
// #define ERR_BADCHANMASK 	":Bad Channel Mask"
// #define RPL_TOPIC 			"TOPIC"
// #define RPL_TOPICWHOTIME 	"TOPICTIME "
// #define RPL_NAMREPLY		": NAMERPLY"
// #define RPL_ENDOFNAMES	    ":End of /NAMES list "


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
        ERR_NOSUCHCHANNEL,
        ERR_TOOMANYCHANNELS,
        ERR_BADCHANNELKEY,	
        ERR_BANNEDFROMCHAN,
        ERR_CHANNELISFULL,
        ERR_INVITEONLYCHAN,
        ERR_BADCHANMASK ,	
        RPL_TOPIC,
        RPL_TOPICWHOTIME,		
        RPL_NAMREPLY,		
        RPL_ENDOFNAMES,
        REGISTERED_SUCCESS,
    };

    static std::string  getError(const std::string &clt, short type);
    static std::string  registrationSuccess(const std::string &nick);

};

#endif // ERRORS_HPP
