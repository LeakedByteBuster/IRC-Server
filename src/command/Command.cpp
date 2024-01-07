#include "Command.hpp"

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


void execute_commmand(std :: vector<std :: string> &commands,int id)
{
    (void) id;
    for(size_t i = 0;i < commands.size();i++)
    {
        std :: cout <<">>"<< commands[i]<<"<<"<< std ::endl;
    }
    std :: cout << "---------------" << std :: endl;
}
