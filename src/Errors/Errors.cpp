#include "Errors.hpp"
#include "Server.hpp"

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

std::string  LogError :: rplAwayMsg(Client &clt,std :: string str)
{
    std :: string msg;

    msg = IRC_NAME + static_cast<std::string>("301 ");
    msg.append(clt.username + static_cast<std::string>(" "));
    msg.append(str);
    
    return(msg);
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
        error.append(":No nickname given");
        break ;

    case LogError::ERR_ERRONEUSNICKNAME :
        error = IRC_NAME + static_cast<std::string>("432 * ");
        error.append("Erroneous nickname");
        break;

    case LogError::ERR_ERRONEUSUSERNAME :
        error = IRC_NAME + static_cast<std::string>("432 * ");
        error.append(":Erroneous username");
        break;

    case LogError::ERR_NICKNAMEINUSE :
        error = IRC_NAME + static_cast<std::string>("433 * ");
        error.append(":Nickname is already in use");
        break ;

    case LogError::ERR_NEEDMOREPARAM :
        error = IRC_NAME + static_cast<std::string>("461 * ");
        error.append(":Not enough parameters");
        break ;

    case LogError::ERR_ALREADYREGISTRED :
        error = IRC_NAME + static_cast<std::string>("462 * ");
        error.append(":You may not reregister");
        break ;

    case LogError::ERR_UNKNOWNCOMMAND :
        error = IRC_NAME + static_cast<std::string>("421 * ");
        error.append("Unknown command");
        break ;
    
     case LogError::ERR_NOSUCHFILE :
        error = IRC_NAME + static_cast<std::string>("1336");
        error.append(nick + static_cast<std::string>(" "));
        error.append(":No such a file in /DIR");
        break ;

    case LogError::ERR_NOSUCHNICK :
        error = IRC_NAME + static_cast<std::string>("133");
        error.append(nick + static_cast<std::string>(" "));
        error.append(":No such nick/channel");
        break ;

    case LogError::ERR_NOSUCHFILENAME :
        error = IRC_NAME + static_cast<std::string>("1336");
        error.append(nick + static_cast<std::string>(" "));
        error.append(": /file name not found");
        break ;
    
    case LogError::ERR_NOFILEFROMSENDER :
        error = IRC_NAME + static_cast<std::string>("1335");
        error.append(nick + static_cast<std::string>(" "));
        error.append(": No file from sender");
        break ;

    case LogError::ERR_NOTEXTTOSEND :
        error = IRC_NAME + static_cast<std::string>("412");
        error.append(nick + static_cast<std::string>(" "));
        error.append(":No text to send");
        break ;

    case LogError::ERR_CANNOTSENDTOCHAN :
        error = IRC_NAME + static_cast<std::string>("404");
        error.append(nick + static_cast<std::string>(" "));
        error.append(":Cannot send to channel");
        break ;
    
    default:
        std::cerr << "Warning getError(): Unknown type : " << type << std::endl;
    }

    return (error);
}
