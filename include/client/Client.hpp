#if !defined(CLIENT_HPP)
#define CLIENT_HPP

#include <iostream>
#include <arpa/inet.h>
#include "File.hpp"
#include <vector>


/*
    - Each client is distinguished from other clients by a unique nickname
    - the real name/address of the host that the client is connecting from, 
        the username of the client on that host, and the server to which the client is connected.
    - Nicknames are non-empty strings with the following restrictions:
        They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
        They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
        They MUST NOT start with a character listed as a channel type, channel membership prefix, or prefix listed in the IRCv3 multi-prefix Extension.
        They SHOULD NOT contain any dot character ('.', 0x2E).
    - An IRC message is a single line, delimited by a pair of CR ('\r', 0x0D) and LF ('\n', 0x0A) characters.
        - When reading messages from a stream, read the incoming data into a buffer. Only parse and process a 
            message once you encounter the \r\n at the end of it. If you encounter an empty message, silently ignore it.
        - When sending messages, ensure that a pair of \r\n characters 
            follows every single message your software sends out.
    -
*/

class   Operator {

public :
    /* Eject a client from the channel */
    void    kick(){}
    /* invite a client to a channel */
    void    invite(){}
    /* Change or view the channel topic */
    void    topic(){}
    /* Change the channel’s mode */
    void    mode(/* int mode */){}
};

class   Client : public Operator{

public :
    /* doe */
    bool                isRegistred;
    /* operator priviligies */
    bool                isOperator;
    /* client info */
    std::string         nickname;
    std::string         username;
    std::string         hostname;
    // /* Connection infos */
    struct sockaddr_in  hints;
    int                 fd;

    std :: vector<file> Files;

public :
    Client();
    Client(std::string port, std::string password);
    Client& operator=(Client &rhs);
    virtual ~Client();

    // send message to other user
    void    sendMsg(int id,std :: string msg);
    virtual void    sendMsg(char/* message to a channel */) {};

};
#endif // CLIENT_HPP