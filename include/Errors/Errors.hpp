#if !defined(ERRORS_HPP)
#define ERRORS_HPP

#include <string>
#include "Client.hpp"

#define IRC_NAME    ":ircCamel.localhost "

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
class   LogError {
public :
    enum    PASS {

        PASS_NOT_SUPLLIED,
        INCORRECT_PASS,

       INVALID_NICKNAME,
       ERR_NICKNAMEINUSE,
       ERR_ALREADYREGISTRED, 

       REGISTERED_SUCCESS
    };

    static std::string    passErrors(const std::string &clt, short type);
    static std::string    nickErrors(const std::string &clt, short type);

};

#endif // ERRORS_HPP
