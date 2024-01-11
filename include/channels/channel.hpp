#pragma once 

#include <iostream> 
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
// join errors 
#include "vector"
#include<set>
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

class server;
class Client;
class channel
{
private:
	std::vector <int> id_clients_in_channel;
	std::string name;
	std::string key;
	std::string topic;
	int limit;
	
	bool inviteMode;
	bool topicMde;
	bool keyMode;
	bool operatorMode;
	bool limitMode;
public:
	channel();
	channel(Client  &client);
	~channel() ;
	std::string & getKey (void) ;
	std::string & getName (void) ;
	std::string & getTopic (void) ;
	void setKey (std::string key) ;
	void setName (std::string name) ;
	void setTopic (std::string topic) ;
	
};
	std::vector<std::string>  check_key(std::string &line) ;
	std::vector<std::string> parse_channel_key (std::string &key);
	std::map<std::vector<std::string>,std::vector<std::string> > parse_join_command(std::vector<std::string> & commands);
	int  parse_channel_name_token (std::string token);
	bool is_duplicated(std::vector<std::string>& channel_names);

