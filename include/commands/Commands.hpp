#pragma once 


#include<iostream>
#include<map>
#include <unordered_set>
class channels
{
private:
    std::string name;
    std::string topic;
    // std::map <int ,Client> Clients;
    int Pass;
    std::string operators;

public:
    channels(std::string channel_name, int key);
    ~channels();
    void setTopic(std::string & topic);

};
int parse_name (const std::string &  buff);
void join (std::string buff);
void execute_command(const std::string & buff);