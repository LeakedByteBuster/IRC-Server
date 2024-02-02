#include "Server.hpp"

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
        
//         // sendError(id ,ERR_NEEDMOREPARAMS,commands[0]);
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
//             // sendError(id ,ERR_BADCHANMASK);
//             std::cout << ERR_BADCHANMASK << std::endl;
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
//             // sendError(id ,ERR_BADCHANNELKEY,"1");
//             return std::map<std::vector<std::string>,std::vector<std::string> >();
//         }
//     }
//     else 
//     {
//         // sendError(id ,ERR_NEEDMOREPARAMS,"");
//         return std::map<std::vector<std::string>,std::vector<std::string> >();
//     }

// }

/*
   <channel>    ::= ('#' | '&') <chstring>
   <chstring>   ::= <any 8bit code except SPACE, BELL, NUL, CR, LF and
                     comma (',')>
*/

bool    isChannelNameCorrect(std::string name)
{
    std::string specials = " ,\a\r\n\0"

    for (int i = 0; i < name.size(); i++) {
        if (specials.compare(name[i]) == 0) {
            return (0);
        }
    }

    return (1);
}

std::vector<std::pair<std::string, std::string> >   parseJoinCommand(std::vector<std::string> &strList)
{
    std::vector<std::pair<std::string, std::string> >   tokens;
    std::vector<std::string>                            splited;

    // skip first argument "join"
    strList.erase(0);

    if (strList.size() > 2)
        /* this is an error */

    splited = splitByValue(strList[0], ',');
    for (size_t i = 0; i < splited.size(); i++) {
        if (!isChannelNameCorrect(splited[i]))
            tokens.push_back(std::make_pair(splited[i], ""));
    }

    if (strList.size() == 2) {
        std::vector<std::string>    passwords = splitByValue(strList[1], ',');
        for (size_t i = 0; (i < passwords.size()) && (i < tokens.size()); i++) {
            tokens[i].second = passwords[i];
        }
    }

    return (tokens);
}

void    join(Client &clt, std::vector<std::string> &command)
{
    // parse command input/keys
    std::vector<std::pair<std::string, std::string> >   tmpChannels;

    tmpChannels = parseJoinCommand(command);



    // Loop through vector to check and set each channel 
    return ;
}