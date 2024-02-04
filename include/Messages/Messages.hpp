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

#define IRC_NAME    "ircCamel.localhost "
#define SERVER_VERSION    "ircCamel 1.0"
#define ERRORS_SIZE 18

#include <string>
#include <map>
#include "Client.hpp"

class   Message;
class   Channel;
struct  ListOfErrorsNum;

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
    static void         setErrorsDatabase();
    static std::string  getError(const std::string &clt, short type);
    static std::string  rplAwayMsg(Client &clt, std::string str);
    static std::string  getJoinError(const Channel &ch,
            const Client &clt, short type);

    typedef enum    ListOfErrorsNum {
/* -------------------------------------------------------------------------- */
/*                               GeneralErrors                                */
/* -------------------------------------------------------------------------- */
        ERR_UNKNOWNCOMMAND = 421,
        ERR_NEEDMOREPARAMS = 461,
/* -------------------------------------------------------------------------- */
/*                             RegistrationErrors                             */
/* -------------------------------------------------------------------------- */
        ERR_NONICKNAMEGIVEN = 431,
        ERR_ERRONEUSNICKNAME = 432,
        ERR_NICKNAMEINUSE = 433,
        ERR_ALREADYREGISTRED = 462,
        ERR_ERRONEUSUSERNAME = 5,
        ERR_INCORRECT_PASS = 3,
/* -------------------------------------------------------------------------- */
/*                            FileTransfertErrors                             */
/* -------------------------------------------------------------------------- */
        ERR_NOSUCHNICK = 401,
        ERR_NOTEXTTOSEND = 412,
        ERR_NOSUCHFILE = 8,
        ERR_NOSUCHFILENAME = 10,
        ERR_NOFILEFROMSENDER = 11,
/* -------------------------------------------------------------------------- */
/*                                 JoinErrors                                 */
/* -------------------------------------------------------------------------- */
        ERR_NOSUCHCHANNEL = 403, //   "<client> <channel> :No such channel" | ERR_BADCHANMASK is more powerfull 
        ERR_TOOMANYCHANNELS = 405, //   "<client> <channel> :You have joined too many channels"
        ERR_CHANNELISFULL = 471, //   "<client> <channel> :Cannot join channel (+l)"
        ERR_INVITEONLYCHAN = 473, //   "<client> <channel> :Cannot join channel (+i)"
        ERR_BANNEDFROMCHAN = 474, //   "<client> <channel> :Cannot join channel (+b)"
        ERR_BADCHANNELKEY = 475, //   "<client> <channel> :Cannot join channel (+k)"
        ERR_BADCHANMASK = 476, //    "<channel> :Bad Channel Mask" // Invalid channel name
/* -------------------------------------------------------------------------- */
/*                                JoinReplies                                 */
/* -------------------------------------------------------------------------- */
        RPL_ENDOFNAMES = 366, // "<client> <channel> :End of /NAMES list" // S <-   :irc.example.com 366 patty #irctoast :End of /NAMES list.
        RPL_TOPIC = 332, // "<client> <channel> :<topic>"
        RPL_TOPICWHOTIME = 333, // "<client> <channel> <nick> <setat>"
        RPL_NAMREPLY = 353 // "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"

        // ERR_TOOMANYTARGETS = 407,
        // ERR_UNAVAILRESOURCE = 15
    } ListOfErrors_t;

};

/* -------------------------------------------------------------------------- */
/*                               Function Utils                               */
/* -------------------------------------------------------------------------- */

/* [<nick> '!' <user> ] ['@' <host> ] : nick!~user@hostname */
std::string getId(const Client &clt);
/* :ircCamel.localhost <Error Number> <Client Nickname> */
std::string getMessageFirstPart(const Client &clt, const std::string errNum);
// Returns the error string stored at index 'type' in ErrorsDatabase map
const char *getStaticErrorMsg(const short type);

#endif // ERRORS_HPP
