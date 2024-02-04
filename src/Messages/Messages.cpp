#include "Messages.hpp"
#include "Server.hpp"

/* -------------------------------------------------------------------------- */
/*                                  static Members                            */
/* -------------------------------------------------------------------------- */
//  Stores all static error messages
std::map<short, std::string>    Message::ErrorsDatabase;

/* -------------------------------------------------------------------------- */
/*                              Messages Prefix                               */
/* -------------------------------------------------------------------------- */

// <':'><ircCamel.localhost> <Error Number> <Client Nickname>
std::string errorPrefix(const Client &clt, const std::string errNum)
{
    std::string buff(":");
    buff = SERVER_PREFIX + errNum + " " + clt.nickname;
    return (buff);
}

// nick!~user@hostname
std::string userPrefix(const Client &clt)
{
    std::string str(":");

    str = clt.nickname + " " + "!";
    str.append(clt.username + "@");
    str.append(inet_ntoa(clt.hints.sin_addr));
    return (str);
}

//  <nick!~user@hostname> <command> <channel name>
std::string replyCommandPrefix(const Channel &ch, const Client &clt, const std::string command, int user_or_server)
{
    std::string prefix = (user_or_server == TYPE_SERVER) ? SERVER_PREFIX : userPrefix(clt);

    std::string replyPrfx(
        prefix  +
        " "     +
        command +
        ch.name
    );
    return (replyPrfx);
}


/* -------------------------------------------------------------------------- */
/*                             Gets Static Error                              */
/* -------------------------------------------------------------------------- */

// Returns the error string stored at index 'type' in ErrorsDatabase map
const char *getStaticErrorMsg(const short type) {
    return (Message::ErrorsDatabase[type].data());
}

/* -------------------------------------------------------------------------- */
/*                           Append Messages                                  */
/* -------------------------------------------------------------------------- */

std::string  Message :: rplAwayMsg(Client &clt,std :: string str)
{
    std :: string msg;

    msg = static_cast<std::string>(":") + SERVER_PREFIX + "301 ";
    msg.append(clt.username + static_cast<std::string>(" "));
    msg.append(str);

    return(msg);
}

/* -------------------------------------------------------------------------- */
/*                          Error Messages Handlers                           */
/* -------------------------------------------------------------------------- */

// std::string nameReply(const Channel &ch, const Client &clt, const std::string command)
// {
//     std::vector<std::string>    users;
//     std::string                 reply;
//     std::string                 tmp;
//     size_t                      i = 0;
    
//     users.insert(ch.ClientsInChannel.begin(), ch.ClientsInChannel.end());
//     do {
//         tmp.append(users[i]);

//     } while (i < users.size())

//     for (size_t i = 0; ; i++) {
//         if (i != 0)
//     }

    
//     reply = errorPrefix(clt, Message::RPL_NAMREPLY) + " @ " + ch.name; // "@" because all channel are public
//     return (reply);
// }

//  Returns a string that contain the error message :
// :ircCamel.localhost <Error Number> <Client Nickname> :<error message>
std::string Message::getError(const std::string &nick, short type)
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

/* -------------------------------------------------------------------------- */
/*                          Reply Messages Handlers                           */
/* -------------------------------------------------------------------------- */

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
        std::cerr << "Warning getError(): Unknown type: " << symbol << std::endl;
    }

    return error;
}
