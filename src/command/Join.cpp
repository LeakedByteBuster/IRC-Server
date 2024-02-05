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
            Message::getJoinError( Channel(commandList[0]), clt, Message::ERR_BADCHANMASK));
        return (tokens);
    }
    
    for (size_t i = 0; i < splited.size(); i++) {
        if (!isChannelNameCorrect(splited[i])) {
            if (i != 0)
                error.append("\r\n");
            error.append(
                Message::getJoinError( Channel(splited[i]), clt, Message::ERR_BADCHANMASK)
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
            Message::getError(clt.nickname, Message::ERR_NEEDMOREPARAMS));
        return ;
    }

    if ((tmpChannels = parseJoinCommand(command, clt)).empty()) {
        return ;
    }
    
    std::string id(tmpChannels[0].first);
    Server::ChannelsInServer.insert( std::make_pair ( id, Channel() ) );
    Server::ChannelsInServer[id].clientsInChannel[clt.fd] = clt;

    // for (size_t i = 0; i < tmpChannels.size(); i++) {
    //     std::cout << "tmpChan: " << tmpChannels[i].first << " " << tmpChannels[i].second << std::endl;
    // }
    // std::cerr << "{ " << Message::joinPostReply( Channel( id, ""), clt, "MODE" ) << " }" << std::endl;
    
    // Loop through vector to set varianles of each channel
    // for (size_t i = 0; i < tmpChannels.size(); i++) {
        // check if channel existed
            // store channel
            // store channel key
            // send messages
    // }

    return ;
}
