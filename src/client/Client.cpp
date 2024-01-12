#include "Client.hpp"
#include <sys/socket.h>

Client::Client() : isRegistred(0), isOperator(0), fd(0)
{
    memset(&hints, 0, sizeof(hints));
}

Client::Client(std::string , std::string )
{

}

Client& Client::operator=(Client &)
{
    return (*this);
}

Client::~Client()
{

}

/*
//  getClientInfo (Client &clt) {
    str = (['!' <user> ] ['@' <host> ])
    return (str)
}
*/
