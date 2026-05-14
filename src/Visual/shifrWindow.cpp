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

