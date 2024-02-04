#include "Messages.hpp"
#include "Server.hpp"

/* -------------------------------------------------------------------------- */
/*                                  static Members                            */
/* -------------------------------------------------------------------------- */
//  Stores all static error messages
std::map<short, std::string>    Message::ErrorsDatabase;

/* -------------------------------------------------------------------------- */
/*                           Append To From Message                           */
/* -------------------------------------------------------------------------- */

/* <':'><ircCamel.localhost> <Error Number> <Client Nickname> */
std::string getMessageFirstPart(const Client &clt, const std::string errNum)
{
    std::string buff(":");

    buff = IRC_NAME + errNum + " " + clt.nickname;

    return (buff);
}

/* Returns a string of the form nick!~user@hostname */
std::string getId(const Client &clt)
{
    std::string str(":");

    str = clt.nickname + " " + "!";
    str.append(clt.username + "@");
    str.append(inet_ntoa(clt.hints.sin_addr));

    return (str);
}

std::string  Message :: rplAwayMsg(Client &clt,std :: string str)
{
    std :: string msg;

    msg = static_cast<std::string>(":") + IRC_NAME + "301 ";
    msg.append(clt.username + static_cast<std::string>(" "));
    msg.append(str);

    return(msg);
}


// Returns the error string stored at index 'type' in ErrorsDatabase map
const char *getStaticErrorMsg(const short type) {
    return (Message::ErrorsDatabase[type].data());
}

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
                        + IRC_NAME \
                        + #errorNum \
                        + " " + nick + " "; \
                error.append(errorMsg); \
                break;

            FOR_LIST_OF_ERRORS(BUILD_ERROR)
        #undef BUILD_ERROR

    default:
        std::cerr << "Warning getError(): Unknown type: " << type << std::endl;
    }

    return error;
}

//  Returns a string that contain the error message
std::string Message::getJoinError(const Channel &ch, const Client &clt, short type)
{
    std::string error;

    switch (type)
    {
        #define BUILD_JOIN_ERROR(errorType, errorNum, errorMsg) \
            case Message::errorType: \
                error = static_cast<std::string>(":") \
                        + getMessageFirstPart(clt, #errorNum) \
                        + " " +  ch.name + " "; \
                error.append(errorMsg); \
                break;

            FOR_LIST_OF_JOIN_ERRORS(BUILD_JOIN_ERROR)
        #undef BUILD_JOIN_ERROR

    default:
        std::cerr << "Warning getError(): Unknown type: " << type << std::endl;
    }

    return error;
}

// std::string Message::getJoinReply(const Channel &ch, const Client &clt, short type)
// {
//     std::string rpl;

//     return(rpl);
// }

// Sets the map in Message class to the specified error msg
// [ <ircCamel.localhost> <errNum> <nick> ] should be already set
void    Message::setErrorsDatabase()
{
    Message::ListOfErrorsNum    errorNumbers[ERRORS_SIZE] = {
        Message::ERR_INCORRECT_PASS,    // 1
        Message::ERR_NONICKNAMEGIVEN,   // 2
        Message::ERR_ERRONEUSNICKNAME,  // 3
        Message::ERR_ERRONEUSUSERNAME,  // 4
        Message::ERR_NICKNAMEINUSE,     // 5
        Message::ERR_NEEDMOREPARAMS,    // 6
        Message::ERR_ALREADYREGISTRED,  // 7
        Message::ERR_UNKNOWNCOMMAND,    // 8
        Message::ERR_NOSUCHFILE,        // 9
        Message::ERR_NOSUCHNICK,        // 10
        Message::ERR_NOSUCHFILENAME,    // 11
        Message::ERR_NOFILEFROMSENDER,  // 12
        Message::ERR_NOTEXTTOSEND,      // 13
        Message::ERR_NOSUCHCHANNEL,     // 14
        Message::ERR_TOOMANYCHANNELS,   // 15
        Message::ERR_CHANNELISFULL,     // 16
        Message::ERR_INVITEONLYCHAN,    // 17
        Message::ERR_BADCHANMASK        // 18
        /* NOTICE: Change ERRORS_SIZE macro in Messages.hpp to the current size */
    };

    const char * staticErrorsString[] = {
          ":Password Incorrect"                 // 1
        , ":No nickname given"                  // 2
        , ":Erroneous nickname"                 // 3
        , ":Erroneous username"                 // 4
        , ":Nickname is already in use"         // 4
        , ":Not enough parameters"              // 6
        , ":You may not reregister"             // 7
        , ":Unknown command"                    // 8
        , ":No such a file in /DIR"             // 9
        , ":No such nick/channel"               // 10
        , ":/file name not found"               // 11
        , ":No file from sender"                // 12
        , ":No text to send"                    // 13
        , ":No such channel"                    // 14
        , ":You have joined too many channels"  // 15
        , ":Cannot join channel (+l)"           // 16
        , ":Cannot join channel (+i)"           // 17
        , ":Bad Channel Mask"                   // 18
        /* NOTICE: Change ERRORS_SIZE macro in Messages.hpp to the current size */
    };
    
    for (int i = 0; i < ERRORS_SIZE; i++) {
        ErrorsDatabase[errorNumbers[i]] = staticErrorsString[i];
    }

}

