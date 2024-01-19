#pragma once 

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include<set>
#include"Client.hpp"
#include"Errors.hpp"

// #define ERR_NEEDMOREPARAMS "461  : Not enough parameters\n"

class Client;
class channel
{
private:
	std::vector <int,Client> id_clients_in_channel;
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
	channel(int id,std::string name,std::string key ,bool isoperator);
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
};
	std::vector<std::string>  check_key(std::string &line) ;
	std::vector<std::string> parse_channel_key (std::string &key);
	std::map<std::vector<std::string>,std::vector<std::string> > parse_join_command(std::vector<std::string> & commands,int id);
	int  parse_channel_name_token (std::string token);
	bool is_duplicated(std::vector<std::string>& channel_names);
	void sendError(int fd, std::string error,std::string prefix);
