#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //create a drop-down list of available time zones in format "UTC+00:00"
    //use a map zones(key = selection, value = text in format "+00:00")
    //to match selection with string to be appended to updated date and timezone
    QList<QByteArray> timeZoneIds  = QTimeZone::availableTimeZoneIds();
    for (int i = 0; i < timeZoneIds.size(); ++i){
        if (timeZoneIds[i].contains("UTC") && !timeZoneIds[i].contains("Etc") && !(timeZoneIds[i].size()<=3)){
            QString current = QString::fromStdString(timeZoneIds[i].toStdString().substr(3,6));
            this->zones[current] = current;
            ui->CBzoneCreateD->addItem(current);
            ui->CBzoneModD->addItem(current);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

size_t MainWindow::getSize(std::ifstream &file) {
    size_t i = 0;
    file.seekg (0, file.end);
    i = file.tellg();
    file.seekg (0, file.beg);
    return i;
}

size_t MainWindow::getPos(std::string str, char const *input, size_t const &len){
    if (str == "NONE")
        return len;
    std::string tag;
    size_t length = str.length();
    tag.resize(length);

    size_t pos = len - 1;
    bool found = true;
    while (pos > 0){
        found = true;
        for (int i = pos; i >= 0 && (pos - i) < str.length();){
            if (input[i] != str[length - (pos - i) - 1  ]){
                found = false;
                break;
            }
            --i;
        }
        if (found){
            return pos+1;
        }
        --pos;
    }
    return len;
}

void MainWindow::setPDFformat(size_t const &len){
    for (size_t i = 0; i < this->metaFormatCreate.size(); ++i){
        if (getPos(this->metaFormatCreate[i], this->cont.getContents(),len) != len){
            this->formats["created"] = this->metaFormatCreate[i];
            this->formats["modified"] = this->metaFormatModify[i];
            this->formats["date"] = this->dateFormat[i];
            this->formats["zone"] = this->offsetFormat[i];
            this->formats["author"] = this->authorFormat[i];
            this->formats["tool"] = this->toolFormat[i];
            return;
        }
    }
    qDebug() << "something is wrong in meta data formats, refer to setPDFformat()";
}

void MainWindow::openFile(QString fileName){
    //create file stream to use for meta data extraction
    std::ifstream file(fileName.toStdString(), std::ios::binary);
    this->asFound.initialize();
        //get number of bytes in the file
        int len = getSize(file);
            qDebug() << "file len = " << len;

        //get file contents as a binary array
            this->cont.init(len);
            this->cont.setContents(file, len);
            asFound.setFileLen(len);
    file.close();

//pending: move code below to separate function
        //depending of pdf version, date format will differ find relevant format and extract it
        setPDFformat(len);
            qDebug() << "time: " << formats["zone"] << ", date: " << formats["date"] << ", offset: " << ", author: " << formats["author"] << ", tool: " << formats["tool"];
        std::vector<std::string> fetch {"created", "modified"};
    //created and Modified date & timezone
        for (size_t i = 0; i < fetch.size(); ++i){
            size_t pos = getPos(this->formats[fetch[i]],this->cont.getContents(),len);
            asFound.setDateCoord(pos, fetch[i]);
            asFound.setZoneCoord(pos + this->formats["date"].length() + 1, fetch[i]);
            asFound.extractDate(this->cont, pos, this->formats["date"], fetch[i]);
            asFound.extractTime(this->cont, asFound.getZoneCoord(fetch[i]), this->formats["zone"], fetch[i]);
        }

    //extract author
        size_t authorPos = getPos(this->formats["author"],this->cont.getContents(),len);
        asFound.setAuthorCoord(authorPos);
        asFound.setAuthor(asFound.extractAuthor(this->cont, authorPos ,this->formats["author"]));

    //extract tool
        size_t toolPos = getPos(this->formats["tool"],this->cont.getContents(),len);
        asFound.setToolCoord(toolPos);
            if (this->formats["tool"] == this->toolFormat[0]){//old-style meta
                size_t toolEnd = getPos(this->toolFormat[2], this->cont.getContents(),len) - this->toolFormat[2].length();//find end of tool name
                asFound.setTool(asFound.extractTool(this->cont, toolPos , toolEnd));
            }
            else//new style meta
                asFound.setTool(asFound.extractTool(this->cont, toolPos));
}

void MainWindow::clearForms(){
            ui->LEcreateDate->clear();
            ui->LEmodifyDate->clear();
            ui->LEauthor->clear();
            ui->LEtool->clear();
}

void MainWindow::on_pbBrowse_clicked()
{
            SXMPMeta::Initialize();
            SXMPFiles::Initialize();
            SXMPFiles myFile;
            std::string filename = "C:\\Users\\bogda\\OneDrive\\Documents\\tvb2\\230131-Tverdoi-employment-cert.pdf";
            myFile.OpenFile(filename, kXMP_UnknownFile, kXMPFiles_OpenForUpdate);
            SXMPMeta meta;
            std::string packet;
            XMP_PacketInfo info;
            myFile.GetXMP(&meta, &packet, &info);

            bool exists = true;
            // Read a simple property
            std::string simpleValue;  //Stores the value for the property
            exists = meta.GetProperty(kXMP_NS_XMP, "CreatorTool", &simpleValue, NULL);
            if (exists)
                qDebug() << "CreatorTool = " << simpleValue;
            else
                simpleValue.clear();


            // Get dc:MetadataDate
            XMP_DateTime myDate;
            if (meta.GetProperty_Date(kXMP_NS_XMP, "CreateDate", &myDate, NULL))
            {
                // Convert the date struct into a convenient string and display it
                std::string myDateStr;
                SXMPUtils::ConvertFromDate(myDate, &myDateStr);
                qDebug() << "meta:MetadataDate = " << myDateStr;
            }
            else
                qDebug() << "no date found\n";

            myFile.CloseFile();
            SXMPFiles::Terminate();
            SXMPMeta::Terminate();


            this->fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                  QDir::currentPath(),
                                                  tr("Adobe (*.pdf)"));
    ui->LEfilePath->setText(this->fileName);

    openFile(this->fileName);

    clearForms();

//fill in created date, time and time zone
    if (this->asFound.getDate("created").isValid()){
        ui->LEcreateDate->setText(
            this->asFound.getDate("created").toString() +
            ", UTC" +
            this->asFound.getZoneSign("created") +
            this->asFound.getZone("created").toString());
        //synchronize date/time selection with created date
        ui->DTcreateDate->setDateTime(this->asFound.getDate("created"));
    }
    else{
            ui->LEcreateDate->setText("date is not valid");
            ui->DTcreateDate->setDateTime(QDateTime::currentDateTime());
    }
    //set drop-down box for time zone to local time zone (create date)
    QString offset = QTimeZone::systemTimeZone().comment().mid(4,6);
    ui->CBzoneCreateD->setCurrentText(offset);

//fill in modified date, time and time zone
    if (this->asFound.getDate("modified").isValid()){
        ui->LEmodifyDate->setText(
            this->asFound.getDate("modified").toString()+
            ", UTC" +
            this->asFound.getZoneSign("modified") +
            this->asFound.getZone("modified").toString());
        //synchronize date/time selection with 'modified' date
        ui->DTmodDate->setDateTime(this->asFound.getDate("modified"));
    }
    else{
        ui->LEmodifyDate->setText("date is not valid");
        ui->DTmodDate->setDateTime(QDateTime::currentDateTime());
    }
    //set drop-down box for time zone to local time zone ('modified' date)
    ui->CBzoneModD->setCurrentText(offset);

    ui->LEauthor->setText(this->asFound.getAuthor());
    ui->LEtool->setText(this->asFound.getTool());
}

void MainWindow::on_pbUpdate_clicked()
{
    Meta asLeft(asFound);

//create file stream to use for meta data update
//use new file to prevent data loss in the original file
        std::string name = ui->LEfilePath->text().toStdString();
    name.append("_updated.pdf");
    std::fstream file(name,
            std::ios::binary |
            std::ios::out);
    if (!file.is_open())
        qDebug() << "file not open!";

//update Tool if checkbox is checked
//first delete old tool then insert new one
if (ui->CBTool->isChecked()){
        //update coordinate as it could have changed
        size_t toolPos = getPos(this->formats["tool"],this->cont.getContents(),this->cont.getLen());
        asLeft.setToolCoord(toolPos);
        asLeft.setTool(ui->LEtool->text());
    this->cont.erase(toolPos, this->asFound.getTool().length());
    this->cont.insert(
        toolPos,
        asLeft.getTool().toStdString(),
        asLeft.getTool().length());
}

//update Author if checkbox is checked
//first delete old author then insert new one
    if (ui->CBAuthor->isChecked()){
        //update coordinate as it could have changed
        size_t authPos = getPos(this->formats["author"],this->cont.getContents(),this->cont.getLen());
        asLeft.setAuthorCoord(authPos);
        asLeft.setAuthor(ui->LEauthor->text());
        this->cont.erase(authPos, this->asFound.getAuthor().length());
        this->cont.insert(
            authPos,
            asLeft.getAuthor().toStdString(),
            asLeft.getAuthor().length());
    }

//update created date
    std::vector<std::string> update;

    if (ui->CBcreateDate->isChecked()){
        update.emplace_back("created");
    }
    if (ui->CBmodDate->isChecked()){
            update.emplace_back("modified");

    }
    if (update.size() != 0){
            for (size_t i = 0; i < update.size(); ++i){
                size_t datePos = getPos(this->formats[update[i]],this->cont.getContents(),this->cont.getLen());
                asLeft.setDateCoord(datePos, update[i]);
                QDateTime date;
                if (update[i] == "created")
                    date = ui->DTcreateDate->dateTime();
                else
                    date = ui->DTmodDate->dateTime();
                asLeft.setDate(date,update[i]);

                std::string newZone;
                if (update[i] == "created")
                    newZone = ui->CBzoneCreateD->currentText().toStdString();
                else
                    newZone = ui->CBzoneModD->currentText().toStdString();

                if (this->formats["zone"][2] == '\'')
                    newZone[3] = '\'';
                qDebug() << "new " << update[i] << " time zone: " << newZone;
                std::string newDate =
                    asLeft.getDate(update[i]).toString(QString::fromStdString(this->formats["date"])).toStdString() +
                    newZone;
                qDebug() << "new date: " << newDate;
                this->cont.replace(asLeft.getDateCoord(update[i]), newDate, newDate.length());
            }
    }

    //write all changes to the file
    file.write(this->cont.getContents(),this->cont.getLen());
    file.close();
}

void MainWindow::on_pbCancel_clicked()
{
    MainWindow::~MainWindow();
}

