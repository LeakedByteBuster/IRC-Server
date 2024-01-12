#include "Command.hpp"
#include "../../include/channels/channel.hpp"
#include<set>
#include<fstream>
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



        // if (key.length ()<4)
        //     std::cout << "PASS must be more than 4 characters\n";
    // for (size_t i =1 ; i < key.length () ;i++)
    // {
    //     if (isalnum (key[0]) &&(isalnum(key[i])|| (key[i]== ','  && isalnum(key[i+1])))) 
    //     {
    //             std::cout << "i : " << i << "\n";
    //         // if(key [i] == ',')
    //         // {
    //             std :: cout << "------- key --------" << key [i]<< std :: endl;
    //             validKeys.push_back(key);
    //         // }
    //         // else 
    //         // {
    //         //     std::cout << "param error \n";
    //         //     return std::vector<std::string>();
    //         // }
    //      }
    //     else
    //     {
    //         std::cout << "all keys must be alphanumeric separated by comma\n";
    //         return std::vector<std::string>();
    //     }
    // }
    // return validKeys;


// bool  check_if_exist(std::vector<std::string> &channels)
// {
//     // std::find()

// }

int check_is_valid(std::string name,std::string key, std::map<std::string,channel> &channelsInServer)
{
     channel& foundChannel = channelsInServer[name];
        if (foundChannel.getKey()== key)
        {
            // Key matches the channelKey attribute
            return 1;
        }
        else
        {
            // Key does not match the channelKey attribute
            return 0;
        }
}
int check_modes (std::string name,std::map<std::string,channel> &channelsInServer)
{
     channel& foundChannel = channelsInServer[name];
     if (!foundChannel.getInviteMode() && !foundChannel.getkeyMode())
        return (1);
    else 
        return (0);
}
int check_limit (std::string name,std::map<std::string,channel> &channelsInServer)
{
    channel& foundChannel = channelsInServer[name];
    if (static_cast<int>(foundChannel.get_id_clients_in_channel().size())<foundChannel.getLimit())
        return (1);
    else 
        return (0);
}


void parse_command(std::vector<std::string> & commands, std::map<std::string,channel> &channelsInServer, int id)
{

    std::map<std::vector<std::string>,std::vector<std::string> > channel_keys;
    if (!commands.empty())
    {
        if (commands.front() == "JOIN" || commands.front() == "join")
        {
            channel_keys = parse_join_command(commands);
            if (!channel_keys.empty ())
            {
            std::map<std::vector<std::string>,std::vector<std::string> > ::iterator it = channel_keys.begin();
                if (it!= channel_keys.end())
                {
                    for (std::vector<std::string>::const_iterator nameIt = it->first.begin(), keyIt = it->second.begin(); \
                    nameIt != it->first.end() || keyIt != it->second.end(); \
                    ++nameIt)
                    {
                            std::string lol = keyIt != it->second.end() ? *keyIt : ""; 

                    std::cout << "channelname == " << *nameIt << "           keys : " << *keyIt ;
                    
                    // for (std::vector<std::string>::size_type i = 0; i < keys.size(); ++i) 
                    // {
                    //     std::cout << keys[i] << " ";
                    // }
                    std::cout << "\n";

                        if (channelsInServer.find (*nameIt)!=  channelsInServer.end()) //channel already exists
                        {
                            if (check_is_valid(*nameIt,*keyIt,channelsInServer)&& check_limit(*nameIt,channelsInServer) && check_modes(*nameIt,channelsInServer) )
                                std::cout << "joined channel succesfly \n";
                            else 
                                std::cout << "invalid key \n";

                        }
                        else //new channel
                        {
                            std::cout <<"not found \n";
                            std::string lol = keyIt != it->second.end() ? *keyIt : ""; 
                            channel a(id,*nameIt,*keyIt ,1);
                            channelsInServer[*nameIt] = a;
                        }
                        if (keyIt != it->second.end())
                            keyIt++;
                    }
                    std::cout << "\n";
                }
            }
            else
            {
            std::cout << "erroor f chi blassa\n";
            // else 
            //     channel a(id,,)

            }
        }
    }
    // else if (firstCommand == "KICK")
    //     check_channel_name (commands);
    // else if (firstCommand == "MODE")
    //     check_channel_name (commands);
    // else if (firstCommand == "INVITE")
    //     check_channel_name (commands);
    else 
         std::cout << "command not found \n";
}
void execute_commmand(std::map<std::string,channel> &channelsInServer, 
    std :: vector<std :: string> &commands,int id)
{
    // std::cout << commands <<"\n"
    parse_command(commands,channelsInServer,id);
//     for(size_t i = 0;i < commands.size();i++)
//     {
//         std :: cout <<">>"<< commands[i]<<"<<"<< std ::endl;
//     }
//     std :: cout << "---------------" << std :: endl;
}
