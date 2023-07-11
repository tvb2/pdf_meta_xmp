#include "data.h"

Data::Data()
{

}

void Data::erase(size_t _beg, size_t _len){
    size_t newLen = this->len - _len;
    char* temp = new char[newLen];
    size_t i = 0, j = 0;
    for (; i < this->len; ++i, ++j) {
        if (i == _beg)
            i = _beg + _len;
        temp[j] = this->contents[i];
    }
    delete[] this->contents;
    this->contents = new char[newLen];
    for (i = 0; i < newLen; ++i)
        this->contents[i] = temp[i];
    delete[] temp;
}

void Data::insert(size_t _beg, std::string _str, size_t _len){
    size_t newLen = this->len + _len;
    char *temp = new char[newLen];
    // copy contents up to _beg
    size_t i = 0;
    for (; i < _beg; ++i) {
        temp[i] = this->contents[i];
    }
    size_t j = i; // where we stopped at contents
    // append _str to temp
    for (; i < (_beg + _len); ++i) {
        temp[i] = _str[i - _beg];
    }
    // copy the rest of contents into temp
    for (; j < len; ++i, ++j)
        temp[i] = this->contents[j];
    delete[] this->contents;
    this->contents = new char[newLen];
    for (i = 0; i < newLen; ++i)
        this->contents[i] = temp[i];
    delete[] temp;
}

void Data::replace(size_t _beg, std::string _str, size_t _len){
    for (size_t i = _beg; i < (_beg + _len); ++i) {
        this->contents[i] = _str[i - _beg];
    }
}
