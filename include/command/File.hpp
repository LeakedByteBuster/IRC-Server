#include <iostream>
#include <sstream>

// file has three member sender which the name of client that send the file
// and reciever the client that recive the file
// and the filename

class file
{
    private:
    std :: string const sender;
    std :: string const receiver;
    std :: string const filename;

    public :

    // constructor
    file(std :: string const Filename,std :: string const Sender,std :: string Receiver) : filename(Filename),sender(Sender),receiver(Receiver){};

    //getters
    std :: string getFilname(){return filename;};
    std :: string getReceiver(){return receiver;};
    std :: string getSender(){return sender;};
    ~file();
};