#if !defined(CHANNEL_HPP)
#define CHANNEL_HPP

#include <map>
#include "Client.hpp"
#include "Modes.hpp"
#include "Operator.hpp"

struct	channelLimits {
	// Max users that can connect to the channel
	int	maxUsers;
	int	nameLen;
} ;

class Channel : public ChannelModes, public channelLimits {

private:
	const std::string	key;

public:
							// std::map< Client's fd, Client >
	std::map<int, Client>	clientsInChannel;
							//	Topic of the channel
	std::string				topic;
							// The channel name
	const std::string		name;

							// Operators of the channel
	// std::vector<int>		ClientsOperators;
	
	Channel();
	Channel(const std::string name);
	Channel(const std::string name, const std::string key, std::string topic, int maxUsers);
	~Channel();

	std::string getClientsInString();
};

#endif // CHANNEL_HPP
