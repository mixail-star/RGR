#ifndef SHIFRWINDOW_H
#define SHIFRWINDOW_H

#include <QString>
#include <QDialog>

namespace Ui {
class XTEA;
}

class XTEA : public QDialog
{
    Q_OBJECT

public:
    explicit XTEA(QString algorithmName, QWidget *parent = nullptr);
    ~XTEA();

private slots:
    void on_return_2_clicked();

private:
    Ui::XTEA *ui;
    QString activeCipher;
};

#endif // SHIFRWINDOW_H
