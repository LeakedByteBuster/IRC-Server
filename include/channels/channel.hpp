#pragma once 

#include <iostream> 
// join errors 
#include "vector"
#include"../client/Client.hpp"
#define ERR_NEEDMOREPARAMS ":Not enough parameters"
#define ERR_NOSUCHCHANNEL ":No such channel"
#define ERR_TOOMANYCHANNELS ":You have joined too many channels"
#define ERR_BADCHANNELKEY ":Cannot join channel (+k)"
#define ERR_BANNEDFROMCHAN ":Cannot join channel (+b)"
#define ERR_CHANNELISFULL ":Cannot join channel (+l)"
#define ERR_INVITEONLYCHAN ":Cannot join channel (+i)"
#define ERR_BADCHANMASK ":Bad Channel Mask"
#define RPL_TOPIC ":"
#define RPL_TOPICWHOTIME " : "
#define RPL_NAMREPLY ": "
#define RPL_ENDOFNAMES ":End of /NAMES list"
class channel
{
private:
	std::vector <int> fdclients;
	// char mode;

public:
	channel(int client);
	~channel();
};

channel::channel(int client)
{
	(void)client;
}

channel::~channel()
{
}
