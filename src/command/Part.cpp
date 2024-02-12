#include "Part.hpp"

#include "Server.hpp"

std :: string Partprefix(Client &clt)
{
    std::string prefix;
    prefix.append(":");
    prefix.append(userPrefix(clt));
    prefix.append(" PART :");
    return(prefix);
}

void Part_client(Client & clt, std::vector<std::string> command)
{
    if(command.size() < 2 || (command.size() == 2 && command[1].compare(":") == 1))
    {Server::sendMsg(clt, _ERR(clt.nickname, ERR_NEEDMOREPARAMS)); return;}

    std::vector<std::string> channels = parse_such(command[1]);
    std::string reason = compile_msg(command,2);
    for(size_t i = 0;i < channels.size();i++)
    {
        int id = check_channel(Server::ChannelsInServer , channels[i],clt);

        if(!id) // Channel Not found
        {
            Server::sendMsg(clt, JOIN_ERR(Channel(channels[i]), clt , ERR_NOSUCHCHANNEL));
            continue;
        }
        else if(id == 2) // channel found and the leaver is a member on it
        {
            std::string msg;
            std::map<std::string,Channel>::iterator it = Server::ChannelsInServer.find(channels[i]);
            if(it != Server::ChannelsInServer.end())
            {
                msg = commandReply(it->second, clt, "PART", TYPE_USER);
                msg.append(" :" + reason);

                if(it->second.clientsInChannel.size() > 1)
                {
                    Server::sendMsg(it->second,clt,msg);
                    DeleteClt(it->second,clt);
                }
                else
                {
                    Server::sendMsg(it->second,clt,msg);
                    Server::ChannelsInServer.erase(it);
                }

                Server::sendMsg(clt,msg);
            }
            else{

                Server::sendMsg(clt, JOIN_ERR(it->second, clt , ERR_NOSUCHCHANNEL));
            }
            continue;
        }
        else // the channel found but the sender is not member on it
        {
            Server::sendMsg(clt, _ERR(clt.nickname, ERR_NOTONCHANNEL));
            continue;
        }
    }


}
