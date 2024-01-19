
// #include "Server.hpp"
#include <string>
#include "Errors.hpp"


static  std::string getPassError (const std::string &nick) 
{
    std::string str;

    str = IRC_NAME + static_cast<std::string>("464 ");
    str.append(nick + static_cast<std::string>(" "));
    str.append (
        ":Password Incorrect\n"
    );
    return (str);
}

std::string LogError::registrationSuccess(const std::string &nick)
{
    /* REGISTERED_SUCCESS

    S <-   :irc.example.com 001 <nick> :Welcome to the ExampleNet Internet Relay Chat Network <nick>
    */
    std::string str;

    str = IRC_NAME + static_cast<std::string>("001 ");
    str.append(nick + static_cast<std::string>(" "));
    str.append(
        ":Welcome to the Camel Internet Relay Chat Network " + nick
    );

    return (str);
}

std::string    LogError::getError(const std::string &nick, short type)
{
    std::string error;

    switch (type)
    {
    case LogError::INCORRECT_PASS :
        return (getPassError(nick));

    case LogError::ERR_NONICKNAMEGIVEN:
        error = IRC_NAME + static_cast<std::string>("431 * ");
        error.append(nick + static_cast<std::string>(" "));
        error.append(":No nickname given");
        break ;

    case LogError::ERR_ERRONEUSNICKNAME :
        error = IRC_NAME + static_cast<std::string>("432 ");
        error.append(nick + static_cast<std::string>(" "));
        error.append(":Erroneous nickname");
        break;

    case LogError::ERR_ERRONEUSUSERNAME :
        error = IRC_NAME + static_cast<std::string>("432 ");
        error.append(nick + static_cast<std::string>(" "));
        error.append(":Erroneous username");
        break;

    case LogError::ERR_NICKNAMEINUSE :
        error = IRC_NAME + static_cast<std::string>("433 * ");
        error.append(nick + static_cast<std::string>(" "));
        error.append(":Nickname is already in use");
        break ;

    case LogError::ERR_NEEDMOREPARAM :
        error = IRC_NAME + static_cast<std::string>("461 * ");
        error.append(nick + static_cast<std::string>(" "));
        error.append(":Not enough parameters");
        break ;

    case LogError::ERR_ALREADYREGISTRED :
        error = IRC_NAME + static_cast<std::string>("462 ");
        error.append(nick + static_cast<std::string>(" "));
        error.append(":You may not reregister " + nick);
        break ;

    default:
        std::cerr << "Warning passErrors: Unknown type : " << type << std::endl;
    }

    return (error);
}
