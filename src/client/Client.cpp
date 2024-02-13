#include "Client.hpp"
#include "channel.hpp"
#include <sys/socket.h>

Client::Client() : fd(0)
{

}

Client::Client(const int rfd) : fd(rfd), isRegistred(0)
{
    isOperator = 0;
    memset(&hints, 0, sizeof(hints));
}

Client::Client(const Client &rhs) : fd(rhs.fd)
{
    *this = rhs;
}

Client& Client::operator=(const Client &rhs)
{
    if (this == &rhs)
        return (*this);

    username = rhs.username;
    realname = rhs.realname;
    nickname = rhs.nickname;
    isOperator = rhs.isOperator;
    isRegistred = rhs.isRegistred;
    Files = rhs.Files;
    
    memset(&hints, 0, sizeof(hints));
    hints.sin_len   = rhs.hints.sin_len;
    hints.sin_family = rhs.hints.sin_family;
    hints.sin_port = rhs.hints.sin_port;
    hints.sin_addr.s_addr = rhs.hints.sin_addr.s_addr;
    std::strcpy(hints.sin_zero, rhs.hints.sin_zero);

    return (*this);
}

Client::~Client() { }


