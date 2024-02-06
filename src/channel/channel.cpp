#include "channel.hpp"

Channel::Channel()
    : key(""), topic(""), name("")
{
    usersLimit = 25;

    isInviteOnly = 0;
    isUsersLimit = 0;
    isTopic = 0;
    isKey = 0;
}

Channel::Channel(const std::string name)
    : key(""), topic(""), name(name)
{
    usersLimit = 25;

    isInviteOnly = 0;
    isUsersLimit = 0;
    isTopic = 0;
    isKey = 0;
}

Channel::Channel(const std::string name, const std::string key, std::string topic, int usersLimit)
    : key(key), topic(topic), name(name)
{
    this->usersLimit = usersLimit;

    isInviteOnly = 0;
    isUsersLimit = 0;
    isTopic = 0;
    isKey = 0;
}

Channel::~Channel()
{

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

