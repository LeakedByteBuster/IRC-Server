#if !defined(CHANNEL_HPP)
#define CHANNEL_HPP

#include <map>
#include <iostream>
#include "Modes.hpp"
#include "Operator.hpp"


class Client;
#define DEFAULT_MAX_USERS_IN_CHANNEL	25

struct	channelLimits {
	int	usersLimit; // max users in channel
	int	nameLimit; // max characters in channel name
} ;

class Channel : public ChannelModes, public channelLimits {

private:
	std::string	key;

public:
	std::vector <std::string> invitedUsers; // when  mode invite only enable add client nick to this vector
	std::map<int, Client>	clientsInChannel; // std::map< Client's fd, Client >
	std::string				topic; // Topic of the channel
	std::string				modeString; // Topic of the channel
	const std::string		name; // The channel name

	Channel();
	Channel(const std::string name);
	Channel(const std::string name, const std::string key, std::string topic, int usersLimit);
	~Channel();

	std::string getUsersInString() const ;
	std::string getClientsInString() const;
	const std::string	&getKey() const ;
	void				setKey(std::string key);
	const std::string	getModeString() const ;
};

#endif // CHANNEL_HPP
