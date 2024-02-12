#include "Server.hpp"

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

// <':'><ircCamel.localhost> <symbol> <client Nickname> <channel name>
std::string replyPrefix(const Channel &ch, const Client &clt, const std::string symbol)
{
    std::string buff(":");
    buff.append( SERVER_PREFIX + symbol + " " + clt.nickname + " " + ch.name );
    return (buff);
}

// nick!~user@hostname
std::string userPrefix(const Client &clt)
{
    std::string str(":");

    str = clt.nickname + "!";
    str.append(clt.username + "@");
    str.append(inet_ntoa(clt.hints.sin_addr));
    return (str);
}

// <':'>< ircCamel.localhost / <nick!~user><@><hostname>> <command> <channel name>
// prefix types : TYPE_SERVER, TYPE_USER
std::string commandReply(const Channel &ch, const Client &clt, std::string command, int prefixType)
{
    std::string rpl(":"); 
    rpl.append( (prefixType == TYPE_SERVER) ? SERVER_PREFIX : userPrefix(clt) + " ");
    rpl.append(command + " " + ch.name );

    return (rpl);
}

// SERVER_PREFIX num clt.nickname ch.name :Coolest topic
std::string commandReply2(const Channel &ch, const Client &clt, std::string num, std::string  msg)
{
    std::string rpl(":"); 
    rpl.append(SERVER_PREFIX + num);
    rpl.append(+ " " + clt.nickname + " " + ch.name);
    rpl.append( + " :" + msg );

    return (rpl);
}

std::string commandReply3(const Channel &ch, const Client &clt, std::string command, int prefixType,std::string topic)
{
    std::string rpl(":"); 
    rpl.append( (prefixType == TYPE_SERVER) ? SERVER_PREFIX : userPrefix(clt) + " ");
    rpl.append(command + " " + ch.name );
    rpl.append(" :" + topic);
    return (rpl);
}
std::string commandReply4(const Channel &ch, const Client &clt, std::string command, int prefixType,std::string target)
{
    std::string rpl(":"); 
    rpl.append( (prefixType == TYPE_SERVER) ? SERVER_PREFIX : userPrefix(clt) + " ");
    rpl.append(command + " " + target );
    rpl.append(" :" + ch.name);
    return (rpl);
}
std::string commandReply5(const Channel &ch, const Client &clt, int prefixType,std::string target)
{
    std::string rpl(":"); 
    rpl.append( (prefixType == TYPE_SERVER) ? SERVER_PREFIX : userPrefix(clt) + " ");
    rpl.append("341 " + clt.nickname + " " + target );
    rpl.append(" " + ch.name);
    return (rpl);
}
/* -------------------------------------------------------------------------- */
/*                       Static Error Messages Database                       */
/* -------------------------------------------------------------------------- */

#define ERRORS_ENUM_SIZE             25 // used in setErrorsDatabase()

// Sets the map in Message class to the specified static error message
void    Message::setErrorsDatabase()
{
    Message::error_t    errorNumbers[ERRORS_ENUM_SIZE] = {
        ERR_INCORRECT_PASS,     // 1
        ERR_NONICKNAMEGIVEN,    // 2
        ERR_ERRONEUSNICKNAME,   // 3
        ERR_ERRONEUSUSERNAME,   // 4
        ERR_NICKNAMEINUSE,      // 5
        ERR_NEEDMOREPARAMS,     // 6
        ERR_ALREADYREGISTRED,   // 7
        ERR_UNKNOWNCOMMAND,     // 8
        ERR_NOSUCHFILE,         // 9
        ERR_NOSUCHNICK,         // 10
        ERR_NOSUCHFILENAME,     // 11
        ERR_NOFILEFROMSENDER,   // 12
        ERR_NOTEXTTOSEND,       // 13
        ERR_NOSUCHCHANNEL,      // 14
        ERR_TOOMANYCHANNELS,    // 15
        ERR_CHANNELISFULL,      // 16
        ERR_INVITEONLYCHAN,     // 17
        ERR_BADCHANMASK,        // 18
        ERR_CHANOPRIVSNEEDED,   // 19
        ERR_USERNOTINCHANNEL,   // 20
        ERR_NOTONCHANNEL ,       // 21
        ERR_CANNOTSENDTOCHAN,   // 22
        ERR_BADCHANNELKEY,       // 23
        ERR_INPUTTOOLONG,         // 24
        ERR_NOTONCHANNEL,         //25

        /* NOTICE: Change ERRORS_ENUM_SIZE macro in Messages.hpp to the current size */
    };

    const char * staticErrorsString[] = {
          ":Password Incorrect"                 // 1
        , ":No nickname given"                  // 2
        , ":Erroneous nickname"                 // 3
        , ":Erroneous username"                 // 4
        , ":Nickname is already in use"         // 4
        , ":Not enough parameters."              // 6
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
        , ":Invalid channel name"               // 18
        , ":You're not channel operator"        // 19 
        , ":They aren't on that channel"        // 20
        , ":You're not on that channel"         // 21
        , ":Cannot send to channel"             // 22
        , ":Cannot join channel (+k) - bad key" // 23
        , ":Input line was too long"            // 24
        ,":You're not on that channel"          //25
        /* NOTICE: Change ERRORS_ENUM_SIZE macro in Messages.hpp to the current size */
    };
    
    for (int i = 0; i < ERRORS_ENUM_SIZE; i++) {
        ErrorsDatabase[errorNumbers[i]] = staticErrorsString[i];
    }

}
