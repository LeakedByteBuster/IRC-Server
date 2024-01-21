
// #include "Server.hpp"
#include <string>
#include "Message.hpp"


std::string LogError::registrationSuccess(const std::string &nick)
{
    std::string str;

    str = IRC_NAME + static_cast<std::string>("001 * ");
    str.append(
        ":Welcome to the Camel Internet Relay Chat Network " + nick
    );

    return (str);
}
// Server::sendMsg(clt, getError(nick, ERR_...))
// std::string    LogError::getError(const std::string &nick, short type)
// {


std::string  LogError :: rplAwayMsg(Client &clt,std :: string str)
{
    std :: string msg;

    msg = IRC_NAME + static_cast<std::string>("301 ");
    msg.append(clt.username + static_cast<std::string>(" "));
    msg.append(str);
    
    return(msg);
}
// }
#define FOR_LIST_OF_ERRORS(BUILD_MESSAGE) \
    BUILD_MESSAGE(LogError::INCORRECT_PASS, 464 * , : Password Incorrect)           \
    BUILD_MESSAGE(LogError::ERR_NONICKNAMEGIVEN, 431 * , : No nickname given)       \
    BUILD_MESSAGE(LogError::ERR_ERRONEUSNICKNAME, 432 * , : Erroneous nickname)     \
    BUILD_MESSAGE(LogError::ERR_ERRONEUSUSERNAME, 432 * , : Erroneous username)     \
    BUILD_MESSAGE(LogError::ERR_NICKNAMEINUSE, 433 * , : Nickname is already in use)\
    BUILD_MESSAGE(LogError::ERR_NEEDMOREPARAM, 461 * , : Not enough parameters)     \
    BUILD_MESSAGE(LogError::ERR_ALREADYREGISTRED, 462 * , : You may not reregister) \
    BUILD_MESSAGE(LogError::ERR_UNKNOWNCOMMAND, 421 * , : Unknown command)          \
    BUILD_MESSAGE(LogError::ERR_NOSUCHFILE, 1336 * , : No such a file in /DIR)      \
    BUILD_MESSAGE(LogError::ERR_NOSUCHNICK, 133 * , : No such nick/channel)         \
    BUILD_MESSAGE(LogError::ERR_NOSUCHFILENAME, 1336 * , : /file name not found)    \
    BUILD_MESSAGE(LogError::ERR_NOFILEFROMSENDER, 1335 * , : No file from sender)   \
    BUILD_MESSAGE(LogError::ERR_NOTEXTTOSEND, 412 * , : No text to send)            \
    BUILD_MESSAGE(LogError::ERR_CANNOTSENDTOCHAN, 404 * , : Cannot send to channel) \
    BUILD_MESSAGE(LogError::ERR_BADCHANNELKEY, 475 *  , :Cannot join channel (+k) - bad key) \
    BUILD_MESSAGE(LogError::ERR_BADCHANMASK, 476 *  , :Bad Channel Mask) \
    BUILD_MESSAGE(LogError::ERR_NEEDMOREPARAMS, 461 *  ,: Not enough parameters)


std::string LogError::getError(const std::string &, short type)
{
    std::string error;

    switch (type)
    {
        #define BUILD_MESSAGE(errorType, errorNum, errorMsg) \
            case LogError::errorType: \
                error = IRC_NAME + static_cast<std::string>(#errorNum); \
                error.append(#errorMsg); \
                break;

            FOR_LIST_OF_ERRORS(BUILD_MESSAGE);
        #undef BUILD_MESSAGE

    default:
        std::cerr << "Warning getError(): Unknown type: " << type << std::endl;
    }

    return error;
}
// std::string LogError::getError(const std::string &, short type)
// {
//     std::string error;

//     switch (type)
//     {
//         #define BUILD_MESSAGE(errorType, errorNum, errorMsg) \
//             case LogError::errorType: \
//                 error = IRC_NAME + static_cast<std::string>(#errorNum); \
//                 error.append(#errorMsg); \
//                 break;

//             FOR_LIST_OF_ERRORS(BUILD_MESSAGE)
//         #undef BUILD_MESSAGE

//     case LogError::ERR_BADCHANMASK :
//         error = IRC_NAME + static_cast<std::string>("475 ");
//         error.append(nick + static_cast<std::string>(" "));
//         error.append(":Cannot join channel (+k) - bad key" + nick);
//         break ;
//     default:
//         std::cerr << "Warning getError(): Unknown type: " << type << std::endl;
//     }

//     return error;
// }
