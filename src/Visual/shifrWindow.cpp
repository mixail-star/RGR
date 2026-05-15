#include "shifrWindow.h"
#include "ui_shifrWindow.h"
//#include "mainwindow.h""

XTEA::XTEA(QString algorithmName, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::XTEA)
    , activeCipher(algorithmName)
{
    ui->setupUi(this);
}

XTEA::~XTEA()
{
    delete ui;
}

void XTEA::on_return_2_clicked()
{
    this->accept();
}


void XTEA::on_CoddingButton_clicked()
{
    if (activeCipher == "XTEA") {
        qDebug("123");
    }
    else     if (activeCipher == "vichener") {
        qDebug("123");
    }
    else     if (activeCipher == "RS4") {
        qDebug("123");
    }
    else     if (activeCipher == "litorey") {
        qDebug("123");
    }
    else     if (activeCipher == "affShifr") {
        qDebug("123");
    }
    else     if (activeCipher == "AES128") {
        qDebug("123");
    }
    else     if (activeCipher == "RCA") {
        qDebug("123");
    }
    else     if (activeCipher == "atbash") {
        qDebug("123");
    }
}

