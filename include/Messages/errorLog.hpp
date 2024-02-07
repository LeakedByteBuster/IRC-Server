#if !defined(ERRORLOG_HPP)
#define ERRORLOG_HPP

/*
    TO SET NO STATIC PART OF YOUR MESSAGE : <SERVER_PRIFIX> <symbol from ErrorTypesAndNumbers> .... <static error>:
        1 - Add an error number to the enum struct "ErrorTypesAndNumbers"
        2 - Define it like this below : "#define ERR_BADCHANMASK         Message::BADCHANMASK"
        3 - Add it to the macro FOR_LIST_OF_ERRORS [Don't forget to add '\', but not for last line]
        4 - Add static error message to  "void    Message::setErrorsDatabase()", i.e
            [add symbol to errorNumbers, static string, and set macro ERRORS_ENUM_SIZE]

*/


// shortcut for using Message::getError() method
#define _ERR        Message::getError
// shortcut for using Message::getJoinError() method
#define JOIN_ERR    Message::getJoinError

#define ERR_BADCHANMASK         Message::BADCHANMASK
#define ERR_UNKNOWNCOMMAND      Message::UNKNOWNCOMMAND
#define ERR_NEEDMOREPARAMS      Message::NEEDMOREPARAMS
#define ERR_NONICKNAMEGIVEN     Message::NONICKNAMEGIVEN
#define ERR_ERRONEUSNICKNAME    Message::ERRONEUSNICKNAME
#define ERR_NICKNAMEINUSE       Message::NICKNAMEINUSE
#define ERR_ALREADYREGISTRED    Message::ALREADYREGISTRED
#define ERR_ERRONEUSUSERNAME    Message::ERRONEUSUSERNAME
#define ERR_INCORRECT_PASS      Message::INCORRECT_PASS
#define ERR_NOSUCHNICK          Message::NOSUCHNICK
#define ERR_NOTEXTTOSEND        Message::NOTEXTTOSEND
#define ERR_NOSUCHFILE          Message::NOSUCHFILE
#define ERR_NOSUCHFILENAME      Message::NOSUCHFILENAME
#define ERR_NOFILEFROMSENDER    Message::NOFILEFROMSENDER
#define ERR_NOSUCHCHANNEL       Message::NOSUCHCHANNEL
#define ERR_TOOMANYCHANNELS     Message::TOOMANYCHANNELS
#define ERR_CHANNELISFULL       Message::CHANNELISFULL
#define ERR_INVITEONLYCHAN      Message::INVITEONLYCHAN
#define ERR_BANNEDFROMCHAN      Message::BANNEDFROMCHAN
#define ERR_BADCHANNELKEY       Message::BADCHANNELKEY
#define ERR_CANNOTSENDTOCHAN    Message::CANNOTSENDTOCHAN
#define ERR_INPUTTOOLONG        Message::MSGTOOLONG
// #define rplAwayMsg and call _ERR()

/*
    error number in ERR_NONICKNAMEGIVEN and ERR_ERRONEUSNICKNAME
    should stay hard coded because for switch case in getError()
*/
#define FOR_LIST_OF_ERRORS(BUILD_ERROR) \
    BUILD_ERROR(ERR_NOSUCHNICK,       133, \
        getStaticErrorMsg(ERR_NOSUCHNICK)) \
    BUILD_ERROR(ERR_NOTEXTTOSEND,     412, \
        getStaticErrorMsg(ERR_NOTEXTTOSEND)) \
    BUILD_ERROR(ERR_UNKNOWNCOMMAND,   421, \
        getStaticErrorMsg(ERR_UNKNOWNCOMMAND)) \
    BUILD_ERROR(ERR_NONICKNAMEGIVEN,  431, \
        getStaticErrorMsg(ERR_NONICKNAMEGIVEN)) \
    BUILD_ERROR(ERR_ERRONEUSNICKNAME, 432, \
        getStaticErrorMsg(ERR_ERRONEUSNICKNAME)) \
    BUILD_ERROR(ERR_NICKNAMEINUSE,    433, \
        getStaticErrorMsg(ERR_NICKNAMEINUSE)) \
    BUILD_ERROR(ERR_NEEDMOREPARAMS,   461, \
        getStaticErrorMsg(ERR_NEEDMOREPARAMS)) \
    BUILD_ERROR(ERR_ALREADYREGISTRED, 462, \
        getStaticErrorMsg(ERR_ALREADYREGISTRED)) \
    BUILD_ERROR(ERR_INCORRECT_PASS,   464, \
        getStaticErrorMsg(ERR_INCORRECT_PASS)) \
    BUILD_ERROR(ERR_NOFILEFROMSENDER, 1335, \
        getStaticErrorMsg(ERR_NOFILEFROMSENDER)) \
    BUILD_ERROR(ERR_NOSUCHFILE,       1336, \
        getStaticErrorMsg(ERR_NOSUCHFILE)) \
    BUILD_ERROR(ERR_NOSUCHFILENAME,   1336, \
        getStaticErrorMsg(ERR_NOSUCHFILENAME)) \
    BUILD_ERROR(ERR_CANNOTSENDTOCHAN,    404, \
        getStaticErrorMsg(ERR_CANNOTSENDTOCHAN)) \
    BUILD_ERROR(ERR_INPUTTOOLONG,    417, \
        getStaticErrorMsg(ERR_INPUTTOOLONG))

