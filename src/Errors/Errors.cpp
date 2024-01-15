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

    case CORRECT_PASS :
    /*
    S <-   :irc.example.com 001 <nick> :Welcome to the ExampleNet Internet Relay Chat Network <nick>
    */
        str = IRC_NAME + static_cast<std::string>("001 ");
        str.append(nick + static_cast<std::string>(" "));
        str.append(
            ":Welcome to the Camel Internet Relay Chat Network " + nick
        );
        break;

    case ERR_ALREADYREGISTRED :
        str = IRC_NAME + static_cast<std::string>("462 ");
        str.append(nick + static_cast<std::string>(" "));
        str.append(
            ":You may not reregister " + nick
        );
        break ;

    default:
        std::cerr << "Warning : Unknown type " << type << std::endl;
    }

    return (str);
}

std::string    LogError::nickErrors(const std::string &nick, short type)
{
    std::string error;
    
    switch (type)
    {
    case LogError::INVALID_NICKNAME :
        /*  S <-   :irc.example.com 432 george 345gman! :Erroneous Nickname */
        error = IRC_NAME + static_cast<std::string>("432 ");
        error.append(nick + static_cast<std::string>(" "));
        error.append(
            ":Erroneous Nickname"
        );
        break;
    
    case LogError::ERR_NICKNAMEINUSE :
    /* S <-   :irc.example.com 433 * ben :Nickname is already in use */
        error = IRC_NAME + static_cast<std::string>("433 * ");
        error.append(nick + static_cast<std::string>(" "));
        error.append(
            ":Nickname is already in use"
        );
        break ;

    default:
        break;
    }
    return error;
}