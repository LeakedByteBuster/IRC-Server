#include "Server.hpp"

bool    isChannelNameCorrect(std::string name)
{
    std::string specials = " ,\a\r\n\0";

    if (name[0] != '#') {
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
                    Message::getJoinError(Channel(splited[i], ""), clt,
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
    // parse command input/keys
    std::vector<std::pair<std::string, std::string> >   tmpChannels;

    tmpChannels = parseJoinCommand(command, clt);
    for (size_t i = 0; i < tmpChannels.size(); i++) {
        // check if channel existed
            // store channel
            // store channel key
            // send messages
    }

    // Loop through vector to set varianles of each channel

    // for (size_t i = 0; i < tmpChannels.size(); i++) {
    //     std::cout << "tmpChan: " << tmpChannels[i].first << " " << tmpChannels[i].second << std::endl;
    // }
    return ;
}
