#if !defined(ERRORS_HPP)
#define ERRORS_HPP

#define SERVER_PREFIX           "ircCamel.localhost "
#define SERVER_VERSION          "ircCamel 1.0"

#define MAX_CHANNEL_NAME_LEN    200
#define MAX_CHANNELS_PER_USER   10

#include <string>
#include <map>
#include "Client.hpp"
#include "errorLog.hpp"
#include "replyLog.hpp"

class   Message;
class   Channel;

enum    s_types {
    TYPE_USER, // used in getJoinReply()
    TYPE_SERVER, // used in getJoinReply()
};

class   Message :  public ErrorTypesAndNumbers, public RepliesTypesAndNumbers {

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
    // sends replies  366, 353, JOIN reply and MODE reply
    static std::string  getJoinReply( const Channel &ch, const Client &clt);

    // sends KICK reply
    static std::string getKickReply(const Channel &ch, const Client & clt, std::string reason, std::string target);
    // send KICKED reply
    static std::string getKickedReply(const Channel &ch, const Client &clt, std::string target);
    // Sets the map in Message class to the specified static error message
    static void         setErrorsDatabase();
    // Returns the error string stored at index 'type' in ErrorsDatabase map
    static const char   *getStaticErrorMsg(const short type);



} ;

/* -------------------------------------------------------------------------- */
/*                               Function Utils                               */
/* -------------------------------------------------------------------------- */


// :ircCamel.localhost <Error Number> <Client Nickname>
std::string  errorPrefix(const Client &clt, const std::string errNum);
// <nick!~user><@><hostname>
std::string  userPrefix(const Client &clt);
// <':'>< ircCamel.localhost / <nick!~user><@><hostname> > <command> <channel name>
// prefix types : TYPE_SERVER, TYPE_USER
std::string commandReply(const Channel &ch, const Client &clt, std::string command, int prefixType);
// <':'><ircCamel.localhost> <symbol> <client Nickname> <channel name>
std::string replyPrefix(const Channel &ch, const Client &clt, const std::string symbol);
// std::string commandReply2( <sender> , <reciever> , <message>) 
// it's same of replyPrefix but change channle with reciever
std::string privmsgReply(const Client &sender , const Client &recv ,  const std::string &msg);

#endif // ERRORS_HPP
