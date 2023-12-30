#include "Client.hpp"

Client::Client()
{
    std::cout << "Error [Client class]: use parameterized constructor" << std::endl;
}

Client::Client(std::string , std::string )
{

}

Client& Client::operator=(Client &)
{
    return (*this);
}

Client::~Client()
{

}
