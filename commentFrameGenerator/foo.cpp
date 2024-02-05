#include <iostream>
#include <iomanip>
#include <string.h>

#define MAX_SIZE    80
#define LINE_MAX_SIZE    67

using   namespace std;

const string line = "/* -------------------------------------------------------------------------- */";

void    generate(string word, size_t len)
{
    int padding = ((MAX_SIZE - len) / 2) - 2;
    int check = ((len % 2) == 0);

    cout << line << endl;
    cout << "/*";
    for(int i = 0; i < padding; i++) cout << " ";
    cout << word;
    if ((len % 2) != 0)
        padding++;
    for(int i = 0; i < padding; i++) cout << " ";
    cout << "*/" << endl;
    cout << line << endl;
    return ;
}

int main(int ac, char **av)
{
    if (ac < 2){
        printf("Error: incorrect number of arguments");
        return(1);
    }

    int i = 1;
    while(av[i]){   
        string word = av[i];
        if (word.length() > LINE_MAX_SIZE){
            printf("Error: line too long...");
            return(1);
        }
        generate(word, strlen(av[i]));
        i++;
    }
    return (0);
}
/* --------------------------------------------------------------------- */
/*                            Constructors                               */
/* --------------------------------------------------------------------- */
