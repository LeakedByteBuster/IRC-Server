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
                send_file(sev,commands,it->second);
            }
            else if(!first_argument.compare("GETFILE"))
            {
                get_file(sev,commands,it->second);
            }
            else if (!first_argument.compare("NICK"))
            {
                it->second.nickname = commands[1].c_str();
                // std :: cout <<" >>> "<< it->second.nickname << std::endl;
            }
            else if(!first_argument.compare("PRVMSG"))
            {
                // prv_msg(sev,commands,id);
            }
            else
            {
                it->second.sendMsg(it->second,"COMMAND NOT FOUND !!!");
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
void send_file(Server *sev,std :: vector<std :: string> & commands,Client cl)
{
    std :: FILE  *FileName;
    int fd = search_a_client(sev,commands[2]);

    if(commands.size() < 3)
    {
        cl.sendMsg(cl,ERR_NEEDMOREPARAMS);
        return;
    }
    FileName = fopen(commands[1].c_str(),"rb"); 
    if(!FileName)
    {
        cl.sendMsg(cl,"No Such file in your /DIR");
        return;
    }
    if(!fd)
    {
        cl.sendMsg(cl,"No Such a client");
        return;
    }
    TFile fl(FileName,commands[1].c_str(),cl.nickname,commands[2].c_str());
    std::map<int,Client>::iterator rec = sev->clients.find(fd);
    rec->second.Files.push_back(fl);
    if (fseek(FileName, 0, SEEK_SET) != 0) {
        perror("Error setting file position");
        fclose(FileName);
        return;
    }
    // fclose(FileName);
    
}

// void send_msg(Server *srv,std::vector<std::string> command,int id)
// {
    
// }


// SYNTAXE : GETFILE FILENAME SENDER 
void get_file(Server *srv,std :: vector<std :: string> command,Client cl)
{
    if(command.size() < 3)
    {
        cl.sendMsg(cl,ERR_NEEDMOREPARAMS);
        return;
    }
    else if(command[1].empty())
    {
        cl.sendMsg(cl,"FILENAME NOT FOUND\n");
        return;
    }
    else if(!search_a_client(srv,command[2]))
    {
        cl.sendMsg(cl,"NO SUCH A CLIENT\n");
        return;
    }
    else if(cl.Files.empty())
    {
        cl.sendMsg(cl,"NO SUCH A FILE TO GET IT !!!\n");
    }
    else if(!search_a_file(cl,command[2].c_str()))
    {
        cl.sendMsg(cl,"NO SUCH A FILE TO GET IT FROM SENDER!!!\n");
    }
    else
    {
        // client,sender,filename
        creat_file(cl,command[2],command[1]);
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
    char line[100];
    std::fstream myfile;

    for(; it != clt.Files.end();it++)
    {
        if(it->getSender().compare(sender) == 0)
        {
            fd = it->getstream();
        }   
    }

    myfile.open("transferd_" + filename,std::ios::out | std::ios::binary);
    if(!myfile.is_open() && !myfile.good())
    {
        std :: cout << "C 'ant open file "<<std::endl;
    }
    while(std::fread(line,sizeof(char),sizeof(line)-1,fd) > 0 && feof(fd))
    {
            myfile << line;
    }
    if (feof(fd)) {
        printf("\nEnd of file reached.\n");
    } else if (ferror(fd)) {
        perror("Error reading from file");
    }


    myfile.close();
    clt.Files.clear();
    
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
