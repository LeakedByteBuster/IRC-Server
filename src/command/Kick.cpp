#include "Server.hpp" 



int  channelFound (std::string name)
{
    std::map<std::string, Channel>  :: iterator it = Server::ChannelsInServer.find(name);
    if (it!= Server::ChannelsInServer.end ())
        return (1);
    return  (0);
}

bool clientIsOnChannel (std::string channelName,int fd)
{
    std::map<std::string, Channel>  :: iterator it = Server::ChannelsInServer.find(channelName);
    if (it != Server::ChannelsInServer.end ())
    {
        std::map <int,Client> ::iterator it_C = it->second.clientsInChannel.find(fd);
        if (it_C != it->second.clientsInChannel.end ())
        return (1);
    }
    return (0);
}

Channel  getChannnel (std::string name)
{
     std::map<std::string, Channel>  :: iterator it = Server::ChannelsInServer.find(name);
    if (it == Server::ChannelsInServer.end ())
        return (it->second);
    return Channel ();
}

bool UserInChannel(std::string name,std::string channelName)
{
    std::map<std::string, Channel> ::iterator iter = Server::ChannelsInServer.find(channelName);
    if (iter!= Server::ChannelsInServer.end ())
    {
        std::string str  = iter->second.getUsersInString();
        std::vector<std::string> tokens = splitBySpace(str);
        std::vector<std::string> ::iterator it;
        it  = std::find(tokens.begin () ,tokens.end(),name);
        if (it != tokens.end())
            return 1;
    }
    return 0;
}
std::string reasonArg (std::vector<std::string> &command,size_t positionStart)
{
    std::string reason; 
    if (command.size() > positionStart )
    {
        for (size_t i = positionStart ; i < command.size () ; ++i)
        {
            reason = reason+" "+command[i];
        }
    }
    else
        reason = " NO reason .";
    return (reason);
}

void    Operator::kick(Client &clt, std::vector<std::string> &command)
{
     // skip first argument "kick"
    std::vector<std::string>   splited = splitBySpace(command[0]);
    std::string reason =reasonArg(command , 3);
    command.erase(command.begin());
    if (command.empty()) 
    {
        Server::sendMsg( clt, _ERR(clt.nickname, ERR_NEEDMOREPARAMS));
        return ;
    }
    // check if channel existed
    if (!channelFound(command[0]))
    {
         Server::sendMsg( clt, JOIN_ERR(command[0],clt, ERR_BADCHANMASK));
        return ;
    }
    // //check if user is on channel
    if (!clientIsOnChannel(command[0],clt.fd))
    {
         Server::sendMsg( clt,_ERR(clt.nickname,ERR_NOTONCHANNEL));
        return ;
    }
    // check if it is operator
    if (!clt.isOperator)
    {
        Server::sendMsg( clt,_ERR(clt.nickname,ERR_CHANOPRIVSNEEDED));
        return ;
    }
    // check if target is on channel 
    if (!UserInChannel(command[1],command[0]))
    {
        Server::sendMsg( clt,_ERR(clt.nickname,ERR_USERNOTINCHANNEL));
        return ;
    }
    //send Reply
   Server::sendMsg(clt, Message::getKickReply(Server::ChannelsInServer[command[0]], clt , reason , command[1]));
//    Server::sendMsg(clt, Message::getKickReply(Server::ChannelsInServer[command[0]], clt , reason , command[1]));
   Server::sendMsg(Server::ChannelsInServer[command[0]], Message::getKickedReply(Server::ChannelsInServer[command[0]], clt, command[1]));
   Server::ChannelsInServer[command[0]].clientsInChannel.erase(clt.fd);
}
