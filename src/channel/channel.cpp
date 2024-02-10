#include "channel.hpp"

Channel::Channel() : ChannelModes(USERS_CHANNEL_LIMIT, 0)
    , key(""), topic(""), name("")
{
}

Channel::Channel(const std::string name) : ChannelModes(USERS_CHANNEL_LIMIT, 0)
    , key(""), topic(""), name(name)
{
}

Channel::Channel(const std::string name, const std::string key, std::string topic, int usersLimit)
    : ChannelModes( usersLimit, !key.empty() ? 1 : 0 ), key(key), topic(topic), name(name)
{
    this->usersLimit = usersLimit;
}

Channel::~Channel()
{
}

const std::string	Channel::getModeString() const {

    std::string modeString("+");

    if (isInviteOnly)
        modeString.append("i");
    if (isTopic)
        modeString.append("t");
    if (isUsersLimit)
        modeString.append("l");
    if (isKey)
        modeString.append("k");
    if (isUsersLimit) {
        std::stringstream   ss;
        std::string         token;
        ss << usersLimit;
        ss >> token;
        modeString.append(" " + token);
    }
    if (isKey) {
        modeString.append(" " + getKey());
    }

    return (modeString);
}

const std::string	& Channel::getKey() const {
    return (this->key);
}
//  returns a string that contains all clients in the given channel
std::string   Channel::getClientsInString() const 
{
    std::string                   clients;
    std::string                   tmp;

    std::map<int, Client>::const_iterator it = clientsInChannel.begin();

    for (; it != clientsInChannel.end(); it++) {
        if (it->second.isOperator) {
            tmp = "@";
        }
        tmp.append(it->second.nickname + " ");

        clients.append(tmp);
        tmp.clear();
    }

    return (clients);
}
