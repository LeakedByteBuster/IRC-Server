#include "Server.hpp"

bool    isChannelNameCorrect(std::string name)
{
    // disables leaving all channels : (name.compare("0") == 0)
    if (name.empty() || name[0] != '#' || name.compare("0") == 0) {
        return (0);
    }
    
    std::string specials = " ,\a\r\n\0";
    for (size_t i = 0; i < specials.size(); i++) {
        if (name.find(specials[i]) != std::string::npos) {
            return (0);
        }
    }
    return (1);
}

#define UUUID 656
std::vector<std::pair<std::string, std::string> >
    parseJoinCommand(std::vector<std::string> &commandList, const Client &clt)
{
    // pair<channel name, channel password>
    std::vector<std::pair<std::string, std::string> >   tokens;
    std::vector<std::string>                            splited;
    std::string                                         error;

    splited = splitByValue(commandList[0], ',');
    if (splited.empty()) {
        Server::sendMsg( clt,
            JOIN_ERR( Channel(commandList[0]), clt, ERR_BADCHANMASK));
        return (tokens);
    }
    
    for (size_t i = 0; i < splited.size(); i++) {
        if (!isChannelNameCorrect(splited[i]) || splited[i].size() > MAX_CHANNEL_NAME_LEN + 1) {
            if (i != 0)
                error.append("\r\n");
            error.append(
                JOIN_ERR( Channel(splited[i]), clt, ERR_BADCHANMASK)
            );
            continue ;
        }
        tokens.push_back(std::make_pair(splited[i], ""));
    }

    if (commandList.size() >= 2) {
        std::vector<std::string>    passwords = splitByValue(commandList[1], ',');
        for (size_t i = 0; (i < passwords.size()) && (i < tokens.size()); i++) {
            tokens[i].second = passwords[i];
        }
    }

    if (!error.empty()) {
        Server::sendMsg( clt, error );
    }
    return (tokens);
}

/*
While a user is joined to a channel, they receive all status messages 
related to that channel including new JOIN, PART, KICK, and MODE messages.

If a user’s JOIN command is successful, the server:

Sends them a JOIN message described above.
May send a MODE message with the current channel’s modes.
Sends them RPL_TOPIC and RPL_TOPICTIME numerics if the channel has a topic set (if the topic is not set, the user is sent no numerics).
Sends them one or more RPL_NAMREPLY numerics (which also contain the name of the user that’s joining).
*/
void    join(Client &clt, std::vector<std::string> &command)
{
    // pair<channel name, channel key>
    std::vector<std::pair<std::string, std::string> >   tmpChannels;
    // skip first argument "join"
    command.erase(command.begin());
    if (command.empty()) {
        Server::sendMsg( clt,
            _ERR(clt.nickname, ERR_NEEDMOREPARAMS));
        return ;
    }
    if ((tmpChannels = parseJoinCommand(command, clt)).empty()) { return ; }

    clt.isOperator = 1;
    std::string id(tmpChannels[0].first);
    Server::ChannelsInServer.insert( std::make_pair ( id, Channel(command[0]) ) );
    Server::ChannelsInServer[id].clientsInChannel[clt.fd] = clt;

    Server::sendMsg(clt, Message::getJoinReply(Server::ChannelsInServer[id], clt));
    
    std::cerr << Message::getJoinReply(Server::ChannelsInServer[id], clt);
    std::cerr.flush();
    
    // for (size_t i = 0; i < tmpChannels.size(); i++) {
    //     std::cout << "tmpChan: " << tmpChannels[i].first << " " << tmpChannels[i].second << std::endl;
    // }
    // std::cerr << "{ " << Message::joinPostReply( Channel( id, ""), clt, "MODE" ) << " }" << std::endl;
    /*
        for (size_t i = 0; i < tmpChannels.size(); i++) {
            // if (channel exist) {
                create channel
                set ChannelsInServer
                send Replies
            } else if (channel not exist) {
                if (isExisted)
                    send Error
                if (isInvite == 0 && isKey == 0)
                    add client to channel
                
                send Replies to new client
                Brodcast JOIN message to channel
            }
        }
    */
    return ;
}
