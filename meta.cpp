#include "meta.h"

Meta::Meta()
{

}

Meta::Meta(Meta & other){
    this->dateCreated = other.dateCreated;
    this->zoneCreated = other.zoneCreated;
    this->signCreated = other.signCreated;
    this->dateCoordCreated = other.dateCoordCreated;
    this->zoneCoordCreated = other.zoneCoordCreated;

    this->dateModified = other.dateModified;
    this->zoneModified = other.zoneModified;
    this->signModified = other.signModified;
    this->dateCoordModified = other.dateCoordModified;
    this->zoneCoordModified = other.zoneCoordModified;

    this->tool = other.tool;
    this->author = other.author;
    this->toolCoord = other.toolCoord;
    this->authorCoord = other.authorCoord;
    this->fileLen = other.fileLen;
}

void Meta::setDateCoord(size_t _position, std::string const where){
    if (_position > this->fileLen)
        _position = this->fileLen;
    if (where == "created")
        this->dateCoordCreated = _position;
    else
        this->dateCoordModified = _position;
};

void Meta::setZoneCoord(size_t _position, std::string const where){
    if (_position > this->fileLen)
        _position = this->fileLen;
    if (where == "created")
        this->zoneCoordCreated = _position;
    else
        this->zoneCoordModified = _position;
}

void Meta::extractDate(Data &cont, size_t pos, std::string const &format, std::string const where){
    if (pos == this->fileLen || where == "NONE")
        return;
    QString temp(QString::fromStdString(format));
    size_t init = pos;
    for (size_t i = pos; (i - init) < temp.length(); ++i){
        temp[i - init] = cont.getContents()[i];
    }
    //convert date from string to date/time format
    if (where == "created"){
        this->dateCreated = QDateTime::fromString(temp, QString::fromStdString(format));
        qDebug()<<"date created: " << this->dateCreated;
    }
    else{
        this->dateModified= QDateTime::fromString(temp, QString::fromStdString(format));
        qDebug()<<"date modified: " << this->dateModified;
    }
    return;
}

void Meta::extractTime(Data &cont, size_t pos, std::string format, std::string const where){
    if (pos == this->fileLen || where == "NONE")
        return;
    QString temp(QString::fromStdString(format));
    size_t init = pos;
    for (size_t i = pos; (i - init) < temp.length(); ++i){
        char t = cont.getContents()[i];
        if (t == '\''){// '
            t = ':';
            format[i - init] = t;
        }
        temp[i - init] = t;
    }
    //convert date from string to date/time format
    if (where == "created"){
        this->zoneCreated = QTime::fromString(temp, QString::fromStdString(format));
        qDebug()<<"time zone created: " << this->zoneCreated;
    }
    else{
        this->zoneModified = QTime::fromString(temp, QString::fromStdString(format));
        qDebug()<<"time zone modified: " << this->zoneModified;
    }
    setSign(cont.getContents()[pos - 1], where);
    return;
}

QString Meta::extractAuthor(Data &cont, size_t pos, std::string const &tag){
    if (pos == this->fileLen || tag == "NONE")
        return "";
    QString temp = "";
    size_t init = pos;
    size_t i = pos;
    char t = cont.getContents()[i];
    //condition potentially prone to errors if field contains combination of ')/' or ')\n'
    while( !(t == ')' && (cont.getContents()[i+1] == '\n' || cont.getContents()[i+1] == '/'))){
        temp.resize(temp.length()+1);
        temp[i - init] = t;
        ++i;
        t = cont.getContents()[i];
    }
    qDebug() << "text: " << temp;
    return temp;
}

QString Meta::extractTool(Data &cont, size_t pos){
    if (pos == this->fileLen)
        return "";
    QString temp = "";
    size_t init = pos;
    size_t i = pos;
    char t = cont.getContents()[i];

    //condition potentially prone to errors if field contains combination of ')/' or ')\n'
    while( !(t == ')' && (cont.getContents()[i+1] == '\n' || cont.getContents()[i+1] == '/'))){
        temp.resize(temp.length()+1);
        temp[i - init] = t;
        ++i;
        t = cont.getContents()[i];
    }
    qDebug() << "text: " << temp;
    return temp;
}

QString Meta::extractTool(Data &cont, size_t beg, size_t end){
    if (beg == this->fileLen)
        return "";
    size_t len = end - beg;
    QString temp(len,' ');
    for (size_t i = beg; i < end; ++i){
        temp[i - beg] = cont.getContents()[i];
    }
    qDebug() << "tool: " << temp;
    return temp;
}

void Meta::setDate(QDateTime & _dateTime, std::string const where){
    if (where == "created")
        this->dateCreated = _dateTime;
    else
        this->dateModified = _dateTime;
}

QDateTime Meta::getDate(std::string where){
    if (where == "created")
        return this->dateCreated;
    else
        return this->dateModified;
}

QTime Meta::getZone(QString where){
    if (where == "created")
        return this->zoneCreated;
    else
        return this->zoneModified;
}

char Meta::getZoneSign(QString where){
    if (where == "created")
        return this->signCreated;
    else
        return this->signModified;
}
