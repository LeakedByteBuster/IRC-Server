#pragma once 

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include "TFile.hpp"
#include "Messages.hpp"
#include "channel.hpp"
#include <utility>

#define MSG_MAX 512


class Server;

enum    Commands {
    UNKNOWN,
    SENDFILE,
    GETFILE,
    NICK,
    PASS_USER,
    PRVMSG,
    PONG,
    IRCBOT,
    DATE,
    JOKE,
    WHOAMI,
    JOIN,
    MODE,
    KICK,
    TOPIC,
    INVITE
};

// compare first string in vector too see if is a valid command
void                        execute_commmand(std::map<int,Client> &clients, std ::vector<std ::string> &commands, int id);
void                        join(Client &clt, std::vector<std::string> &command);

//  FTP
void                        send_file(std::map<int,Client> &clients,std :: vector<std :: string> & commands,Client &cl);
void                        get_file(std::map<int,Client> &clients,std :: vector<std :: string> &command,Client &cl);
int                         search_a_file(Client clt,std :: string sender);
void                        creat_file(Client clt,std :: string sender,std :: string filename);
const char *                getDownMsg(void);

// PRVMSG 
void                       prv_msg(std::vector<std :: string>command,const Client &clt ,std::map<int,Client> Clients);
void                       check_targets(std::vector<std::string>commmand,std::vector<std::string> client_and_channels,const Client &clt,std::map<int,Client> clients);
void                       sendPrvmsg(const Client &sender,std::string msg,Client recv);
int                        check_text_msg(std ::string msg);
std::vector<std::string>   parse_such(std::string str);
std :: string              compile_msg(std::vector<std::string> commands,int position);
int                        check_channel(std::map<std::string,Channel> channles_server,std::string channel_name,const Client &clt);
int                        search_a_client(std::map<int,Client> clients, std ::string NickName);
int                        is_client_in_channel (std::string & name,std::map<std::string,Channel> &channelsInServer,std::string cltname);
int                        check_existed_channel (std::map<std::string,Channel> &channelsInServer , std::string name);
void                       Detrm_Dest_Msg(Channel ChnlDest ,  const Client & except , std::string Msg , bool Operator);
std::string                ChnlReply(const Client &sender , const Channel &recv ,  const std::string &msg);


