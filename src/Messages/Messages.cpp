#include "Messages.hpp"
#include "Server.hpp"

//  Stores all static error messages
std::map<short, std::string>    Message::ErrorsDatabase;

// Sets the map in Message class to the specified error msg
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
}

/* <':'><ircCamel.localhost> <Error Number> <Client Nickname> */
std::string getSource(const Client &clt, const std::string errNum)
{
    std::string buff(":");

    buff = IRC_NAME + errNum; + " " + clt.nickname;

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

#define FOR_LIST_OF_ERRORS(BUILD_ERROR) \
    BUILD_ERROR(Message::ERR_NOSUCHNICK,       133, \
        getStaticErrorMsg(Message::ERR_NOSUCHNICK)) \
    BUILD_ERROR(Message::ERR_NOTEXTTOSEND,     412, \
        getStaticErrorMsg(Message::ERR_NOTEXTTOSEND)) \
    BUILD_ERROR(Message::ERR_UNKNOWNCOMMAND,   421, \
        getStaticErrorMsg(Message::ERR_UNKNOWNCOMMAND)) \
    BUILD_ERROR(Message::ERR_NONICKNAMEGIVEN,  431, \
        getStaticErrorMsg(Message::ERR_NONICKNAMEGIVEN)) \
    BUILD_ERROR(Message::ERR_ERRONEUSNICKNAME, 432, \
        getStaticErrorMsg(Message::ERR_ERRONEUSNICKNAME)) \
    BUILD_ERROR(Message::ERR_ERRONEUSUSERNAME, 432, \
        getStaticErrorMsg(Message::ERR_ERRONEUSUSERNAME)) \
    BUILD_ERROR(Message::ERR_NICKNAMEINUSE,    433, \
        getStaticErrorMsg(Message::ERR_NICKNAMEINUSE)) \
    BUILD_ERROR(Message::ERR_NEEDMOREPARAMS,   461, \
        getStaticErrorMsg(Message::ERR_NEEDMOREPARAMS)) \
    BUILD_ERROR(Message::ERR_ALREADYREGISTRED, 462, \
        getStaticErrorMsg(Message::ERR_ALREADYREGISTRED)) \
    BUILD_ERROR(Message::ERR_INCORRECT_PASS,   464, \
        getStaticErrorMsg(Message::ERR_INCORRECT_PASS)) \
    BUILD_ERROR(Message::ERR_NOFILEFROMSENDER, 1335, \
        getStaticErrorMsg(Message::ERR_NOFILEFROMSENDER)) \
    BUILD_ERROR(Message::ERR_NOSUCHFILE,       1336, \
        getStaticErrorMsg(Message::ERR_NOSUCHFILE)) \
    BUILD_ERROR(Message::ERR_NOSUCHFILENAME,   1336, \
        getStaticErrorMsg(Message::ERR_NOSUCHFILENAME))
    // BUILD_ERROR(Message::ERR_CANNOTSENDTOCHAN, 404 , : Cannot send to channel)

//  Returns a string that contain the error message
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
