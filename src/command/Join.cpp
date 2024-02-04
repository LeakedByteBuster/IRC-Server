#include "Server.hpp"

bool    isChannelNameCorrect(std::string name)
{
    std::string specials = " ,\a\r\n\0";
    // disables leaving all channels : (name.compare("0") == 0)
    if (name[0] != '#' || name.compare("0") == 0) {
        return (0);
    }
    for (size_t i = 0; i < specials.size(); i++) {
        if (name.find(specials[i]) != std::string::npos) {
            return (0);
        }
    }
    return (1);
}

std::vector<std::pair<std::string, std::string> >
    parseJoinCommand(std::vector<std::string> &strList, const Client &clt)
{
    // pair<channel name, channel password>
    std::vector<std::pair<std::string, std::string> >   tokens;
    std::vector<std::string>                            splited;

    // skip first argument "join"
    strList.erase(strList.begin());

    if (strList.size() >= 1) {
        
        splited = splitByValue(strList[0], ',');
        for (size_t i = 0; i < splited.size(); i++) {
            if (!isChannelNameCorrect(splited[i])) {
                Server::sendMsg( clt,
                    Message::getJoinError( Channel(splited[i]), clt,
                        Message::ERR_BADCHANMASK));
                continue ;
            }
            tokens.push_back(std::make_pair(splited[i], ""));
        }

        if (strList.size() >= 2) {
            std::vector<std::string>    passwords = splitByValue(strList[1], ',');
            for (size_t i = 0; (i < passwords.size()) && (i < tokens.size()); i++) {
                tokens[i].second = passwords[i];
            }
        }
    }
    if (strList.empty()) // string given is empty
        tokens.push_back(std::make_pair("#", ""));
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


    std::cout << "here 1 " << std::endl;
    tmpChannels = parseJoinCommand(command, clt);
    if (tmpChannels.empty())
        return ;
    for (size_t i = 0; i < tmpChannels.size(); i++) {
        std::cout << "tmpChan: " << tmpChannels[i].first << " " << tmpChannels[i].second << std::endl;
    }
    std::string id(tmpChannels[0].first);

    Server::ChannelsInServer.insert( std::make_pair ( id, Channel() ) );

    Server::ChannelsInServer[id].clientsInChannel[clt.fd] = clt;
    std::cout << "PP:" <<  Server::ChannelsInServer[id].getClientsInString() << std::endl;

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
