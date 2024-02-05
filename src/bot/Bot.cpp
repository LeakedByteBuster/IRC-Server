#include "Bot.hpp"
#include "Messages.hpp"
#include <stdlib.h>

Bot::Bot()
{
        const char* jokeArray[] = {
            "I'm not a hard drinker. I actually find it pretty easy.",
            "What's a zebra? A few sizes bigger than an A.",
            "I don't like shopping centers. Once you've seen one, you've seen the mall.",
            "What do you call a murderer with two butts? An assassin.",
            "A bossy man walked into a bar, then ordered everyone a round.",
            "I used to have an addiction to the hokey pokey, but then I turned it around.",
            "Why did the golfer cry? He was going through a rough patch.",
            "Why did the woman throw her bills out the window? She wanted to send them via airmail.",
            "What do cows do on date night? Go to the moo-vies.",
            "Did you hear about the tree that went into banking? It started its own branch.",
            "What happened to the archeologist who lost her job? Her career was in ruins.",
            "How does a lumberjack know how many trees he's cut down? He keeps a log.",
            "Why shouldn't you trust atoms? They make up everything.",
            "Did you hear about the broken guitar for sale? You can buy it with no strings attached.",
            "Did you hear about the woman who couldn't stop collecting magazines? She had issues.",
            "Two fish are in a tank. One says to the other, “Any idea how to drive this thing?”",
            "I once made a lot of money cleaning up leaves. I was raking it in.",
            "Why was the math book down in the dumps? It had a lot of problems.",
            "Why do barbers make good drivers? They know a lot of short cuts.",
            "Why did the elephant leave the circus? It was sick of working for peanuts.",
            "Why did the invisible man turn down the job offer? He couldn't see himself doing it.",
            "What's the best thing about Switzerland? I don't know, but the flag's a plus.",
            "Did you hear about the ski trip? It started off fine but went downhill fast.",
            "How do you know when a computer is on a diet? It quits eating after only one byte."
        };
        
        size_t len = sizeof(jokeArray) / sizeof(jokeArray[0]);
        jokes.insert(jokes.begin(), jokeArray, jokeArray + len);
        numJokes = len;
}

std::string     getInfos(const Client &clt)
{
    std::string ret;
    bool    t = clt.isOperator;

    ret = static_cast<std::string>("003 ");
    ret.append(clt.username + ": Nickname: " + clt.nickname + "\r\n");
    
    ret.append(static_cast<std::string>("003 ")
        + clt.username + ": Username: " + clt.username + "\r\n");
    
    ret.append(static_cast<std::string>("003 ") + clt.username);
    ret.append(": realname: " + clt.realname + "\r\n");
    
    ret.append(static_cast<std::string>("003 ") + clt.username);
    t == 1 ?    ret.append(": Operator Status: true\r\n") \
                : ret.append(": Operator Status: false\r\n");

    return (ret);
}

const std::string & Bot::getJoke()
{
    static int  i;
    if (i == numJokes)
        i = 0;
    return (jokes[i++]);
}

std::string   Bot::botExecuter(std::string buff, const Client &)
{
    std::vector<std::string>    commands(splitByLines(buff));
    std::vector<std::string>    tokens;
    std::string                 ret;
    int                         res = 0;
    Bot b;

    if (commands.size() >= 1) {
        for (unsigned long i = 0; i < commands.size(); i++) {
            tokens = splitBySpace(commands[i]);
            if (!tokens.empty()) {
                res =     (tokens[0].compare("/date") == 0)     * 1 \
                        + (tokens[0].compare("/DATE") == 0)     * 1 \
                        + (tokens[0].compare("/joke") == 0)    * 2 \
                        + (tokens[0].compare("/JOKE") == 0)    * 2 \
                        + (tokens[0].compare("/whoami") == 0)   * 3 \
                        + (tokens[0].compare("/WHOAMI") == 0)   * 3;

                switch (res)
                {
                // case 1:

                //     ret = static_cast<std::string>("003: ");
                //     ret.append(geTime());
                //     break ;

                // case 2:
                //     ret = static_cast<std::string>("003: ");
                //     ret.append(b.getJoke());
                //     break ;

                // case 3:
                //     ret.append(getInfos(clt));
                //     break ;

                // default:
                //     ret = _ERR(clt.nickname, ERR_UNKNOWNCOMMAND);
                //     break ;
                }
            }
        }
    }
    return (ret);
}
