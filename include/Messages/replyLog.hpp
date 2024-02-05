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
        RPL_ENDOFNAMES = 366,   // "<client> <channel> :End of /NAMES list" || S <-   :irc.example.com 366 patty #irctoast :End of /NAMES list.
        RPL_TOPIC = 332,        // "<client> <channel> :<topic>"            || S <-   :irc.example.com 332 alice #test :This is my cool channel! https://irc.com
        RPL_TOPICWHOTIME = 333, // "<client> <channel> <nick> <setat>"      || S <-   :irc.example.com 333 alice #test dan!~d@localhost 1547691506
        RPL_NAMREPLY = 353      // "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}" || S <-   :irc.example.com 353 alice @ #test :alice @dan

    } reply_t;
};

#endif // REPLYLOG_HPP