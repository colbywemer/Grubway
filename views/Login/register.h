#ifndef REGISTER_H
#define REGISTER_H

#include <AdminDashboard>
#include <Database>

#include <QCloseEvent>
#include <QMainWindow>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class Register; }
QT_END_NAMESPACE

class Register : public QMainWindow
{
    Q_OBJECT

public:
    Register(QWidget *parent = nullptr);
    ~Register();

    void noReturnToLogin();
    void setAdminDash(AdminDashboard *ad);
    void setFirstRun();
    void setAdminAccount(Account account);

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_RegisterButton_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::Register *ui;
    AdminDashboard *parent_widget = nullptr;
    Account leaderAccount;

    bool goToLogin = true, firstRun = false;
};

#endif // REGISTER_H
