#include "channel.hpp"

channel::channel()
{

}
channel::channel(Client  &clt ,std::string name,std::string key)
{
    clt.isOperator = 1 ;
    this->name = name;
    this->key = key;
    this->keyMode = 0 ;
    this->inviteMode = 0;
    this->limitMode = 0;
    this->topicMode = 0;
    this->limit = 20 ;
	this->id_clients_in_channel.insert(std::make_pair (clt.fd,clt));
   

}
//getters and setters
    std::string channel::getName(){return this->name;}
    std::map<int,Client>& channel::get_id_clients_in_channel (void){return this->id_clients_in_channel;}
	int channel::getLimit(void) const  {return this->limit;}
    const std::string & channel::getKey (void) const {return this->key;}
	bool channel::getInviteMode (void) const{return this->inviteMode;}
	bool channel::gettopicMode (void) const{return this->topicMode;}
	bool channel::getkeyMode (void) const{return this->keyMode;}
	bool channel::getOperatorMode (void) const{return this->operatorMode;}
	bool channel::getLimitMode (void) const {return this->inviteMode;}
    channel::~channel()
{
}

bool is_duplicated(std::vector<std::string>& channel_names) 
{
    std::set<std::string> unique_channels;
    for (std::vector<std::string>::iterator it = channel_names.begin(); it != channel_names.end(); ++it){
        if (!unique_channels.insert(*it).second) {
            return true;
        }
    }
    return false;
}

int  parse_channel_name_token (std::string token)
{
    std::vector<std::string> channel_names;
   
    if (token[0] == '#' && isalnum (token[1]))
    {
    
        for (size_t i = 1 ;i < token.length()-1;i++)
        {
            if (!isalnum (token[i]))
             return 0;
        }
    } 
    else 
        return 0;
    return 1;
}

int check_is_valid_key(std::string & line)
{
    for (size_t i =1 ; i < line.length ()-1 ;i++)
    {
        if (!isalnum (line [i]) || (line[i] == ',' &&!isalnum(line[i+1])))
            return (0);
    }
    return (1);
}

std::vector<std::string> parse_channel_key (std::string &key)
{
    std::vector<std::string> valid_keys;
    std::stringstream ss(key);
    std::string line;
    if (key.back() == ',')
           return std::vector<std::string>();
    while (std::getline (ss,line,','))
    {
        std::vector<std::string> validKeys;
        if (!check_is_valid_key (line))
        {
            valid_keys.clear();
            
            break;
        }
        else
            valid_keys.push_back(line);
    }
    return valid_keys;
}

std::vector<std::string> parse_channel(const std::string& str)
{
    std::vector<std::string> channel_names;
    std::stringstream ss(str);
    std::string token;
    int count_ch= 0;
    if (str.back() == ',')
        return std::vector<std::string>();
    while (std::getline (ss,token,','))
    {
        if (!parse_channel_name_token(token))
        {
            channel_names.clear();
            break;
        }
        else
        {
            channel_names .push_back(token);
            count_ch++;
        }
    }
    if (is_duplicated(channel_names))
    {
        channel_names.clear();
        return std::vector<std::string>();

    }
    return channel_names;
}

std::map<std::vector<std::string>,std::vector<std::string> > parse_join_command(std::vector<std::string> & commands,Client clt)
{
    std::map<std::vector<std::string>,std::vector<std::string> > channels_keys;
    std::vector<std::string>validChannels;
    if (commands.size()==1 || (commands.size()==2 && commands[1] == "#"))
    {
        
        Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_NEEDMOREPARAMS));
        return std::map<std::vector<std::string>,std::vector<std::string> >();
    }
    else if (commands.size()== 2)
    {
        validChannels = parse_channel(commands[1]);
        if (!validChannels.empty())
        {
            std::vector<std::string> emptyStringVector(validChannels.size(), "");
            channels_keys.insert(std::make_pair(validChannels, emptyStringVector));
            return (channels_keys);
        }
        else 
        {
            // sendError(id ,ERR_BADCHANMASK);
                Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_BADCHANMASK));
             return std::map<std::vector<std::string>,std::vector<std::string> >();
        }
    }
    else if (commands.size() == 3)
    {
        validChannels =  parse_channel(commands[1]);
        std::vector<std::string> valid_keys =parse_channel_key (commands[2]);

        if (!valid_keys.empty() &&  validChannels.size() >= valid_keys.size())
        {
            channels_keys.insert(std::make_pair(validChannels, valid_keys));
            return (channels_keys);
            
        }
        else
        {
            Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_BADCHANMASK));
            return std::map<std::vector<std::string>,std::vector<std::string> >();
        }
    }
    else 
    {
            Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_BADCHANMASK));
        return std::map<std::vector<std::string>,std::vector<std::string> >();
    }

}

int check_existed_channel (std::map<std::string,channel> &channelsInServer , std::string name)
{
    std::map<std::string, channel>::iterator iter = channelsInServer.find(name);
    if (iter != channelsInServer.end())
        return (1);
    return 0;
}   
