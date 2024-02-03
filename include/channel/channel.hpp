#if !defined(CHANNEL_HPP)
#define CHANNEL_HPP

#include <map>
#include "Client.hpp"
#include "Modes.hpp"
#include "Operator.hpp"

class Channel : public ChannelModes, public Operator {

private:
	const std::string	name;
	const std::string	key;

public:
	std::map<int, Client>	ClientsInChannel;
	int						maxUsersLimit;
	// std::vector<int>		ClientsOperators;

	Channel();
	~Channel();

};

#endif // CHANNEL_HPP
