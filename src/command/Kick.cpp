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


int  getFdByNick(std::string nick)
{
    // if (nick.empty ())
    //     return 0;
    std::map < int ,Client > ::iterator it = Server::clients.begin();
    for (; it!= Server::clients.end(); ++it)
    {
        if (it->second.nickname == nick)
            return it->first ;
    }
    return  -1;
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
        reason = "";
    return (reason);
}


void    Operator::kick(Client &clt, std::vector<std::string> &command,std::map<int, Client> &clients)
{

/*
KICK #aa user :okokkookok   :

*/
    std::string reason;

    command.erase(command.begin());
    // if (command) 
    reason = reasonArg(command , 2);
    if (reason[0] == ':')
        reason.erase(0);
    // std::cout << "res --> " << reason << std::endl;
    // command.erase(command.begin());
    if (command.back() == ":")
        command.pop_back ();
    if (command.empty()) 
    {
        Server::sendMsg( clt, _ERR(clt.nickname, ERR_NEEDMOREPARAMS));
        return ;
    }
    // check if channel existed
    if (!channelFound(command[0]))
    {
        Server::sendMsg(clt, JOIN_ERR(command[0],clt, ERR_NOSUCHCHANNEL));
        return ;
    }
    // //check if user is on channel
    if (!clientIsOnChannel(command[0],clt.fd))
    {
        Server::sendMsg( clt,_ERR(clt.nickname, ERR_NOTONCHANNEL));
        return ;
    }
    // check if target is on channel 
    int targetfd = getFdByNick (command[1],clients);
    if (targetfd == -1 ||  !clientIsOnChannel(command[0],targetfd))
    {
        Server::sendMsg( clt,_ERR(clt.nickname,ERR_USERNOTINCHANNEL));
        return ;
    } 
    // check if user is operator
    if (!clt.isOperator)
    {
        Server::sendMsg( clt,_ERR(clt.nickname,ERR_CHANOPRIVSNEEDED));
        return ;
    }
    //send Reply to kicker
//    Server::sendMsg(clt, Message::getKickReply(Server::ChannelsInServer[command[0]], clt , reason, command[1]));
    //send Reply to channel 
   Server::sendMsg(Server::ChannelsInServer[command[0]], Message::getKickReply(Server::ChannelsInServer[command[0]], clt , reason, command[1]));
    //Erase client from channel vector 
   Server::ChannelsInServer[command[0]].clientsInChannel.erase(targetfd);
    if (Server::ChannelsInServer[command[0]].clientsInChannel.size() == 0)
        Server::ChannelsInServer.erase(command[0]);
}
