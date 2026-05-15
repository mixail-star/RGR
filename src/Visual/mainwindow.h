#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_XTEA_clicked();

    void on_RS4_clicked();

    void on_Vichener_clicked();

    void on_Litorey_clicked();

    void on_Affin_shifr_clicked();

    void on_AES128_clicked();

    void on_RCA_clicked();

    void on_atbash_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
