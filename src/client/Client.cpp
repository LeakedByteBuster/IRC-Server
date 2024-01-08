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

Client::~Client()
{

}
// Client::sendMsg (char * msg)
// {

// }