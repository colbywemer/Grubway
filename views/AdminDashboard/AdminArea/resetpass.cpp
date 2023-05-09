#include "resetpass.h"
#include "ui_resetpass.h"

#include <QMessageBox>

ResetPass::ResetPass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResetPass)
{
    ui->setupUi(this);
}

ResetPass::~ResetPass()
{
    delete ui;
}

void ResetPass::setAdminDash(QWidget *ad)
{
    parent_widget = ad;
}
void ResetPass::setAdminAccount(Account account){
    adminAccount = account;
}

void ResetPass::setUsername(QString username)
{
    account_object = selectAccountByUsername(username);

    setWindowTitle("Change Password for: " + username);
}

void ResetPass::on_CancelBtn_clicked()
{
    close();
}

void ResetPass::on_OKBtn_clicked()
{
    QMessageBox MsgBox;

    QString ChangePass = ui->ChangePassLine->text(),
        ConfirmPass = ui->ConfirmChgPassLine->text();

    if (ChangePass != "" && ConfirmPass != "")
    {
        if (ChangePass == ConfirmPass)
        {
            account_object.password = ChangePass;

            updateAccount(account_object);
            Logs log;
            log.time = QDateTime::currentDateTime();
            log.account = adminAccount;
            log.action = "Changed Password For '" + account_object.username + "'";
            log.type = "Account";
            createLog(log);
            MsgBox.information(this, "Password Changed", "Password successfully changed.");
            MsgBox.show();

            parent_widget->show();

            close();
        }

        else
        {
            MsgBox.critical(this, "Invalid Input", "Password fields do not match.");
            MsgBox.show();
        }
    }

    else
    {
        MsgBox.critical(this, "Missing Fields", "Fields cannot be empty.");
        MsgBox.show();
    }
}

void ResetPass::closeEvent(QCloseEvent *event)
{
    parent_widget->show();

    event->accept();
}

