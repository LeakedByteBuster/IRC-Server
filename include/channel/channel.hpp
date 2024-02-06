#if !defined(CHANNEL_HPP)
#define CHANNEL_HPP

#include <map>
#include "Client.hpp"
#include "Modes.hpp"
#include "Operator.hpp"

struct	channelLimits {
	int	usersLimit;
	int	nameLimit; // max characters in channel name
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
	Channel(const std::string name, const std::string key, std::string topic, int usersLimit);
	~Channel();

	std::string getClientsInString() const;
	const std::string	&getKey() const ;
};

#endif // CHANNEL_HPP
