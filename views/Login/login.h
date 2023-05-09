#ifndef Login_H
#define Login_H

#include "register.h"

#include <QMainWindow>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_LoginButton_clicked();
    void on_RegisterButton_clicked();

private:
    Ui::Login *ui;
    Register *r;
};

#endif