#define FOR_LIST_OF_JOIN_ERRORS(BUILD_JOIN_ERROR) \
    BUILD_JOIN_ERROR(ERR_NOSUCHCHANNEL,    403, \
        getStaticErrorMsg(ERR_NOSUCHCHANNEL)) \
    BUILD_JOIN_ERROR(ERR_TOOMANYCHANNELS,  405, \
        getStaticErrorMsg(ERR_TOOMANYCHANNELS)) \
    BUILD_JOIN_ERROR(ERR_CHANNELISFULL,    471, \
        getStaticErrorMsg(ERR_CHANNELISFULL)) \
    BUILD_JOIN_ERROR(ERR_INVITEONLYCHAN,   473, \
        getStaticErrorMsg(ERR_INVITEONLYCHAN)) \
    BUILD_JOIN_ERROR(Message::ERR_BANNEDFROMCHAN,   474, \
        getStaticErrorMsg(Message::ERR_BANNEDFROMCHAN)) \
    BUILD_JOIN_ERROR(ERR_BADCHANNELKEY,    475, \
        getStaticErrorMsg(ERR_BADCHANNELKEY)) \
    BUILD_JOIN_ERROR(Message::ERR_BADCHANMASK,      476, \
        getStaticErrorMsg(Message::ERR_BADCHANMASK))

/* -------------------------------------------------------------------------- */
/*                           Enum Struct Of Errors                            */
/* -------------------------------------------------------------------------- */
struct  ErrorTypesAndNumbers {
    typedef enum    s_errorNumbers {

        UNKNOWNCOMMAND = 421,
        NEEDMOREPARAMS = 461,
    /* -------------------------------------------------------------------------- */
    /*                             Registration Errors                            */
    /* -------------------------------------------------------------------------- */
        NONICKNAMEGIVEN = 431,
        ERRONEUSNICKNAME = 432,
        NICKNAMEINUSE = 433,
        ALREADYREGISTRED = 462,
        ERRONEUSUSERNAME = 5,
        INCORRECT_PASS = 3,
    /* -------------------------------------------------------------------------- */
    /*                            FileTransfert Errors                            */
    /* -------------------------------------------------------------------------- */
        NOSUCHFILE = 8,
        NOSUCHFILENAME = 10,
        NOFILEFROMSENDER = 11,
    /* -------------------------------------------------------------------------- */
    /*                           PRIVMSG Errors                            */
    /* -------------------------------------------------------------------------- */
        CANNOTSENDTOCHAN = 404,
        NOSUCHNICK = 401,
        NOTEXTTOSEND = 412,
        MSGTOOLONG = 417,
    /* -------------------------------------------------------------------------- */
    /*                                 Join Errors                                */
    /* -------------------------------------------------------------------------- */
        NOSUCHCHANNEL = 403,    // "<client> <channel> :No such channel" | BADCHANMASK is more powerfull 
        TOOMANYCHANNELS = 405,  // "<client> <channel> :You have joined too many channels"
        CHANNELISFULL = 471,    // "<client> <channel> :Cannot join channel (+l)"
        INVITEONLYCHAN = 473,   // "<client> <channel> :Cannot join channel (+i)"
        BANNEDFROMCHAN = 474,   // "<client> <channel> :Cannot join channel (+b)"
        BADCHANNELKEY = 475,    // "<client> <channel> :Cannot join channel (+k)"
        BADCHANMASK = 476,      // "<client> <channel> :Invalid channel name"

        // ERR_ERR_TOOMANYTARGETS = 407,
        // ERR_ERR_UNAVAILRESOURCE = 15

    } error_t;
} ;

#endif // ERRORLOG_HPP
