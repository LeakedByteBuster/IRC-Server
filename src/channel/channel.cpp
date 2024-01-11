#include "channel.hpp"

channel::channel()
{

}
channel::channel(Client  &client)
{
	(void)client;
}

channel::~channel()
{
}

bool is_duplicated(std::vector<std::string>& channel_names) 
{
    std::set<std::string> unique_channels;
    for (std::vector<std::string>::iterator it = channel_names.begin(); it != channel_names.end(); ++it){
        if (!unique_channels.insert(*it).second) {
            // std::cout << "Channel is duplicated" << std::endl;
            return true;
        }
    }
    return false;
}

int  parse_channel_name_token (std::string token)
{
    std::vector<std::string> channel_names;

    // std::cout << "i >> token [i] : " <<token<<"\n";
    if (token[0] == '#' && isalnum (token[1]))
    {
    
        for (size_t i = 1 ;i < token.length();i++)
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

    if (line.length () < 4)
        return (0);
    for (size_t i =1 ; i < line.length () ;i++)
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
    // std::cout << "line: " << line << "\n";
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
    if (is_duplicated (channel_names))
    {
        channel_names.clear();
        return std::vector<std::string>();

    }
    return channel_names;
}

std::map<std::vector<std::string>,std::vector<std::string> > parse_join_command(std::vector<std::string> & commands)
{
    std::map<std::vector<std::string>,std::vector<std::string> > channels_keys;
    std::vector<std::string>validChannels;
    if (commands.size()==1)
    {
        std::cout <<ERR_NEEDMOREPARAMS<<std::endl;
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
            std::cout << ERR_BADCHANMASK << std::endl;
             return std::map<std::vector<std::string>,std::vector<std::string> >();
        }
    }
    else if (commands.size() == 3)
    {
        validChannels =  parse_channel (commands[1]);
        std::vector<std::string> valid_keys =parse_channel_key (commands[2]);
        // std::cout << "channels size : " << validChannels.size() << "\n";
        // std::cout << "key size : " << valid_keys.size() << "\n";

        if (!valid_keys.empty() &&  validChannels.size() >= valid_keys.size())
        {
            channels_keys.insert(std::make_pair(validChannels, valid_keys));
            return (channels_keys);
            
        }
        else
        {
             std::cout << "BAD channel key\n";
            return std::map<std::vector<std::string>,std::vector<std::string> >();
        }
    }
    else 
    {
        std::cout <<ERR_NEEDMOREPARAMS<<std::endl;
        return std::map<std::vector<std::string>,std::vector<std::string> >();
    }
    // Client::sendmsg(ERR_NEEDMOREPARAMS);
    //print map
    //    std::map<std::vector<std::string>, std::vector<std::string> >::iterator it;
    // for (it = channels_keys.begin(); it != channels_keys.end(); ++it) {
    //     const std::vector<std::string>& key = it->first;
    //     const std::vector<std::string>& value = it->second;

    //     std::cout << "Key: ";
    //     for (std::vector<std::string>::const_iterator keyIt = key.begin(); keyIt != key.end(); ++keyIt) {
    //         std::cout << *keyIt << " ";
    //     }

    //     std::cout << "| Value: ";
    //     for (std::vector<std::string>::const_iterator valueIt = value.begin(); valueIt != value.end(); ++valueIt) {
    //         std::cout << *valueIt << " ";
    //     }

    //     std::cout << std::endl;
    // }

}

