#ifndef EDITUSER_H
#define EDITUSER_H

#include <AdminDashboard>
#include <CashierScreen>
#include <Database>

#include <QCloseEvent>
#include <QWidget>

namespace Ui {
class EditUser;
}

class EditUser : public QWidget
{
    Q_OBJECT

public:
    explicit EditUser(QWidget *parent = nullptr);
    ~EditUser();

    void editingCurrentUser();
    void setAdminDash(AdminDashboard *ad);
    void setCashier(CashierScreen *cs);
    void setUsername(QString username);
    void setAdminAccount(Account account);

private slots:
    void on_CancelBtn_clicked();
    void on_OKBtn_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::EditUser *ui;
    Account account_object;
    Account adminAccount;

    AdminDashboard *parent_widget = nullptr;
    CashierScreen *parent_widget2 = nullptr;

    QString old_name, old_firstname, old_lastname, old_phone, old_address, old_email, old_role;

    bool currentUser = false;
};

#endif // EDITUSER_H
