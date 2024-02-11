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

//  void    Server::sendMsg(const Client &target, std::string msg)
//  {
//      if (msg.size() > 0) { 
//          msg.append("\r\n");

//          ssize_t bytes;
//          if ((bytes = send(target.fd, msg.data(), msg.size(), 0)) == -1) {
//              std::cerr << "Error : " << strerror(errno) << std::endl;
//          }
//          if ((unsigned long)bytes != msg.size()) {
//              std::cerr << "Warning : data loss : buff = " << msg.size() 
//                  << " sent = " << bytes << std::endl;
//          }
// //     } else {
//          std::cerr << "Error sendMsg() : error message is empty";
//      }
//  }

//  void    Client::sendMsg(const Channels &, const std::string &) 
//  {
//     /*
//     for (all clients in channel) {
//         if (bytes = send(target.fd, msg.data(), msg.size(), 0) == -1) {
//             std::cerr << "Error : " << strerror(errno) << std::endl;
//         }
//         if (bytes )
//     }
//     */
//  }
