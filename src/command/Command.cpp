#include "Command.hpp"
#include "../../include/channels/channel.hpp"
#include<set>
#include<fstream>
// #include "client.hpp"
// std::vector<int> channel::get_id_clients_in_channel;
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

void sendError(int fd, std::string error,std::string prefix)
{
    std::string msg = prefix + error;
    int i = write (fd,msg.c_str(),msg.size());
    if (i == -1)
        std::cout << "write Error \n";
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

int check_is_valid(std::string name,std::string key, std::map<std::string,channel> &channelsInServer,int id )
{
    channel& foundChannel = channelsInServer[name];
    if (foundChannel.getKey().compare (key) == 0)
        return 1;
    else 
        sendError(id,ERR_BADCHANNELKEY,name);
    return 0;
       
}
int check_modes (std::string name,std::map<std::string,channel> &channelsInServer)
{
     channel& foundChannel = channelsInServer[name];
     if (!foundChannel.getInviteMode() && !foundChannel.getkeyMode())
        return (1);
    else 
        return (0);
}
int check_limit (std::string name,std::map<std::string,channel> &channelsInServer,int id )
{
    channel& foundChannel = channelsInServer[name];
    std::__1::vector<int> vector_ids = foundChannel.get_id_clients_in_channel();
    std::__1::vector<int> ::iterator it =std::find(vector_ids .begin() ,vector_ids .end(),id);
    for (size_t i =0; i <vector_ids.size();i++)
        std::cout << "vec >>> " <<vector_ids[i] << "\n";
    if (it!=vector_ids .end())
    {
        std::cout << "already in channel\n" << name << "\n";
        return (0);
    }
    if (static_cast<int>(vector_ids.size()+1)>foundChannel.getLimit())
    {
        std::cout << "cant access  channel limit  \n";
        return (0);
    }

    // }
    return (1);
}

void parse_command(std::vector<std::string> & commands, std::map<std::string,channel> &channelsInServer, int id)
{
    std::map<std::vector<std::string>,std::vector<std::string> > channel_keys;
    if (!commands.empty())
    {
        if (commands.front() == "JOIN" || commands.front() == "join")
        {
            channel_keys = parse_join_command(commands,id);
            if (!channel_keys.empty ())
            {
            std::map<std::vector<std::string>,std::vector<std::string> > ::iterator it = channel_keys.begin();
                if (it!= channel_keys.end())
                {
                    for (std::vector<std::string>::const_iterator nameIt = it->first.begin(), keyIt = it->second.begin(); \
                        nameIt != it->first.end() || keyIt != it->second.end(); ++nameIt)
                    {
                            std::string lol = keyIt != it->second.end() ? *keyIt : ""; 
                        if (channelsInServer.find (*nameIt)!=  channelsInServer.end()) //channel already exists
                        {
                            if (check_is_valid(*nameIt,*keyIt,channelsInServer,id)&& check_limit(*nameIt,channelsInServer,id) && check_modes(*nameIt,channelsInServer))
                                {
                                   std::vector<int>& channelData = channelsInServer[*nameIt].get_id_clients_in_channel();
                                   channelData.push_back (id);
                                    channelData.insert(channelData.end(), 0, id);
                                   for (unsigned long i = 0; i < channelsInServer[*nameIt].get_id_clients_in_channel().size(); i++) {
                                        std::cout << "id : " << *nameIt << " value ==> " <<  channelsInServer[*nameIt].get_id_clients_in_channel()[i] << std::endl;
                                   }
                                    // std::cout << "client :"  << id <<"joined channel succesfly \n";
                                }
                            else 
                                sendError(id,ERR_BADCHANNELKEY,"111112");
                        }
                        else //new channel
                        {
                            // std::cout <<"not found \n";
                            std::string lol = keyIt != it->second.end() ? *keyIt : ""; 
                            channel a(id,*nameIt,*keyIt ,1);
                            channelsInServer[*nameIt] = a;
                        }
                        if (keyIt != it->second.end())
                            keyIt++;
                    }
                    std::cout << "\n";
                }
            else
            {
                sendError(id,ERR_BADCHANNELKEY,"");
            }
            // else 
            //     channel a(id,,)

            // }
        }
    }
    // else if (firstCommand == "KICK")
    // {
    //     if (commands.size () == 1 )
    //     {
    //         sendError(id ,ERR_NEEDMOREPARAMS,commands[0]);
    //         return; 
    //     }
    //     else if (commands.size () == 2)
    //     {
    //         if (check_channel_name_token (commands[1]))
    //             check_users(commands[2]);

    //     }
    //     else if (commands.size () == 3)
    //     {
    //         if (check_channel_name_token (commands[1]) && check_is_valid)
    //         {
    //             check_users(commands[2]);
    //             add_comment (command[3]);
    //         }

    //     }

    // }
    // else if (firstCommand == "MODE")
    //     check_channel_name (commands);
    // else if (firstCommand == "INVITE")
    //     check_channel_name (commands);
    else 
        sendError(id,"command not found \n","");
}
}


void Server::execute_command(std :: vector<std :: string> &commands,int id)
{
    parse_command(commands,channelsInServer,id);
    // std::cout << commands <<"\n"
//     for(size_t i = 0;i < commands.size();i++)
//     {
//         std :: cout <<">>"<< commands[i]<<"<<"<< std ::endl;
//     }
//     std :: cout << "---------------" << std :: endl;
}
