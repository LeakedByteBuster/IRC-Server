#include "Server.hpp"

/* -------------------------------------------------------------------------- */
/*                                  static Members                            */
/* -------------------------------------------------------------------------- */
//  Stores all static error messages
std::map<short, std::string>    Message::ErrorsDatabase;

/* -------------------------------------------------------------------------- */
/*                      Getter of static database errors                      */
/* -------------------------------------------------------------------------- */

// Returns the error string stored at index 'type' in ErrorsDatabase map
const char *    Message::getStaticErrorMsg(const short type) {
    return (Message::ErrorsDatabase[type].data());
}

/* -------------------------------------------------------------------------- */
/*                          Error Message Handlers                            */
/* -------------------------------------------------------------------------- */

//  Returns a string that contain the error message :
// :ircCamel.localhost <Error Number> <Client Nickname> :<error message> /// missing command ...
std::string _ERR(const std::string &nick, short type)
{
    std::string error;

    switch (type)
    {
        #define BUILD_ERROR(errorType, errorNum, errorMsg) \
            case Message::errorType: \
                error = static_cast<std::string>(":") \
                        + SERVER_PREFIX \
                        + #errorNum \
                        + " " + nick + " " \
                        + errorMsg; \
                break;

            FOR_LIST_OF_ERRORS(BUILD_ERROR)
        #undef BUILD_ERROR

    default:
        std::cerr << "Warning getError(): Unknown type: " << type << std::endl;
    }

    return error;
}

//  Returns a string that contain the error message
std::string Message::getJoinError(const Channel &ch, const Client &clt, short symbol)
{
    std::string error;

    switch (symbol)
    {
        #define BUILD_JOIN_ERROR(errorType, errorNum, errorMsg) \
            case Message::errorType: \
                error = static_cast<std::string>(":") \
                        + errorPrefix(clt, #errorNum) \
                        + " " +  ch.name + " "; \
                error.append(errorMsg); \
                break;

            FOR_LIST_OF_JOIN_ERRORS(BUILD_JOIN_ERROR)
        #undef BUILD_JOIN_ERROR

    default:
        std::cerr << "Warning getJoinError(): Unknown type: " << symbol << std::endl;
    }

    return error;
}


/* -------------------------------------------------------------------------- */
/*                          Reply Messages Handlers                           */
/* -------------------------------------------------------------------------- */

/*
    returns a string that has :
        JOIN message
        MODE message with the current channel’s modes
        RPL_TOPIC
        RPL_ISUPPORT :
            Format: CASEMAPPING=<casemap>, CHANLIMIT=<prefixes>:[limit]{,<prefixes>:[limit]},
            CHANMODES=A,B,C,D[,X,Y...], CHANNELLEN=<string>, CHANTYPES=[string],
            NICKLEN=<number>, PREFIX=[(modes)prefixes], USERLEN=<number>
*/

std::string Message::getJoinReply(const Channel &ch, const Client &clt)
{
    std::string reply(
        commandReply(ch, clt, "JOIN", TYPE_USER) + "\r\n"
        + commandReply(ch, clt, "MODE", TYPE_SERVER) + " " + ch.getModeString() + "\r\n"
        + ":" + SERVER_PREFIX + "353 " + clt.nickname + " @ " + ch.name + " :" + ch.getClientsInString() +"\r\n"
        + replyPrefix(ch, clt, "366") + " :End of /NAMES list."
    );
    if (ch.isTopic) {
        reply.append("\r\n" + commandReply(ch, clt, "TOPIC", TYPE_USER) + " :" + ch.topic + "\r\n");
    }
    return (reply);
}

std::string  Message::rplAwayMsg(Client &clt,std :: string str)
{
    std :: string msg;

    msg = static_cast<std::string>(":") + SERVER_PREFIX + "301 ";
    msg.append(clt.username + static_cast<std::string>(" "));
    msg.append(str);

    return(msg);
}

std::string Message::getKickReply(const Channel &ch, const Client &clt, std::string reason, std::string target)
{
     std::string reply(
        commandReply(ch, clt, "KICK", TYPE_USER)
        + " " 
        + target
        + reason
        );
    return  (reply);
}

std::string Message::getTopicReply(const Channel &ch, const Client & clt, std::string num , std::string topic)
{
     std::string reply(
        commandReply2(ch, clt, num, topic)
        );
    return  (reply);
}

void    printLog(std::vector<std::string> args)
{
    for (size_t i = 0; i <  args.size(); i++) {
        std::cout  << HRED << "[ DEBUG ] " << args[i] << RESET << std::endl;
    }
    std::cout << std::endl;
} 
