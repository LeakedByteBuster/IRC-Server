#include "Messages.hpp"
#include "Server.hpp"

//  Stores all static error messages
std::map<short, std::string>    Message::ErrorsDatabase;

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

// Returns the string at index 'type' given in argument
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
// [ <ircCamel.localhost> <errNum> <nick> ] are already set
void    Message::setErrorsDatabase()
{
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_INCORRECT_PASS, ":Password Incorrect")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_NONICKNAMEGIVEN, ":No nickname given")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_ERRONEUSNICKNAME, ":Erroneous nickname")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_ERRONEUSUSERNAME, ":Erroneous username")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_NICKNAMEINUSE, ":Nickname is already in use")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_NEEDMOREPARAMS, ":Not enough parameters")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_ALREADYREGISTRED, ":You may not reregister")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_UNKNOWNCOMMAND, ":Unknown command")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_NOSUCHFILE, ":No such a file in /DIR")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_NOSUCHNICK, ":No such nick/channel")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_NOSUCHFILENAME, ":/file name not found")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_NOFILEFROMSENDER, ":No file from sender")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_NOTEXTTOSEND, ":No text to send")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_NOSUCHCHANNEL, ":No such channel")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_TOOMANYCHANNELS, ":You have joined too many channels")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_CHANNELISFULL, ":Cannot join channel (+l)")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_INVITEONLYCHAN, ":Cannot join channel (+i)")
    );
    ErrorsDatabase.insert(
        std::make_pair(Message::ERR_BADCHANMASK, ":Bad Channel Mask")
    );
}

