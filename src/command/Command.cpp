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


// void check_if_exist(std::vector<std::string> &channels)
// {
//     std::find()

// }


void parse_command(std::vector<std::string> & commands,int id)
{
    (void)id;
    std::map<std::vector<std::string>,std::vector<std::string> > channel_keys;
    if (!commands.empty())
    {
        if (commands.front() == "JOIN" || commands.front() == "join")
        {
            channel_keys = parse_join_command(commands);
        // if (!channel_keys.empty ())
        // {
        //     if (check_if_exist(channel_keys.first()));

        // }
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
}
void Server::execute_commmand(std :: vector<std :: string> &commands,int id)
{
    // (void) id ;
    // std::cout << commands <<"\n"
    parse_command(commands,id);
    // for(size_t i = 0;i < commands.size();i++)
    // {
    //     std :: cout <<">>"<< commands[i]<<"<<"<< std ::endl;
    // }
    // std :: cout << "---------------" << std :: endl;
}
