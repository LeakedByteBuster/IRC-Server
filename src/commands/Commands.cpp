#include "Commands.hpp"




int parse_name (const std::string &  buff)
{
    std::cout << "buff1 : " << buff[1];
    return 0;
}

void join (std::string buff)
{
    std::cout<< "here\n";
    if (buff.empty ())
        std::cout << "Error \n";
    if (buff.size() == 2)
        parse_name ("buff");
}
void kick (const std::string & buff)
{
    if (buff.empty())
        std::cout << "ERROR" << "\n";

}
void execute_command(const std::string & buff)
{
    std::cout << buff << "\n";
    std::cout << buff.compare("kick") << "\n";
        // join (buff);
    if (buff.compare("kick")== 0)
        kick (buff);

}