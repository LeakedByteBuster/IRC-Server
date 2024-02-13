#include "Server.hpp"
#include <algorithm>

bool    isChannelNameCorrect(std::string name)
{
    // disables leaving all channels : (name.compare("0") == 0)
    if (name.empty() || name[0] != '#' || name.compare("0") == 0) {
        return (0);
    }
    
    std::string specials = " \a\r\n\0";
    for (size_t i = 0; i < specials.size(); i++) {
        if (name.find(specials[i]) != std::string::npos) {
            return (0);
        }
    }
    return (1);
}

std::vector<std::pair<std::string, std::string> >
    parseJoin(std::vector<std::string> &commandList, const Client &clt)
{
    // pair<channel name, channel password>
    std::vector<std::pair<std::string, std::string> >   tokens;
    std::vector<std::string>                            splited;
    std::string                                         error;

    splited = splitByValue(commandList[0], ',');
    if (splited.empty()) {
        Server::sendMsg(clt, JOIN_ERR(Channel(commandList[0]), clt, ERR_BADCHANMASK));
        return (tokens);
    }
    
    for (size_t i = 0; i < splited.size(); i++) {
        if (!isChannelNameCorrect(splited[i]) || splited[i].size() > MAX_CHANNEL_NAME_LEN + 1) {
            if (i != 0)
                error.append("\r\n");
            error.append(JOIN_ERR( Channel(splited[i]), clt, ERR_BADCHANMASK));
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

bool    tryInsert(const std::string &name, const std::string key)
{
    std::pair<std::map<std::string, Channel>::iterator, bool>    it;
    
    it = Server::ChannelsInServer.insert(std::make_pair(name, Channel(name, key, "", USERS_CHANNEL_LIMIT)));
    return ((it.second == 1) ? 1 : 0);
}

void    join(Client &clt, std::vector<std::string> &command)
{
    command.erase(command.begin()); // skip first argument "JOIN"
    if (command.empty()) { return (Server::sendMsg( clt, _ERR(clt.nickname, ERR_NEEDMOREPARAMS)), (void)0); }

    // pair<channel name, channel key>
    std::vector<std::pair<std::string, std::string> >   input;
    if ((input = parseJoin(command, clt)).empty()) { return ; }

    for (size_t i = 0; i < input.size(); i++) {
        std::string name = input[i].first;
        std::string key = input[i].second;

        if ( tryInsert(name, key) ) {
            Channel &ch = Server::ChannelsInServer[name];
            std::pair<std::string,Channel> cltPair(name,ch);
            
            if ( !key.empty() ) { ch.isKey = 1; }
            clt.isOperator = 1; // set clt as an operator
            //  insert clt in channel's client map
            ch.clientsInChannel.insert(std::make_pair(clt.fd, clt));
            clt.ChannelIn.insert(cltPair);
            Server::sendMsg(clt, Message::getJoinReply(ch, clt));
            continue ;
        }
        
        Channel &ch = Server::ChannelsInServer[name];
        if (ch.isKey && (ch.getKey().compare(key) != 0)) { Server::sendMsg(clt, JOIN_ERR(ch, clt, ERR_BADCHANNELKEY)); continue ; }
        if (ch.isInviteOnly) {
            if ( std::find(ch.invitedUsers.begin(), ch.invitedUsers.end(), clt.nickname) == ch.invitedUsers.end()) {
                Server::sendMsg(clt, JOIN_ERR(ch, clt, ERR_INVITEONLYCHAN));
                continue ;
            }
        }

        std::pair<std::string,Channel> cltPair(name,ch);

        clt.isOperator = 0; // reInitialize it to zero
        std::__1::pair<std::__1::map<int, Client>::iterator, bool> it;
        it = ch.clientsInChannel.insert(std::make_pair(clt.fd, clt));
        if (it.second == 1) {
            clt.ChannelIn.insert(cltPair);
            Server::sendMsg(clt, Message::getJoinReply(ch, clt));
            /* BroadCast message */
            Server::sendMsg(ch, clt, ":" + userPrefix(clt) + " JOIN :" + ch.name); //S <-   :alice!a@localhost JOIN :#irctoast
        }
    
    }
    return ;
}
