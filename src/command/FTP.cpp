#include "Server.hpp"
#include "FTP.hpp"
#include "Errors.hpp"

// search for a client by his nickname
int search_a_client(std::map<int,Client> clients, std ::string NickName)
{
    if(clients.empty())
    {
        return(0);
    }
        std::map<int, Client>::iterator it = clients.begin();
        for (; it != clients.end(); it++)
        {
            if (it->second.nickname.compare(NickName) == 0)
            {
                return (it->second.fd);
            }
        }
    return (0);
}

int search_a_file(Client clt, std ::string sender)
{
    std ::vector<TFile>::iterator it = clt.Files.begin();

    for (; it != clt.Files.end(); it++)
    {
        if (it->getSender().compare(sender) == 0)
        {
            return (1);
        }
    }
    return (0);
}

void creat_file(Client clt, std ::string sender, std ::string filename)
{
    int file_size;
    char *line = NULL;
    FILE *fd;
    std ::vector<TFile>::iterator it = clt.Files.begin();
    std::fstream myfile;

    for (; it != clt.Files.end(); it++)
    {
        if (it->getSender().compare(sender) == 0)
        {
            fd = it->getstream();
        }
    }
    // determine size of file
    int prev = ftell(fd);
    std::fseek(fd, 0, SEEK_END);
    file_size = ftell(fd);
    std::fseek(fd, prev, SEEK_SET);
        line = new(std::nothrow)char[file_size];
    if(!line)
    {
        clt.sendMsg(clt, "I think the file is too big can you be kind with us :)");
    }
    // open the new file in client /dir
    myfile.open("transferd_" + filename, std::ios::out | std::ios::binary);
    if (myfile.is_open() == 0)
    {
        clt.sendMsg(clt, "C 'ant open file ");
        myfile.close();
        clt.Files.clear();
        clt.Files.clear();
    }
    // read from sender file
    int readbytes = fread(line, 1, file_size, fd);
    if (readbytes == -1)
    {
        clt.sendMsg(clt, "C'ant read from file");
        myfile.close();
        clt.Files.clear();
    }
    myfile.write(line, file_size);
    myfile.close();
    clt.Files.clear();
}

// SYNTAXE SENDFILE FILENAME  RECIEVER
void send_file(std::map<int,Client> &clients, std ::vector<std ::string> &commands, Client &cl)
{
    std ::FILE *FileName;

    if (commands.size() < 3)
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NEEDMOREPARAM));
        return;
    }
    // open file both binary and text
    FileName = fopen(commands[1].c_str(), "rb");
    if (!FileName)
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NOSUCHFILE));
        return;
    }
    // if not found reciever
    int fd = search_a_client(clients, commands[2]);
    if (!fd)
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NOSUCHNICK));
        return;
    }
    // creat object file and push it in client vector of files
    // fl(FILE *,FILNAME,SENDER,RECEIVER);
    TFile fl(FileName, commands[1].c_str(), cl.nickname, commands[2].c_str());
    std::map<int, Client>::iterator rec = clients.find(fd);
    rec->second.Files.push_back(fl);
    cl.sendMsg(cl, getDownMsg());
}

// SYNTAXE : GETFILE FILENAME SENDER
void get_file(std::map<int,Client> &clients, std ::vector<std ::string> &command, Client &cl)
{
    if (command.size() != 3)
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NEEDMOREPARAM));
        return;
    }
    else if (command[1].empty())
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NOSUCHFILENAME));
        return;
    }
    // if c'ant find the sender of file
    else if (!search_a_client(clients, command[2]))
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NOSUCHNICK));
        return;
    }
    //if there is no files in client vector files
    else if (cl.Files.empty())
    {
        // std::cout << "reciever :"<<cl.fd<<std::endl;
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NOSUCHFILENAME));
        return;
    }
    // if there is no file from sender
    else if (!search_a_file(cl, command[2].c_str()))
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NOFILEFROMSENDER));
        return;
    }
    else
    {
        // creat file in reciever /dir
        creat_file(cl, command[2], command[1]);
    }
}