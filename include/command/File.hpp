#pragma once
#include <iostream>
#include <sstream>
#include <fstream>

// file has three member sender which the name of client that send the file
// and reciever the client that recive the file
// and the filename

class file
{
    private:
    std :: string const sender;
    std :: string const receiver;
    std :: string const filename;
    std :: fstream * f;

    public :

    // constructor
    file(std :: fstream  * F,std :: string const Filename,std :: string const Sender,std :: string Receiver) :sender(Sender),receiver(Receiver),filename(Filename),f(F)
    {
    };

    //getters
    std :: string getFilname(){return filename;};
    std :: string getReceiver(){return receiver;};
    std :: string getSender(){return sender;};
    std :: fstream * getstream(){return f;};
    ~file(){};
};