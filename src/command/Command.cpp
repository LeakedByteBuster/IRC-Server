#include "Command.hpp"


//split command into vector of string to check it
std :: vector<std :: string> HandleIncomingMsg(std :: vector <std :: string> &commands,std :: string msg)
{
    std::stringstream   ss(msg);
    std :: string        token;
    std::string        tmp;
    
    std :: getline(ss,token,'\n');
    std::stringstream   parser(token);
        while (getline(parser,tmp,' ')) {
            commands.push_back(tmp);
        }
    return(commands);
}


// check command if it's valide and exucte it
void execute_commmand(Server *sev,std :: vector<std :: string> &commands,int id)
{
    if(!commands.empty())
    {
        std :: string first_argument = commands[0];
        std::map<int,Client>::iterator it = sev->clients.find(id);
        //int regi = it->second.isRegistred;
        if(it == sev->clients.end())
        {
            std :: cout << "No such a client\n";
        }

            if(!first_argument.compare("SENDFILE"))
            {
                send_file(sev,commands,id);
            }
            else if(!first_argument.compare("PRIVMSG"))
            {
                // send_msg(sev,commands,id);
            }
            else if(!first_argument.compare("GETFILE"))
            {
                get_file(sev,commands,id);
            }
            else if (!first_argument.compare("NICK"))
            {
                it->second.nickname = commands[1].c_str();
                // std :: cout <<" >>> "<< it->second.nickname << std::endl;
            }
            else if(!!first_argument.compare("PRVMSG"))
            {
                prv_msg(sev,commands,id);
            }
    }
    
}

//search for a client by his nickname 
int search_a_client(Server *sev,std :: string NickName)
{
    std::map<int,Client>::iterator it = sev->clients.begin();
    for(; it != sev->clients.end();it++)
    {
        if(it->second.nickname.compare(NickName) == 0)
        {
            return(it->second.fd);
        }
    }
    return(0);
}

//send func by nickname 
// SYNTAXE SENDFILE FILENAME  RECIEVER
void send_file(Server *sev,std :: vector<std :: string> & commands,int id)
{
    std :: FILE  *FileName;
    int fd = search_a_client(sev,commands[2]);
    std::map<int,Client>::iterator it = sev->clients.find(id);
    if(commands.size() < 3)
    {
        it->second.sendMsg(it->second,ERR_NEEDMOREPARAMS);
        return;
    }
    FileName = fopen(commands[1].c_str(),"rb"); 
    if(!FileName)
    {
        it->second.sendMsg(it->second,"No Such file in your /DIR");
        return;
    }
    if(!fd)
    {
        it->second.sendMsg(it->second,"No Such a client");
        return;
    }
    TFile fl(FileName,commands[1].c_str(),it->second.nickname,commands[2].c_str());
    std::map<int,Client>::iterator rec = sev->clients.find(fd);
    rec->second.Files.push_back(fl);
    
}

// void send_msg(Server *srv,std::vector<std::string> command,int id)
// {
    
// }


// SYNTAXE : GETFILE FILENAME SENDER 
void get_file(Server *srv,std :: vector<std :: string> command,int id)
{
    std::map<int,Client>::iterator it = srv->clients.find(id);
    if(command.size() < 3)
    {
        it->second.sendMsg(it->second,ERR_NEEDMOREPARAMS);
        return;
    }
    else if(command[1].empty())
    {
        it->second.sendMsg(it->second,"FILENAME NOT FOUND\n");
        return;
    }
    else if(!search_a_client(srv,command[2]))
    {
        it->second.sendMsg(it->second,"NO SUCH A CLIENT\n");
        return;
    }
    else if(it->second.Files.empty())
    {
        it->second.sendMsg(it->second,"NO SUCH A FILE TO GET IT !!!\n");
    }
    else if(!search_a_file(it->second,command[2].c_str()))
    {
        it->second.sendMsg(it->second,"NO SUCH A FILE TO GET IT FROM SENDER!!!\n");
    }
    else
    {
        // client,sender,filename
        creat_file(it->second,command[2],command[1]);
    }
    
}

int  search_a_file(Client clt,std :: string sender)
{
    std :: vector<TFile>::iterator it = clt.Files.begin();

    for(; it != clt.Files.end();it++)
    {
        if(it->getSender().compare(sender) == 0)
        {
            return(1);
        }
    }
    return(0);
}

void creat_file(Client clt,std :: string sender,std :: string filename)
{
    FILE * fd;
    std :: vector<TFile>::iterator it = clt.Files.begin();
    char line[2048];
    std::fstream myfile;

    for(; it != clt.Files.end();it++)
    {
        if(it->getSender().compare(sender) == 0)
        {
            fd = it->getstream();
        }   
    }
    myfile.open("transferd_" + filename,std::ios::binary | std::ios::out);
    if(myfile.is_open())
    {
        std :: cout << "C 'ant open file "<<std::endl;
    }
    while(std::fread(line,1,2048,fd) > 0)
    {
        std :: cout << "downlaod file";
        myfile << line;
    }
    
}

void prv_msg(Server *srv,std::vector<std :: string>command,int id)
{
    int i = 0;
    std::map<int,Client>::iterator it = srv->clients.find(id);
    if(command.size() < 3)
    {
        it->second.sendMsg(it->second,ERR_NEEDMOREPARAMS);
        return;
    }
    for(;command[i][0] != ':';i++)
    {
        if(!search_a_client(srv,command[i]))
        {
            it->second.sendMsg(it->second,command[i] + ERR_NOSUCHNICK);
        }
    }
}
