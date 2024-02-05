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
// :ircCamel.localhost <Error Number> <Client Nickname> :<error message>
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
std::string JOIN_ERR(const Channel &ch, const Client &clt, short symbol)
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
        RPL_TOPIC and RPL_TOPICTIME numerics if the channel has a topic set (if the topic is not set, the user is sent no numerics).
        one or more RPL_NAMREPLY
*/
std::string Message::newInChannelReply(const Channel &, const Client &, const std::string )
{
    // std::map<int, Client>       users;
    // std::string                 clients;
    // std::stringstream           ss;
    
    // // build up the message to sent
    // ss << Message::RPL_NAMREPLY;
    // std::string symbol = ss.str().c_str();

    std::string reply;
    //     commandReply()
    // ); // "@" channel is public

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