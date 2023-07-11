#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QFileDialog>
#include <QString>
#include <QPlainTextEdit>
#include <QDir>
#include <QCheckBox>
#include <QComboBox>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <QList>
#include <QTimeZone>

#include "data.h"
#include "meta.h"

// xmp.cpp : Defines the entry point for the application.
#define WIN_ENV
//#define ENABLE_XMP_CPP_INTERFACE 1

// Must be defined to instantiate template classes
#define TXMP_STRING_TYPE std::string

// Must be defined to give access to XMPFiles
#define XMP_INCLUDE_XMPFILES 1

// Ensure XMP templates are instantiated
#include "XMP.incl_cpp"

// Provide access to the API
#include "XMP.hpp"

#include <iostream>
#include <fstream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //open file and read its contents and metadata
    void openFile(QString fileName);

    //clear all line edit forms
    void clearForms();

    // get size of 'file' in bytes
    size_t getSize(std::ifstream &file);

    /*
     * find position of 'str' in the 'input' char array of 'len' length
     * return len if not found
     */
    size_t getPos(std::string str, char const *input, size_t const &len);

    //select which format of meta data is used in the file
    void setPDFformat(size_t const &len);

private slots:
    void on_pbBrowse_clicked();

    void on_pbUpdate_clicked();

    void on_pbCancel_clicked();

private:
    QString fileName;
    Data cont;
    Meta asFound;
    std::vector<std::string> metaFormatCreate{
        "<xmp:CreateDate>",
        "/CreationDate (D:"
    };
    std::vector<std::string> metaFormatModify{
        "NONE",
        "/ModDate (D:"
    };
    std::vector<std::string> dateFormat{
        "yyyy-MM-ddTHH:mm:ss",
        "yyyyMMddHHmmss"
    };
    std::vector<std::string> offsetFormat{
        "HH:mm",
        "HH'mm"
    };
    std::vector<std::string> authorFormat{
        "NONE",
        "/Author ("
    };
    std::vector<std::string> toolFormat{
        "<xmp:CreatorTool>",
        "/Producer (",
        "</xmp:CreatorTool>"
    };

    std::map<std::string, std::string> formats;
    std::map<QString,QString> zones;

    Ui::MainWindow *ui;
    QLineEdit LEfilePath;
    QLineEdit LEcreateDate;
    QLineEdit LEmodifyDate;
    QLineEdit LEauthor;
    QLineEdit LEtool;

    QCheckBox CBcreateDate;
    QCheckBox CBmodDate;
    QCheckBox CBAuthor;
    QComboBox CBzoneCreateD;
    QComboBox CBzoneModD;

};
#endif // MAINWINDOW_H
