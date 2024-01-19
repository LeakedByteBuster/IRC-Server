#include "Bot.hpp"
#include "Errors.hpp"
#include<stdlib.h>

static std::string itoa(int i)
{
    std::string ret;
    std::string tmp;
    std::string p = "0123456789";

    if (i > 10) {
        while ((i = (i / 10))) {
            tmp = p[i];
            ret.append(tmp);
        }
    } else {
        ret = p[i];
    }
    return (ret);
}

std::string     getInfos(const Client &clt)
{
    std::string ret;
    bool    t = clt.isOperator;

    ret = "Nickname: " + clt.nickname;
    ret.append("\nUsername: " + clt.username);
    t == 1 ?    ret.append("\nOperator Status: true") \
                : ret.append("\nOperator Status: false");
    ret.append("\nrealname: " + clt.realname);
    ret.append("\nID: " + itoa(clt.fd));

    return (ret);
}

std::string   Bot::botExecuter(std::string buff, const Client &clt)
{
    std::vector<std::string>    commands(splitByLines(buff));
    std::vector<std::string>    tokens;
    std::string                 ret;
    int                         res = 0;

    if (commands.size() >= 1) {
        for (unsigned long i = 0; i < commands.size(); i++) {
            tokens = splitBySpace(commands[i]);
            if (!tokens.empty()) {
                res =     (tokens[0].compare("/date") == 0)     * 1
                        + (tokens[0].compare("/DATE") == 0)     * 1
                        + (tokens[0].compare("/jokes") == 0)    * 2
                        + (tokens[0].compare("/JOKES") == 0)    * 2
                        + (tokens[0].compare("/whoami") == 0)   * 3;
                switch (res)
                {
                case 1:
                    ret = geTime();
                    break ;

                case 2:
                    break ;

                case 3:
                    ret = getInfos(clt);
                    break ;

                default:
                    ret = LogError::getError(clt.nickname, LogError::ERR_UNKNOWNCOMMAND);
                    break ;
                }
            }
        }
    }
    return (ret);
}
