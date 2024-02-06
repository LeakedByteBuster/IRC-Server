#if !defined(OPERATOR_HPP)
#define OPERATOR_HPP

class   Operator {

public :
    /* Eject a client from the channel */
    static void     kick(Client &clt, std::vector<std::string> &command);
    /* invite a client to a channel */
    static void    invite(){}
    /* Change or view the channel topic */
    static void    topic(){}
    /* Change the channel’s mode */
    static void    mode(/* int mode */){}
};

#endif // OPERATOR_HPP
