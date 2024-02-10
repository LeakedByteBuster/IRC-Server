#if !defined(OPERATOR_HPP)
#define OPERATOR_HPP

class   Operator {

public :
    /* Eject a client from the channel */
    static void     kick(Client &clt, std::vector<std::string> &command);
    /* invite a client to a channel */
    static void     invite (Client &clt, std::vector<std::string> &command);

    /* Change or view the channel topic */
    static void     topic(Client &clt, std::vector<std::string> &command);
    /* Change the channel’s mode */
    static void    mode(Client clt, std::vector<std::string> args);
};

std::string reasonArg (std::vector<std::string> &command,size_t positionStart);
int         channelFound (std::string name);
bool        clientIsOnChannel (std::string channelName,int fd);

#endif // OPERATOR_HPP
