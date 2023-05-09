#ifndef RESETPASS_H
#define RESETPASS_H

#include <AdminDashboard>
#include <Database>

#include <QCloseEvent>
#include <QWidget>

namespace Ui {
class ResetPass;
}

class ResetPass : public QWidget
{
    Q_OBJECT

public:
    explicit ResetPass(QWidget *parent = nullptr);
    ~ResetPass();

    void setAdminDash(QWidget *ad);
    void setUsername(QString username);
    void setAdminAccount(Account account);

private slots:
    void on_CancelBtn_clicked();
    void on_OKBtn_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::ResetPass *ui;
    Account account_object;
    Account adminAccount;
    QWidget *parent_widget = nullptr;
};

#endif // RESETPASS_H
