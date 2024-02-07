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
    /* -------------------------------------------------------------------------- */
    /*                                   TOPIC  REPLIES                           */
    /* -------------------------------------------------------------------------- */
        // RPL_NOTOPIC = 331 ,   // Sent to a client when joining a channel to inform them that the channel with the name <channel> does not have any topic set.
        // RPL_TOPIC = 332,         // Sent to a client when joining the <channel> to inform them of the current topic of the channel.
        // RPL_TOPICWHOTIME = 333  // Sent to a client to let them know who set the topic (<nick>) and when they set it (<setat> is a unix timestamp). Sent after RPL_TOPIC (332).


    } reply_t;
};

#endif // REPLYLOG_HPP