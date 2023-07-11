#ifndef DATA_H
#define DATA_H
#include <iostream>
#include <fstream>
#include <QDebug>
#include <string>


class Data
{
private:
    char* contents;
    size_t len;
public:
    Data();

    void init(size_t _len){
        this->len = _len;
        this->contents= new char[_len];
}

    void setContents(std::ifstream &file, size_t len)
    {
        file.read(this->contents,len);
    };

    char* getContents(){
        return this->contents;
    }

    size_t getLen(){
        return this->len;
    }

    //erase '_len' number of characters in  contents starting from position '_beg'
    void erase(size_t _beg, size_t _len);

    //insert '_str' of size '_len' starting from '_beg'
    //this effectively increases size of contents by '_len'
    void insert(size_t _beg, std::string _str, size_t _len);

    //replace '_len' bytes starting at '_beg' with '_str' data
    void replace(size_t _beg, std::string _str, size_t _len);

};

#endif // DATA_H
