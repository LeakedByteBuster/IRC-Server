#include "Command.hpp"
#include "../../include/channels/channel.hpp"
#include<set>
// #include "client.hpp"
std :: vector<std :: string> HandleIncomingMsg(std :: vector <std :: string> &commands,std :: string msg)
{
    std::stringstream   ss(msg);
    std :: string        token;
    std::string        tmp;
    
    std :: getline(ss,token);
    std::stringstream   parser(token);
        while (getline(parser,tmp,' ')) {
            commands.push_back(tmp);
        }
    return(commands);
}

bool is_duplicated(std::vector<std::string>& channel_names) {
    std::set<std::string> unique_channels;

    for (std::vector<std::string>::iterator it = channel_names.begin(); it != channel_names.end(); ++it) {
        std::cout << "Channel: " << *it << std::endl;
        if (!unique_channels.insert(*it).second) {
            std::cout << "Channel is duplicated" << std::endl;
            return true;
        }
    }

    return false;
}

std::vector<std::string> check_alnum(const std::string& str) {
    std::vector<std::string> channel_names;
    std::string current_channel;

    for (size_t i = 0; i < str.length(); i++) {
        if (isalnum(str[i]) || (str[i] == ',' && i + 1 < str.length() && str[i + 1] == '#')) {
            if (str[i] == ',' && !current_channel.empty()) {
                if (current_channel[0] != '#') {
                    current_channel = '#' + current_channel;
                }
                channel_names.push_back(current_channel);
                current_channel.clear();
            } else if (isalnum(str[i])) {
                current_channel += str[i];
            }
        }
    }

    if (!current_channel.empty()) {
        if (current_channel[0] != '#') {
            current_channel = '#' + current_channel;
        }
        channel_names.push_back(current_channel);
    }
    if (is_duplicated (channel_names))
    {
        std::cout << "DUPLICATEEEEED\n";
        return std::vector<std::string>();
    }
    return channel_names;
}

std::vector < std::string > check_channel_name (const std::string & commands)
{
    // char * tmp ;
    // tmp = commands[1].c_str ();
    std::vector < std::string > channel_names =check_alnum(commands) ;
            // std::cout << "size" << channels_names.size () << "\n";
    // std::cout << "channel :" << channel_names [0];
    if (commands [0] == '#' && !channel_names.empty () )
    {
        for(size_t i = 0;i < channel_names.size();i++)
        {
        std :: cout <<">>"<< channel_names[i]<<"<<"<< std ::endl;
        }
        // if (is_duplicated (channel_names))
        //     {

        //         std::cout<< "duplicate channel \n";
        //         exit (0);
        //     }
    }
    return channel_names;
    // return std::vector<std::string>();

}
void  parse_join_command(std::vector<std::string> & commands)
{
       if (commands.size()==1)
          std::cout <<ERR_NEEDMOREPARAMS<<std::endl;
            // operator::sendMsg (ERR_NEEDMOREPARAMS);
        else if (commands.size()== 2)
        {
            std::vector<std::string>channels = check_channel_name (commands[1]);
            std::cout << "size" << channels.size () << "\n";
            if (!channels.empty())
                std::cout << "valid names : "<<channels[0] << std::endl; 
                // channels.pushback(std::make_pair ())
            else 
                std::cout << ERR_BADCHANMASK << std::endl;
        }
        else if (commands.size() == 3)
        {
            check_channel_name (commands[1]);
            // check_channel_key (commands[2]);
        }
        else 
        std::cout <<ERR_NEEDMOREPARAMS<<std::endl;
            // Client::sendmsg(ERR_NEEDMOREPARAMS);

}



void parse_command(std::vector<std::string> & commands)
{
    if (!commands.empty())
    {
        std::string firstCommand = commands.front();
    if (firstCommand == "JOIN" || firstCommand == "join")
       parse_join_command(commands);
    // else if (firstCommand == "KICK")
    //     check_channel_name (commands);
    // else if (firstCommand == "MODE")
    //     check_channel_name (commands);
    // else if (firstCommand == "INVITE")
    //     check_channel_name (commands);
    else 
        std::cout << "command not found \n";

    }
}
void execute_commmand(std :: vector<std :: string> &commands,int id)
{
    (void)id;
    parse_command(commands);
    // for(size_t i = 0;i < commands.size();i++)
    // {
    //     std :: cout <<">>"<< commands[i]<<"<<"<< std ::endl;
    // }
    // std :: cout << "---------------" << std :: endl;
}
