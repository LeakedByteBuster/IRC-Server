#include "Server.hpp"

bool    isChannelNameCorrect(std::string name)
{
    std::string specials = " ,\a\r\n\0";

    if (name[0] != '#') 
        return (0);
    for (size_t i = 0; i < name.size(); i++) {
        if (specials.compare(name) == 0) {
            return (0);
        }
    }

    return (1);
}

std::vector<std::pair<std::string, std::string> >   parseJoinCommand(std::vector<std::string> &strList)
{
    // pair<channel name, channel password>
    std::vector<std::pair<std::string, std::string> >   tokens;
    std::vector<std::string>                            splited;

    // skip first argument "join"
    strList.erase(strList.begin());

    if (strList.size() >= 1) {
        
        splited = splitByValue(strList[0], ',');
        for (size_t i = 0; i < splited.size(); i++) {
            if (isChannelNameCorrect(splited[i]))
                tokens.push_back(std::make_pair(splited[i], ""));
        }

        if (strList.size() >= 2) {
            std::vector<std::string>    passwords = splitByValue(strList[1], ',');
            for (size_t i = 0; (i < passwords.size()) && (i < tokens.size()); i++) {
                tokens[i].second = passwords[i];
            }
        }
    }
    if (strList.empty())
        tokens.push_back(std::make_pair("#", ""));
    return (tokens);
}


/*
While a user is joined to a channel, they receive all status messages 
related to that channel including new JOIN, PART, KICK, and MODE messages.

If a user’s JOIN command is successful, the server:

Sends them a JOIN message described above.
May send a MODE message with the current channel’s modes.
Sends them RPL_TOPIC and RPL_TOPICTIME numerics if the channel has a topic set (if the topic is not set, the user is sent no numerics).
Sends them one or more RPL_NAMREPLY numerics (which also contain the name of the user that’s joining).
*/
void    join(Client &, std::vector<std::string> &command)
{
    // parse command input/keys
    std::vector<std::pair<std::string, std::string> >   tmpChannels;

    tmpChannels = parseJoinCommand(command);
    for (size_t i = 0; i < tmpChannels.size(); i++) {
        // check if channel existed
            // store channel
            // store channel key
            // send messages
    }

    // Loop through vector to set varianles of each channel

    // for (size_t i = 0; i < tmpChannels.size(); i++) {
    //     std::cout << "tmpChan: " << tmpChannels[i].first << " " << tmpChannels[i].second << std::endl;
    // }
    return ;
}







// bool is_duplicated(std::vector<std::string>& channel_names) 
// {
//     std::set<std::string> unique_channels;
//     for (std::vector<std::string>::iterator it = channel_names.begin(); it != channel_names.end(); ++it){
//         if (!unique_channels.insert(*it).second) {
//             return true;
//         }
//     }
//     return false;
// }

// int  parse_channel_name_token (std::string token)
// {
//     std::vector<std::string> channel_names;

//     if (token[0] == '#' && isalnum (token[1]))
//     {
    
//         for (size_t i = 1 ;i < token.length();i++)
//         {
//             if (!isalnum (token[i]))
//              return 0;
//         }
//     } 
//     else 
//         return 0;
//     return 1;
// }

// int check_is_valid_key(std::string & line)
// {
//     // if (line.length () < 4)
//     //     return (0);
//     for (size_t i =1 ; i < line.length () ;i++)
//     {
//         if (!isalnum (line [i]) || (line[i] == ',' &&!isalnum(line[i+1])))
//             return (0);
//     }
//     return (1);
// }

// std::vector<std::string> parse_channel_key (std::string &key)
// {
//     std::vector<std::string> valid_keys;
//     std::stringstream ss(key);
//     std::string line;
//     if (key.back() == ',')
//            return std::vector<std::string>();
//     while (std::getline (ss,line,','))
//     {
//         std::vector<std::string> validKeys;
//         if (!check_is_valid_key (line))
//         {
//             valid_keys.clear();
//             break;
//         }
//         else
//             valid_keys.push_back(line);
//     }
//     return valid_keys;
// }

// std::vector<std::string> parse_channel(const std::string& str)
// {
//     std::vector<std::string> channel_names;
//     std::stringstream ss(str);
//     std::string token;
//     int count_ch= 0;
//     if (str.back() == ',')
//            return std::vector<std::string>();
//     while (std::getline (ss,token,','))
//     {
//             if (!parse_channel_name_token(token))
//             {
//                 channel_names.clear();
//                 break;
//             }
//         else
//         {
//             channel_names .push_back(token);
//             count_ch++;
//         }
//     }
//     if (is_duplicated (channel_names))
//     {
//         channel_names.clear();
//         return std::vector<std::string>();

//     }
//     return channel_names;
// }

// std::map<std::vector<std::string>,std::vector<std::string> > parse_join_command(std::vector<std::string> & commands, int)
// {
//     std::map<std::vector<std::string>,std::vector<std::string> > channels_keys;
//     std::vector<std::string>validChannels;
//     if (commands.size()==1)
//     {
        
//         // sendError(id ,Message::ERR_NEEDMOREPARAMS,commands[0]);
//         return std::map<std::vector<std::string>,std::vector<std::string> >();
//     }
//     else if (commands.size()== 2)
//     {
//         validChannels = parse_channel(commands[1]);
//         if (!validChannels.empty())
//         {
//             std::vector<std::string> emptyStringVector(validChannels.size(), "");
//             channels_keys.insert(std::make_pair(validChannels, emptyStringVector));
//             return (channels_keys);
//         }
//         else 
//         {
//             // sendError(id ,Message::ERR_BADCHANMASK);
//             std::cout << Message::ERR_BADCHANMASK << std::endl;
//              return std::map<std::vector<std::string>,std::vector<std::string> >();
//         }
//     }
//     else if (commands.size() == 3)
//     {
//         validChannels =  parse_channel (commands[1]);
//         std::vector<std::string> valid_keys =parse_channel_key (commands[2]);
//         if (!valid_keys.empty() &&  validChannels.size() >= valid_keys.size())
//         {
//             channels_keys.insert(std::make_pair(validChannels, valid_keys));
//             return (channels_keys);
            
//         }
//         else
//         {
//             // sendError(id ,Message::ERR_BADCHANNELKEY,"1");
//             return std::map<std::vector<std::string>,std::vector<std::string> >();
//         }
//     }
//     else 
//     {
//         // sendError(id ,Message::ERR_NEEDMOREPARAMS,"");
//         return std::map<std::vector<std::string>,std::vector<std::string> >();
//     }

// }

/*
   <channel>    ::= ('#' | '&') <chstring>
   <chstring>   ::= <any 8bit code except SPACE, BELL, NUL, CR, LF and
                     comma (',')>
*/

