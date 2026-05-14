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
    hide();
    window.setModal(true);
    window.exec();
}

