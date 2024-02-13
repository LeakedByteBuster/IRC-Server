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

    if (command.size() < 2 || (command.size() == 2 && command[1].compare(":") == 0))
    {
        Server::sendMsg( clt, _ERR(clt.nickname, ERR_NEEDMOREPARAMS));
        return ;
    }
    
    command.erase(command.begin());
    std::string topic  = reasonArg(command , 1);
    if (topic.find(" ") != std::string::npos)
        topic.erase(topic.begin() + 1);
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
    // here if topic not set
    if (command.size ()== 1 )
    {
        // no  topic set
        if (ch.topic.empty())
            Server::sendMsg(clt  ,commandReply2(ch, clt, "331", "No topic is set."));

        else // view topic 
        {
            Server::sendMsg (clt,commandReply2(ch, clt, "332", ch.topic)) ;
            std::stringstream    ss;
            ss << time(NULL);
            Server::sendMsg (clt,commandReply2(ch, clt, "333", ss.str()));
        return ;
        }
    }
    else 
    {
        if ((ch.isTopic && !ch.clientsInChannel[clt.fd].isOperator))
        {
            Server::sendMsg( clt,_ERR(clt.nickname,ERR_CHANOPRIVSNEEDED));
            return ;
        }
        ch.topic = topic;
        std::string msg = commandReply3(ch, clt, "TOPIC" ,TYPE_USER, ch.topic);
        Server::sendMsg(clt,msg);
        return ;
    }
}
