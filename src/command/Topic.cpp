#include <ctime>
#include "Server.hpp"

std::string holdTopic(const std::vector<std::string>& command)
{
    std::string topic;
    if (command.size() > 1)
    {
        for (size_t i = 1; i < command.size(); ++i)
        {
            topic += command[i];
        }
    }
    return topic;
}


void Operator::topic (Client &clt, std::vector<std::string> &command)
{
    if (command.back() == ":")
        command.pop_back ();
    for (size_t i = 0 ; i < command.size () ; ++i)
    {
        std::cout << "command in topic :" << command[i] << " ";
    }
        std::cout <<std::endl ;
    command.erase(command.begin());
    if (command.empty()) 
    {
        Server::sendMsg( clt, _ERR(clt.nickname, ERR_NEEDMOREPARAMS));
        return ;
    }
    std::string topic  =holdTopic(command);
    // check if channel existed
    if (!channelFound(command[0]))
    {
        Server::sendMsg( clt, JOIN_ERR(command[0],clt, ERR_NOSUCHCHANNEL));
        return ;
    }
    Channel &ch = Server::ChannelsInServer[command[0]];
      // //check if user is on channel
    if (!clientIsOnChannel(command[0],clt.fd))
    {
         Server::sendMsg( clt,_ERR(clt.nickname,ERR_NOTONCHANNEL));
        return ;
    }
       // check if it is operator
    if (ch.isInviteOnly && !clt.isOperator)
    {
        Server::sendMsg( clt,_ERR(clt.nickname,ERR_CHANOPRIVSNEEDED));
        return ;
    }
    std::cout << "topic --->" << topic <<std::endl;
    // here if topic not set
    if (command.size ()== 1)
    {
        // no  topic set
        if (ch.topic.empty())
            Server::sendMsg(clt  ,commandReply2(ch, clt, "331", ":No topic is set."));
        else // view topic 
        {
            std::cout << "reply >>" << commandReply2(ch, clt, "332", topic) << std::endl;
            Server::sendMsg (clt,commandReply2(ch, clt, "332", topic)) ;
            std::cout << "REPLY : " << commandReply2(ch, clt, "332", topic) << std::endl ;
            std::stringstream    ss;
            ss << time(NULL);
            Server::sendMsg (clt,commandReply2(ch, clt, "333", ss.str()));
        return ;
        }
    }
    // unset topic 
    else if (command.size() >= 1  && command[1].compare (":") == 0)
    {
        ch.topic = "";
        Server::sendMsg(clt,commandReply(ch, clt, "TOPIC" ,TYPE_USER) + " :");
        return ;
    }
    //set topic 
    else 
    {
        puts("here");
        std::cout << "topic" << ch.topic <<std::endl;
        ch.topic = topic;
        Server::sendMsg(clt,commandReply(ch, clt, "TOPIC" ,TYPE_USER) +topic);
        return ;
    }
}
