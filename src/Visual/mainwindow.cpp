#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "shifrWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_XTEA_clicked()
{
    XTEA window("XTEA");
    window.setModal(true);
    window.exec();
}


void MainWindow::on_RS4_clicked()
{
    XTEA window("RS4");
    window.setModal(true);
    window.exec();
}


void MainWindow::on_Vichener_clicked()
{
    XTEA window("Vichener");
    window.setModal(true);
    window.exec();
}


void MainWindow::on_Litorey_clicked()
{
    XTEA window("Litorey");
    window.setModal(true);
    window.exec();
}


void MainWindow::on_Affin_shifr_clicked()
{
    XTEA window("affShifr");
    window.setModal(true);
    window.exec();
}


void MainWindow::on_AES128_clicked()
{
    XTEA window("AES128");
    window.setModal(true);
    window.exec();
}




void MainWindow::on_RCA_clicked()
{
    XTEA window("RCA");
    window.setModal(true);
    window.exec();
}




void MainWindow::on_atbash_clicked()
{
    XTEA window("atbash");
    window.setModal(true);
    window.exec();
}




