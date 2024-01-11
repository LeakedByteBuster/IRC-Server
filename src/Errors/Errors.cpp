#include "Errors.hpp"

static  std::string getPassError (const std::string &nick) 
{
    std::string str;

    str = IRC_NAME + static_cast<std::string>("464 ");
    str.append(nick + static_cast<std::string>(" "));
    str.append (
        ":Password Incorrect\n"
        "ERROR :Closing Link: localhost (Bad Password)"
    );
    return (str);
}

std::string    LogError::passErrors(const std::string &nick, short type)
{
    std::string str;

    switch (type)
    {
        /*
            S <-   :irc.example.com 464 <NICK> :Password Incorrect
            S <-   ERROR :Closing Link: localhost (Bad Password)
            <connection gets terminated by the server>
        */
    case PASS_NOT_SUPLLIED :
        return (getPassError(nick));
    
    case INCORRECT_PASS :
        return (getPassError(nick));

        /*
            S <-   :irc.example.com 001 <nick> :Welcome to the ExampleNet Internet Relay Chat Network <nick>
        */
    case CORRECT_PASS :
        str = IRC_NAME + static_cast<std::string>("001 ");
        str.append(nick + static_cast<std::string>(" "));
        str.append(
            ":Welcome to the Camel Internet Relay Chat Network" + nick
        );
        break;

    default:
        std::cerr << "Warning : Unknown type " << type << std::endl;
    }

    return (str);
}
