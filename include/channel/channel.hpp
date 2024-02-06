#if !defined(CHANNEL_HPP)
#define CHANNEL_HPP

#include <map>
#include "Client.hpp"
#include "Modes.hpp"
#include "Operator.hpp"

#define USERS_CHANNEL_LIMIT	25

struct	channelLimits {
	int	usersLimit; // max users in channel
	int	nameLimit; // max characters in channel name
} ;

class Channel : public ChannelModes, public channelLimits {

private:
	const std::string	key;

public:
	std::map<int, Client>	clientsInChannel; // std::map< Client's fd, Client >
	std::string				topic; // Topic of the channel
	const std::string		name; // The channel name

	Channel();
	Channel(const std::string name);
	Channel(const std::string name, const std::string key, std::string topic, int usersLimit);
	~Channel();

	std::string			getClientsInString() const ;
	const std::string	&getKey() const ;
};

#endif // CHANNEL_HPP
