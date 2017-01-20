#pragma once

#include <string>

class Message
{
private:
    //map<string, ostream*> message_streams;
    //ostream os;

public:
    Message();
    /*ostream&*/void Add_Message(const std::string& id_string);
};
