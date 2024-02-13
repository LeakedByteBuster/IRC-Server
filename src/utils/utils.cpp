#include "utils.hpp"

/* splits the buff, '\n' is the delim */
std::vector<std::string>    splitByLines(std::string buff)
{
    std::stringstream   ss(buff);
    std::string         line;
    std::vector<std::string> strings;

    while (std::getline(ss, line)) {
        if (!line.empty())
            strings.push_back(line);
    }
    return (strings);
}

std::vector<std::string> splitBySpace(std::string str)
{
    std::stringstream           ss(str);
    std::vector<std::string>    tokens;
    std::string                 token;

    while (ss >> token) {
        if (!token.empty())
            tokens.push_back(token);
    }
    return (tokens);
}

std::vector<std::string> splitByValue(std::string s, char c)
{
    std::cout.flush();
    std::stringstream           ss(s);
    std::vector<std::string>    tokens;
    std::string                 token;

    while (std::getline(ss, token, c)) {
        if (!token.empty())
            tokens.push_back(token);
    }
    return (tokens);
}

//  Returns local current time
std::string  geTime()
{
    time_t  t;
    time(&t);
    std::string currentTime = asctime(localtime(&t));
    currentTime.pop_back(); // removes '\n' character from the string
    return (currentTime);
}

const char *    getBigMsg()
{
        const char    *bigMsg =

  "\n$$$$$$\\ $$$$$$$\\   $$$$$$\\  \n"
        "\\_$$  _|$$  __$$\\ $$  __$$\\  \n"
        "  $$ |  $$ |  $$ |$$ /   \n"
        "  $$ |  $$$$$$$  |$$ | \n"
        "  $$ |  $$  __$$< $$ |  \n"
        "  $$ |  $$ |  $$ |$$ | \n"
        "$$$$$$\\ $$ |  $$ |\\$$$$$$  |     \n"
        "\\______|\\__|  \\__| \\______/   \n";
    return (bigMsg);
}
