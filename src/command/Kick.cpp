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
        std::cout << "str --->" << str << std::endl ;
        std::vector<std::string> tokens = splitBySpace(str);
        std::vector<std::string> ::iterator it;
        it  = std::find(tokens.begin () ,tokens.end(),name);
        if (it != tokens.end())
            return 1;
    }
    return 0;
}

void    Operator::kick(Client &clt, std::vector<std::string> &command)
{
     // skip first argument "kick"
    std::vector<std::string>   splited = splitBySpace(command[0]);

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
    //sendmsg ()
    // if (command[3])
    // {
    //     std::string reason = 
        std::cout  << " KICKED successfly " << std::endl ;
    //kick.isOperator ;
    //kick client ;
   Server::sendMsg(clt, Message::getKickReply(Server::ChannelsInServer[command[0]], clt , command[2] , command[1]));
   Server::ChannelsInServer[command[0]].clientsInChannel.erase(clt.fd);
}
   
   
   
   
   
   
    // if (commands.size() == 3 || commands.size() == 4)
    // {   
    //         // std::cout << "commands--->" << commands[0] << std::endl ;
    //         // std::cout << "commands--->" << commands[1] << std::endl ;
    //         // std::cout << "commands--->" << commands[2] << std::endl ;
    //         // std::cout << "commands--->" << commands[3] << std::endl ;
    //             std::cout << "clt.isOperator  >>>> " << clt.isOperator << std::endl;
    //     if (parse_channel_name_token (commands[1]) && check_existed_channel(channelsInServer,commands[1]))
    //     {
    //         if (is_client_in_channel(commands[1] ,channelsInServer,clt.nickname) && is_client_in_channel (commands[1],channelsInServer, commands[2]))
    //         {
    //                     if (clt.isOperator == 1)
    //                     {
    //                         clt.inChannel.erase(std::remove(clt.inChannel.begin (),clt.inChannel.end (),commands[1]),clt.inChannel.end());
    //                         Server::sendMsg (clt, ":" + clt.nickname + "!" + clt.username + "@" + host_name() + " KICK " + commands[1] + " " + commands[2] + " :" + clt.nickname);
    //                         channelsInServer.erase(commands[1]);
    //                         sendMsg_to_channel(commands[1],channelsInServer, ":" + clt.nickname + "!" + clt.username + "@" + host_name() + " KICK " + commands[1] + " " + commands[2] + " :" + clt.nickname , clt );
    //                         // Server::sendMsg (, ":" + clt.nickname + "!" + clt.username + "@" + host_name() + " KICK " + commands[1] + " " + commands[2] + " :" + clt.nickname);
                           
    //                         // std::cout << "deleted clt n : " << clt.fd << std::endl;
    //                     }
    //                     else
    //                    Server::sendMsg(clt ,Message::getError (clt.nickname, Message ::ERR_CHANOPRIVSNEEDED));
    //         }
    //         else
    //             Server::sendMsg(clt ,Message::getError (clt.nickname, Message ::ERR_USERNOTINCHANNEL));
    //             /// 401 here no such nick /channel 
    //     }
    //     else
    //         Server::sendMsg(clt ,Message::getError (clt.nickname, Message ::ERR_NOSUCHCHANNEL));
    // }
    // else
    //     Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_NEEDMOREPARAMS));
// }