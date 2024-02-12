#include "channel.hpp"
#include "Client.hpp"
#include "Messages.hpp"

Channel::Channel() : ChannelModes()
    , key(""), topic(""), name("")
{
}

Channel::Channel(const std::string name) : ChannelModes()
    , key(""), topic(""), name(name)
{
}

Channel::Channel(const std::string name, const std::string key, std::string topic, int usersLimit)
    : ChannelModes(!key.empty() ? 1 : 0 ), key(key), topic(topic), name(name)
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

void    Channel::setKey(std::string key) {
    this->key = key;
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
        printLog(it->second.isOperator, "op = ");
        if (it->second.isOperator) {
            tmp = "@";
        }
        tmp.append(it->second.nickname + " ");

        clients.append(tmp);
        tmp.clear();
    }

    return (clients);
}

//  returns a string that contains all clients in the given channel
std::string   Channel::getUsersInString() const 
{
    std::string                   clients;
    std::map<int, Client>::const_iterator it = clientsInChannel.begin();

    for (; it != clientsInChannel.end(); it++) {
        clients.append(it->second.nickname + " ");
    }

    return (clients);
}

