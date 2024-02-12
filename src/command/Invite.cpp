#include "Server.hpp"

void Operator::invite (Client &clt, std::vector<std::string> &command,std::map<int, Client> &clients)
{
     if (command.size() < 3 || (command.size() == 2 && command[1].compare(":") == 0))
    {
        Server::sendMsg( clt, _ERR(clt.nickname, ERR_NEEDMOREPARAMS));
        return ;
    }
    // for (size_t i = 0 ; i < command.size () ; ++i)
    // {
    //    std::cout << "command in invite :" << command[i] << " ";
    // }
        // std::cout <<std::endl ;
    
    command.erase (command.begin ());
    if (command.empty()) 
    {
        Server::sendMsg( clt, _ERR(clt.nickname, ERR_NEEDMOREPARAMS));
        return ;
    }
    //  std::cout << "size-->" <<ch.clientsInChannel.size()  << std::endl ;
    if (!channelFound(command[1]) || Server::ChannelsInServer[command[1]].clientsInChannel.size() < 1 )
    {
        Server::sendMsg( clt, JOIN_ERR(command[1],clt, ERR_NOSUCHCHANNEL));
        return ;
    }
    Channel &ch = Server::ChannelsInServer[command[1]];
    if (!clientIsOnChannel(command[1],clt.fd))
    {
        Server::sendMsg(clt,_ERR(clt.nickname,ERR_NOTONCHANNEL));
        return ;
    }
    if (ch.isInviteOnly && !clt.isOperator)
    {
        Server::sendMsg( clt,_ERR(clt.nickname,ERR_CHANOPRIVSNEEDED));
        return ;
    }
    if (getFdByNick(command[0],clients)!= -1)
    {
        if (clientIsOnChannel(ch.name,getFdByNick(command[0],clients)))
        {
            Server::sendMsg( clt,_ERR(clt.nickname,ERR_USERONCHANNEL));
            return ;
        }
    }

    Server::sendMsg(clt, commandReply (ch,clt,"INVITE",TYPE_SERVER) + " :" + command[1]);
    int id = 0;
    if ((id = getFdByNick(command[0],clients))!= -1) {
        Server::sendMsg(clients[id], commandReply (ch,clt,"INVITE",TYPE_USER) + " :" + command[1]);
        ch.invitedUsers.push_back(clients[id].nickname);
    }
    // Server::sendMsg(ch, clt, commandReply (ch,clt,"INVITE",TYPE_USER) + " :"  + command[1]);
    
}