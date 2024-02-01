
#include "Server.hpp"
#include <string>
#include "Message.hpp"


std::string Message::registrationSuccess(const std::string &nick)
{
    std::string str;

    str = IRC_NAME + static_cast<std::string>(" 001 ")+nick+" :Welcome " + nick +  " to the ft_irc network !\r\n" \
    + IRC_NAME + static_cast<std::string>(" 002 ")+nick+ " :Your host is ircCamel.localhost"  +  " to the ft_irc network !\r\n" \
    + IRC_NAME + static_cast<std::string>(" 003 ")+nick+ " :This server was created 2023-9-15 !\r\n"\
    + IRC_NAME + static_cast<std::string>(" 004 ")+nick+ " :Host: " + "ircCamel.localhost"+ ", Version: 1.0, User mode: none, Channel modes: o, t, k, i !\r\n";
    // str.append ()

    return (str);
}
    // str = IRC_NAME + static_cast<std::string>("002 ")+nick+ +" :Your host is " +IRC_NAME + " running version 1.0 !\r\n";
    // str.append(
    //     ":Welcome to the Camel Internet Relay Chat Network " + nick
    // );
// Server::sendMsg(clt, getError(nick, ERR_...))
// std::string    Message::getError(const std::string &nick, short type)
// {

void clientHost(Client sender,std::string str,Client recv)
{
    std::string msg = sender.nickname + " PRVMSG " + recv.nickname;
    msg.append(" " + str);
    Server::sendMsg(recv,msg);
    Message::rplAwayMsg(sender,msg); 
}

std::string  Message :: rplAwayMsg(Client &clt,std :: string str)
{
    std :: string msg;

    msg = IRC_NAME + static_cast<std::string>("301 ");
    msg.append(clt.username + static_cast<std::string>(" "));
    msg.append(str);
    
    return(msg);
}
// }
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
    BUILD_MESSAGE(Message::ERR_NOSUCHNICK, 401 * , : No such nick/channel)         \
    BUILD_MESSAGE(Message::ERR_NOSUCHFILENAME, 1336 * , : /file name not found)    \
    BUILD_MESSAGE(Message::ERR_NOFILEFROMSENDER, 1335 * , : No file from sender)   \
    BUILD_MESSAGE(Message::ERR_NOTEXTTOSEND, 412 * , : No text to send)            \
    BUILD_MESSAGE(Message::ERR_CANNOTSENDTOCHAN, 404 * , : Cannot send to channel) \
    BUILD_MESSAGE(Message::ERR_INVITEONLYCHAN, 473 *  , :Cannot join channel (+i) - invite only) \
    BUILD_MESSAGE(Message::ERR_BADCHANNELKEY, 475 *  , :Cannot join channel (+k) - bad key) \
    BUILD_MESSAGE(Message::ERR_BADCHANMASK, 476 *  , : Bad Channel Mask) \
    BUILD_MESSAGE(Message::ERR_NEEDMOREPARAMS, 461 *  ,: Not enough parameters) \
    BUILD_MESSAGE(Message::ERR_USERONCHANNEL, 443 *  ,: is already on channel) \
    BUILD_MESSAGE(Message::ERR_NOSUCHCHANNEL, 403 *  ,: No such channel) \
    BUILD_MESSAGE(Message::ERR_USERNOTINCHANNEL, 403 *  ,: They aren t on that channel)\
    BUILD_MESSAGE(Message::ERR_NOTONCHANNEL, 442 *  ,:  :You re not on that channel)\
    BUILD_MESSAGE(Message::ERR_CHANOPRIVSNEEDED, 403 *  ,: :You re not channel operator)
    // BUILD_MESSAGE(Message::ERR_CHANOPRIVSNEEDED, 482 *  ,: You're not channel operator)


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

            FOR_LIST_OF_ERRORS(BUILD_MESSAGE);
        #undef BUILD_MESSAGE

    default:
        std::cerr << "Warning getError(): Unknown type: " << type << std::endl;
    }

    return error;
}
// std::string Message::getError(const std::string &, short type)
// {
//     std::string error;

//     switch (type)
//     {
//         #define BUILD_MESSAGE(errorType, errorNum, errorMsg) \
//             case Message::errorType: \
//                 error = IRC_NAME + static_cast<std::string>(#errorNum); \
//                 error.append(#errorMsg); \
//                 break;

//             FOR_LIST_OF_ERRORS(BUILD_MESSAGE)
//         #undef BUILD_MESSAGE

//     case Message::ERR_BADCHANMASK :
//         error = IRC_NAME + static_cast<std::string>("475 ");
//         error.append(nick + static_cast<std::string>(" "));
//         error.append(":Cannot join channel (+k) - bad key" + nick);
//         break ;
//     default:
//         std::cerr << "Warning getError(): Unknown type: " << type << std::endl;
//     }

//     return error;
// }
