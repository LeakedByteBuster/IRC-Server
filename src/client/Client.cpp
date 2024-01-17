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

void    Client::sendMsg(const Client &target, std::string msg)
{
    if (msg.size() > 0) { 
        msg.append("\r\n");

        ssize_t bytes;
        if ((bytes = send(target.fd, msg.data(), msg.size(), 0)) == -1) {
            std::cerr << "Error : " << strerror(errno) << std::endl;
        }
        if ((unsigned long)bytes != msg.size()) {
            std::cerr << "Warning : data loss : buff = " << msg.size() 
                << " sent = " << bytes << std::endl;
        }
    } else {
        std::cerr << "Error sendMsg() : error message is empty";
    }
}

// void    Client::sendMsg(const Channels &, const std::string &) 
// {
    /*
    for (all clients in channel) {
        if (bytes = send(target.fd, msg.data(), msg.size(), 0) == -1) {
            std::cerr << "Error : " << strerror(errno) << std::endl;
        }
        if (bytes )
    }
    */
// }
