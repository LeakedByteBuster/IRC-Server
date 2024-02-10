#include "Server.hpp"



void Operator::invite (Client &clt, std::vector<std::string> &command)
{
    // if (command.back() == ":")
    //     command.pop_back ();
    for (size_t i = 0 ; i < command.size () ; ++i)
    {
       std::cout << "command in invite :" << command[i] << " ";
    }
        std::cout <<std::endl ;
    command.erase (command.begin ());
    if (command.empty()) 
    {
        Server::sendMsg( clt, _ERR(clt.nickname, ERR_NEEDMOREPARAMS));
        return ;
    }
    if (!channelFound(command[1]))
    {
        Server::sendMsg( clt, JOIN_ERR(command[0],clt, ERR_BADCHANMASK));
        return ;
    }
     Channel &ch = Server::ChannelsInServer[command[1]];
    (void) ch;
    if (!clientIsOnChannel(command[1],clt.fd))
    {
         Server::sendMsg( clt,_ERR(clt.nickname,ERR_NOTONCHANNEL));
        return ;
    }
    // if (ch.isInviteOnly && !clt.isOperator)
    // {
    //     Server::sendMsg( clt,_ERR(clt.nickname,ERR_CHANOPRIVSNEEDED));
    //     return ;
    // }
    // if (!UserInChannel(command[0],command[1]))
    // {
    //     Server::sendMsg( clt,_ERR(clt.nickname,ERR_USERNOTINCHANNEL));
    //     return ;
    // }
    // if (OnInvitelist(command[0],command[1]))
    // {
    //      Server::sendMsg( clt,_ERR(clt.nickname,ERR_USERONCHANNEL));
    //     return ;
    // }
    
}