#if !defined(ERRORS_HPP)
#define ERRORS_HPP

#include <string>
#include <map>
#include "Client.hpp"

#define IRC_NAME    "ircCamel.localhost "
#define SERVER_VERSION    "ircCamel 1.0"

class   Message {
public :
    static std::map<short, std::string> ErrorsDatabase;
    static void setErrorsDatabase();

    enum    GeneralErrors {
        ERR_UNKNOWNCOMMAND = 0,
        ERR_NEEDMOREPARAMS = 1
    } ;

    enum    RegistrationErrors {

        ERR_ALREADYREGISTRED = 2,
        ERR_INCORRECT_PASS = 3,
        ERR_ERRONEUSNICKNAME = 4,
        ERR_ERRONEUSUSERNAME = 5,
        ERR_NICKNAMEINUSE = 6,
        ERR_NONICKNAMEGIVEN = 7,
        
        // ERR_CANNOTSENDTOCHAN
    };

    enum    FileTransfertErros {
        ERR_NOSUCHFILE = 8,
        ERR_NOSUCHNICK = 9,
        ERR_NOSUCHFILENAME = 10,
        ERR_NOFILEFROMSENDER = 11,
        ERR_NOTEXTTOSEND = 12
    } ;

    enum    JoinReplies {
        ERR_BADCHANMASK = 13,
        // ERR_TOOMANYTARGETS = 14,        
        // ERR_UNAVAILRESOURCE = 15


        // RPL_TOPIC (332)
            // S <-   :irc.example.com 332 alice #test :This is my cool channel! https://irc.com
            // 
        // ERR_TOOMANYCHANNELS (405)
        // ERR_NEEDMOREPARAMS (461)
        // ERR_CHANNELISFULL (471)
        // ERR_INVITEONLYCHAN (473)
        // ERR_BANNEDFROMCHAN (474)
        // ERR_BADCHANNELKEY (475)

// ERR_NOSUCHCHANNEL (403) // ERR_BADCHANMASK is more powerfull
//// RPL_TOPICTIME (333)
//// RPL_NAMREPLY (353)
    } ;

    static std::string  getError(const std::string &clt, short type);
    static std::string  rplAwayMsg(Client &clt, std::string str);

};

/* [<nick> '!' <user> ] ['@' <host> ] : nick!~user@hostname */
std::string getId(const Client &clt);
/* :ircCamel.localhost <Error Number> <Client Nickname> */
std::string getSource(const Client &clt, const std::string errNum);


#endif // ERRORS_HPP
