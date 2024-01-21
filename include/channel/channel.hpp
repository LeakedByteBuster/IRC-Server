#pragma once 

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
// join errors 
#include "Client.hpp"
#include"Server.hpp"
#include "vector"
#include<set>
// #include"Server.hpp"
// // #include"Client.hpp"
// // #define ERR_NEEDMOREPARAMS "461  : Not enough parameters\n"
// #define ERR_NOSUCHCHANNEL "403 :No such channel\n"
// #define ERR_TOOMANYCHANNELS ":You have joined too many channels\n"
// // #define ERR_BADCHANNELKEY "475 :Cannot join channel (+k)\n"
// // #define ERR_BANNEDFROMCHAN ":Cannot join channel (+b)\n"
// #define ERR_CHANNELISFULL "471:Cannot join channel (+l)\n"
// #define ERR_INVITEONLYCHAN ":Cannot join channel (+i)\n"
// #define ERR_BADCHANMASK "476:Bad Channel Mask\n"
// #define RPL_TOPIC "332:TOPIC\n"
// #define RPL_TOPICWHOTIME " 333 : TOPICTIME \n"
// #define RPL_NAMREPLY ": NAMERPLY\n"
// #define RPL_ENDOFNAMES ":End of /NAMES list \n"

// class Server;
// class Client;
class channel
{
private:
	std::vector <int> id_clients_in_channel;
	std::string name;
	std::string key;
	std::string topic;
	int limit;
	bool inviteMode;
	bool topicMode;
	bool keyMode;
	bool operatorMode;
	bool limitMode;
public:
	channel();
	channel(Client clt,std::string name,std::string key ,bool isoperator);
	~channel();
	std::vector <int> &get_id_clients_in_channel (void);
	const std::string & getKey (void) const;
	std::string & getTopic (void) const  ;
	int getLimit(void) const ;
	bool getInviteMode (void) const;
	bool gettopicMode (void) const;
	bool getOperatorMode (void) const;
	bool getkeyMode (void) const;
	bool getLimitMode (void) const ;
	void setKey (std::string key) ;
	void setName (std::string name) ;
	void setTopic (std::string topic) ;
	std::string getName();
};
	std::vector<std::string>  check_key(std::string &line) ;
	std::vector<std::string> parse_channel_key (std::string &key);
	std::map<std::vector<std::string>,std::vector<std::string> > parse_join_command(std::vector<std::string> & commands,Client clt);
	int  parse_channel_name_token (std::string token);
	bool is_duplicated(std::vector<std::string>& channel_names);
	void sendError(int fd, std::string error,std::string prefix);
