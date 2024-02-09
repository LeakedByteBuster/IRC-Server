#if !defined(OPERATOR_HPP)
#define OPERATOR_HPP

class   Operator {

public :
    /* Kicks a client from the channel */
    static void    kick() {}
    /* invite a client to a channel */
    static void    invite(){}
    /* Change or view the channel topic */
    static void    topic(){}
    /* Change the channel’s mode */
    static void    mode(Client clt, std::vector<std::string> args);
};

#endif // OPERATOR_HPP
