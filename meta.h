#ifndef META_H
#define META_H
#include <QDateTime>
#include <QString>

#include "data.h"

class Meta
{
private:
    QDateTime dateCreated;
    QTime zoneCreated;
    char signCreated;
    size_t dateCoordCreated;
    size_t zoneCoordCreated;

    QDateTime dateModified;
    QTime zoneModified;
    char signModified;
    size_t dateCoordModified;
    size_t zoneCoordModified;

    QString tool;
    QString author;
    size_t toolCoord;
    size_t authorCoord;
    size_t fileLen;
public:
    Meta();
    Meta(Meta & other);

    //initialize class members
    void initialize(){
        dateCreated = QDateTime::fromString("");
        zoneCreated = QTime::fromString("");
        signCreated = '\0';
        dateCoordCreated = 0;
        zoneCoordCreated = 0;

        dateModified = QDateTime::fromString("");
        zoneModified = QTime::fromString("");
        signModified = '\0';
        dateCoordModified = 0;
        zoneCoordModified = 0;

        tool = "";
        author = "";
        toolCoord = 0;
        authorCoord = 0;
        fileLen = 0;


    }

    //set position of date
    void setDateCoord(size_t _position, std::string const where);

    //set offset (time zone) position
    void setZoneCoord(size_t _position, std::string const where);

    //extract date from 'content' starting at 'pos' and having 'format' type and length
    void extractDate(Data &cont, size_t pos, std::string const &format, std::string const where);

    //extract time zone offset from 'content' starting at 'pos' and having 'format' type and length
    void extractTime(Data &cont, size_t pos, std::string format, std::string const where);

    //extract author from 'content' starting at 'pos' and having 'tag'
    QString extractAuthor(Data &cont, size_t pos, std::string const &tag);

    //overloaded extract tool from 'content' starting at 'pos'
    QString extractTool(Data &cont, size_t pos);

    //overloaded extract tool from 'content' starting at 'beg', finish at 'end'
    QString extractTool(Data &cont, size_t beg, size_t end);

    //set author member
    void setAuthor(QString _author){
        this->author = _author;
    }

    //set author coordinate
    void setAuthorCoord(size_t _pos){
        this->authorCoord = _pos;
    }

    //set producer tool member
    void setTool(QString _tool){
        this->tool = _tool;
    }

    //set tool coordinate
    void setToolCoord(size_t _pos){
        this->toolCoord = _pos;
    }

    //set create Date
    void setDate(QDateTime & _dateTime, std::string const where);

    //set create Date timeZone
    void setCreateTimeZone(QTime & _time){
        this->zoneCreated = _time;
    }

    //set modified Date timeZone
    void setModifiedTimeZone(char _sign){
        this->signModified= _sign;
    }

    //set fileLengths
    void setFileLen(size_t _len){
        this->fileLen = _len;
    }

    //return date as a QDateTime format
    QDateTime getDate(std::string where);

    //return time zone in the QTime format
    QTime getZone(QString where);

    //set sign of time zone
    void setSign(char _sign, std::string const where){
        if (where == "created")
            this->signCreated = _sign;
        else
            this->signModified = _sign;
    }

    //return sign of time zone
    char getZoneSign(QString where);

    //return Date coord
    size_t getDateCoord(std::string where){
        if (where == "created")
            return this->dateCoordCreated;
        else
            return this->dateCoordModified;
    }

    //return zone coord
    size_t getZoneCoord(std::string where){
        if (where == "created")
            return this->zoneCoordCreated;
        else
            return this->zoneCoordModified;
    }

    //return author of pdf document
    QString getAuthor(){
        return this->author;
    }

    //return byte where author starts
    size_t getAuthorCoord(){
        return this->authorCoord;
    }

    //return producer tool
    QString getTool(){
        return this->tool;
    }

    //return byte where tool starts
    size_t getToolCoord(){
        return this->toolCoord;
    }

};

#endif // META_H
