#include "Server.hpp"



void Operator::invite (Client &clt, std::vector<std::string> &command,std::map<int, Client> &clients)
{
     if (command.size() < 3 || (command.size() == 2 && command[1].compare(":") == 0))
    {
        Server::sendMsg( clt, _ERR(clt.nickname, ERR_NEEDMOREPARAMS));
        return ;
    }
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
    int targetfd =getFdByNick(command[0],clients);
    if (targetfd != -1)
    {
        if (clientIsOnChannel(ch.name,getFdByNick(command[0],clients)))
        {
            Server::sendMsg( clt,_ERR(clt.nickname,ERR_USERONCHANNEL));
            return ;
        }
    }
    else 
    {
       Server::sendMsg(clt,_ERR(clt.nickname,ERR_NOSUCHNICK));
            return ;
    }
    Server::sendMsg(targetfd, commandReply4 (ch,clt,"INVITE",TYPE_USER,command[0]));
    Server::sendMsg(clt.fd, commandReply5 (ch,clt, TYPE_SERVER,command[0]));   
    ch.invitedUsers.push_back (command[0]);
}