#if !defined(REPLYLOG_HPP)
#define REPLYLOG_HPP

/* -------------------------------------------------------------------------- */
/*                           Enum Struct Of Replies                           */
/* -------------------------------------------------------------------------- */
struct  RepliesTypesAndNumbers {
    typedef enum    s_replyNumbers {

    /* -------------------------------------------------------------------------- */
    /*                                Join Replies                                */
    /* -------------------------------------------------------------------------- */
        // || S <-   :irc.example.com 366 patty #irctoast :End of /NAMES list.
        RPL_ENDOFNAMES = 366,   // "<client> <channel> :End of /NAMES list" 
        // || S <-   :irc.example.com 332 alice #test :This is my cool channel! https://irc.com
        RPL_TOPIC = 332,        // "<client> <channel> :<topic>"            
        // || S <-   :irc.example.com 333 alice #test dan!~d@localhost 1547691506
        RPL_TOPICWHOTIME = 333, // "<client> <channel> <nick> <setat>"      
        // || S <-   :irc.example.com 353 alice @ #test :alice @dan
        RPL_NAMREPLY = 353,      // "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}" 

        RPL_CHANNELMODEIS = 324,   // "<client> <channel> <modestring> <mode arguments>..."
        RPL_CREATIONTIME = 329

    } reply_t;
};

#endif // REPLYLOG_HPP