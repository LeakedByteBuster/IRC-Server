#include "Bot.hpp"

std::string   Bot::executer(std::string buff)
{
    std::vector<std::string>    commands(splitByLines(buff));
    std::vector<std::string>    tokens;
    int                         res = 0;

    if (commands.size() > 1) {
        for (unsigned long i = 0; i < commands.size(); i++) {
            tokens = splitBySpace(commands[i]);
            if (!tokens.empty()) {
                res =     (tokens[0].compare("/time") == 0)     * 1
                        + (tokens[0].compare("/TIME") == 0)     * 1
                        + (tokens[0].compare("/jokes") == 0)    * 2
                        + (tokens[0].compare("/JOKES") == 0)    * 2;
                switch (res)
                {
                case 1:

                    break ;

                case 2:
                    break ;

                case 3:
                    break ;

                default:
                    std::cout << "Error : " << tokens[0] << " command not found" << std::endl;
                    std::cout.flush();
                    break ;
                }
            }
        }
    }
    return ("");
}
