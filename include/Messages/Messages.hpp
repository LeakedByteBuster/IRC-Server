#if !defined(ERRORS_HPP)
#define ERRORS_HPP

/*
    error number in ERR_NONICKNAMEGIVEN and ERR_ERRONEUSNICKNAME
    should stay hard coded because of switch case in getError()
*/
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

#define FOR_LIST_OF_JOIN_ERRORS(BUILD_JOIN_ERROR) \
    BUILD_JOIN_ERROR(Message::ERR_NOSUCHCHANNEL,    403, \
        getStaticErrorMsg(Message::ERR_NOSUCHCHANNEL)) \
    BUILD_JOIN_ERROR(Message::ERR_TOOMANYCHANNELS,  405, \
        getStaticErrorMsg(Message::ERR_TOOMANYCHANNELS)) \
    BUILD_JOIN_ERROR(Message::ERR_CHANNELISFULL,    471, \
        getStaticErrorMsg(Message::ERR_CHANNELISFULL)) \
    BUILD_JOIN_ERROR(Message::ERR_INVITEONLYCHAN,   473, \
        getStaticErrorMsg(Message::ERR_INVITEONLYCHAN)) \
    BUILD_JOIN_ERROR(Message::ERR_BANNEDFROMCHAN,   474, \
        getStaticErrorMsg(Message::ERR_BANNEDFROMCHAN)) \
    BUILD_JOIN_ERROR(Message::ERR_BADCHANNELKEY,    475, \
        getStaticErrorMsg(Message::ERR_BADCHANNELKEY)) \
    BUILD_JOIN_ERROR(Message::ERR_BADCHANMASK,      476, \
        getStaticErrorMsg(Message::ERR_BADCHANMASK))

#define SERVER_PREFIX    "ircCamel.localhost "
#define SERVER_VERSION    "ircCamel 1.0"
#define ERRORS_SIZE 18

#include <string>
#include <map>
#include "Client.hpp"

class   Message;
class   Channel;
struct  ListOfErrorsNum;

enum    s_types {
    TYPE_USER, // used in newInChannelReply()
    TYPE_SERVER, // used in newInChannelReply()
};

class   Message {

public :

/* -------------------------------------------------------------------------- */
/*                                  Members                                   */
/* -------------------------------------------------------------------------- */
    //  Stores all static error messages
    static std::map<short, std::string> ErrorsDatabase;
/* -------------------------------------------------------------------------- */
/*                                  Methods                                   */
/* -------------------------------------------------------------------------- */
    static std::string  getError(const std::string &clt, short type);
    static std::string  getJoinError(const Channel &ch,
            const Client &clt, short symbol);
    
    static std::string  rplAwayMsg(Client &clt, std::string str);
    // sends replies 
    static std::string  newInChannelReply( const Channel &ch,
        const Client &clt, const std::string command );

    // Sets the map in Message class to the specified static error message
    static void         setErrorsDatabase();
    // Returns the error string stored at index 'type' in ErrorsDatabase map
    static const char   *getStaticErrorMsg(const short type);

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                           Enum Struct Of Errors                            */
/* -------------------------------------------------------------------------- */
    typedef enum    s_errorNumbers {

/* -------------------------------------------------------------------------- */
/*                               General Errors                               */
/* -------------------------------------------------------------------------- */
        ERR_UNKNOWNCOMMAND = 421,
        ERR_NEEDMOREPARAMS = 461,
/* -------------------------------------------------------------------------- */
/*                             Registration Errors                            */
/* -------------------------------------------------------------------------- */
        ERR_NONICKNAMEGIVEN = 431,
        ERR_ERRONEUSNICKNAME = 432,
        ERR_NICKNAMEINUSE = 433,
        ERR_ALREADYREGISTRED = 462,
        ERR_ERRONEUSUSERNAME = 5,
        ERR_INCORRECT_PASS = 3,
/* -------------------------------------------------------------------------- */
/*                            FileTransfert Errors                            */
/* -------------------------------------------------------------------------- */
        ERR_NOSUCHNICK = 401,
        ERR_NOTEXTTOSEND = 412,
        ERR_NOSUCHFILE = 8,
        ERR_NOSUCHFILENAME = 10,
        ERR_NOFILEFROMSENDER = 11,
/* -------------------------------------------------------------------------- */
/*                                 Join Errors                                */
/* -------------------------------------------------------------------------- */
        ERR_NOSUCHCHANNEL = 403,    // "<client> <channel> :No such channel" | ERR_BADCHANMASK is more powerfull 
        ERR_TOOMANYCHANNELS = 405,  // "<client> <channel> :You have joined too many channels"
        ERR_CHANNELISFULL = 471,    // "<client> <channel> :Cannot join channel (+l)"
        ERR_INVITEONLYCHAN = 473,   // "<client> <channel> :Cannot join channel (+i)"
        ERR_BANNEDFROMCHAN = 474,   // "<client> <channel> :Cannot join channel (+b)"
        ERR_BADCHANNELKEY = 475,    // "<client> <channel> :Cannot join channel (+k)"
        ERR_BADCHANMASK = 476,      // "<client> <channel> :Invalid channel name"

        // ERR_TOOMANYTARGETS = 407,
        // ERR_UNAVAILRESOURCE = 15
    } error_t;

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                           Enum Struct Of Replies                           */
/* -------------------------------------------------------------------------- */
    typedef enum    s_replyNumbers {

/* -------------------------------------------------------------------------- */
/*                                Join Replies                                */
/* -------------------------------------------------------------------------- */
        RPL_ENDOFNAMES = 366,   // "<client> <channel> :End of /NAMES list" || S <-   :irc.example.com 366 patty #irctoast :End of /NAMES list.
        RPL_TOPIC = 332,        // "<client> <channel> :<topic>"            || S <-   :irc.example.com 332 alice #test :This is my cool channel! https://irc.com
        RPL_TOPICWHOTIME = 333, // "<client> <channel> <nick> <setat>"      || S <-   :irc.example.com 333 alice #test dan!~d@localhost 1547691506
        RPL_NAMREPLY = 353      // "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}" || S <-   :irc.example.com 353 alice @ #test :alice @dan

    } reply_t;

};

/* -------------------------------------------------------------------------- */
/*                               Function Utils                               */
/* -------------------------------------------------------------------------- */


// :ircCamel.localhost <Error Number> <Client Nickname>
std::string  errorPrefix(const Client &clt, const std::string errNum);
// <nick!~user><@><hostname>
std::string  userPrefix(const Client &clt);
//  <nick!~user><@><hostname>
std::string  replyCommandPrefix(const Channel &ch, const Client &clt, const std::string command, int user_or_server);
// <':'>< ircCamel.localhost / <nick!~user><@><hostname> > <command> <channel name>
// prefix types : TYPE_SERVER, TYPE_USER
std::string commandReply(const Channel &ch, const Client &clt, std::string command, int prefixType);

#endif // ERRORS_HPP
