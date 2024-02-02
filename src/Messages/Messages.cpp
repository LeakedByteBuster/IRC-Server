#include "Messages.hpp"
#include "Server.hpp"

std::string Message::postRegistration(const std::string &nick)
{
    std::string str;

    str = IRC_NAME + static_cast<std::string>("001 * ");
    str.append(
        ":Welcome to the Camel Internet Relay Chat Network " + nick
    );

    return (str);
}

std::string  Message :: rplAwayMsg(Client &clt,std :: string str)
{
    std :: string msg;

    msg = IRC_NAME + static_cast<std::string>("301 ");
    msg.append(clt.username + static_cast<std::string>(" "));
    msg.append(str);
    
    return(msg);
}

#define FOR_LIST_OF_ERRORS(BUILD_MESSAGE) \
    BUILD_MESSAGE(Message::INCORRECT_PASS, 464 * , : Password Incorrect)           \
    BUILD_MESSAGE(Message::ERR_NONICKNAMEGIVEN, 431 * , : No nickname given)       \
    BUILD_MESSAGE(Message::ERR_ERRONEUSNICKNAME, 432 * , : Erroneous nickname)     \
    BUILD_MESSAGE(Message::ERR_ERRONEUSUSERNAME, 432 * , : Erroneous username)     \
    BUILD_MESSAGE(Message::ERR_NICKNAMEINUSE, 433 * , : Nickname is already in use)\
    BUILD_MESSAGE(Message::ERR_NEEDMOREPARAM, 461 * , : Not enough parameters)     \
    BUILD_MESSAGE(Message::ERR_ALREADYREGISTRED, 462 * , : You may not reregister) \
    BUILD_MESSAGE(Message::ERR_UNKNOWNCOMMAND, 421 * , : Unknown command)          \
    BUILD_MESSAGE(Message::ERR_NOSUCHFILE, 1336 * , : No such a file in /DIR)      \
    BUILD_MESSAGE(Message::ERR_NOSUCHNICK, 133 * , : No such nick/channel)         \
    BUILD_MESSAGE(Message::ERR_NOSUCHFILENAME, 1336 * , : /file name not found)    \
    BUILD_MESSAGE(Message::ERR_NOFILEFROMSENDER, 1335 * , : No file from sender)   \
    BUILD_MESSAGE(Message::ERR_NOTEXTTOSEND, 412 * , : No text to send)            \
    BUILD_MESSAGE(Message::ERR_CANNOTSENDTOCHAN, 404 * , : Cannot send to channel)

std::string Message::getError(const std::string &, short type)
{
    std::string error;

    switch (type)
    {
        #define BUILD_MESSAGE(errorType, errorNum, errorMsg) \
            case Message::errorType: \
                error = IRC_NAME + static_cast<std::string>(#errorNum); \
                error.append(#errorMsg); \
                break;

            FOR_LIST_OF_ERRORS(BUILD_MESSAGE)
        #undef BUILD_MESSAGE

    default:
        std::cerr << "Warning getError(): Unknown type: " << type << std::endl;
    }

    return error;
}
