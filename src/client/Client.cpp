#include "Client.hpp"

Client::Client()
{
    memset(&hints, 0, sizeof(hints));
    fd = 0;
}

Client::Client(std::string , std::string )
{

}

Client& Client::operator=(Client &)
{
    return (*this);
}

void Client::sendMsg(int id,std :: string msg)
{
    send(id,msg.c_str(),msg.size(),0);
}

Client::~Client()
{

}
