#if !defined(ERRORS_HPP)
#define ERRORS_HPP

#include <string>
#include "Client.hpp"

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
