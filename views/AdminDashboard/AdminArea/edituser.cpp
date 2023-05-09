#include "edituser.h"
#include "ui_edituser.h"

#include <QMessageBox>

EditUser::EditUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditUser)
{
    ui->setupUi(this);

    ui->RoleBox->addItem("Administrator");
    ui->RoleBox->addItem("Manager");
    ui->RoleBox->addItem("Cashier");
}

EditUser::~EditUser()
{
    delete ui;
}

// Only to be called when editing the current logged-in user
void EditUser::editingCurrentUser()
{
    ui->RoleBox->setEnabled(false);

    currentUser = true;
}

void EditUser::setAdminDash(AdminDashboard *ad)
{
    parent_widget = ad;
}

void EditUser::setCashier(CashierScreen *cs)
{
    parent_widget2 = cs;
}

void EditUser::setAdminAccount(Account account){

    adminAccount = account;
}

void EditUser::setUsername(QString username)
{
    account_object = selectAccountByUsername(username);

    setWindowTitle("Edit User Information for: " + username);

    old_name = username;
    old_firstname = account_object.firstName;
    old_lastname = account_object.lastName;
    old_email = account_object.email;
    old_address = account_object.address;
    old_phone = account_object.phoneNumber;
    old_role = account_object.position;

    ui->FirstnameLine->setText(account_object.firstName);
    ui->LastnameLine->setText(account_object.lastName);
    ui->UsernameLine->setText(username);
    ui->AddressLine->setText(account_object.address);
    ui->PhoneLine->setText(account_object.phoneNumber);
    ui->EmailLine->setText(account_object.email);
    ui->RoleBox->setCurrentText(account_object.position);
}

void EditUser::on_CancelBtn_clicked()
{
    close();
}

void EditUser::on_OKBtn_clicked()
{
    QMessageBox MsgBox;

    QString address_input = ui->AddressLine->text(),
        email_input = ui->EmailLine->text(),
        firstname_input = ui->FirstnameLine->text(),
        lastname_input = ui->LastnameLine->text(),
        phone_input = ui->PhoneLine->text(),
        username_input = ui->UsernameLine->text(),
        role = ui->RoleBox->currentText();

    if (username_input != "" && firstname_input != "" && lastname_input != "" && phone_input != "" &&
        email_input != "" && address_input != "")
    {
        if (username_input != old_name && selectAccountByUsername(username_input).username == "")
        {
            account_object.username = username_input;
            account_object.address = address_input;
            account_object.email = email_input;
            account_object.firstName = firstname_input;
            account_object.lastName = lastname_input;
            account_object.phoneNumber = phone_input;
            account_object.position = role;

            updateAccount(account_object);
            Logs log;
            log.account = adminAccount;
            log.time = QDateTime::currentDateTime();
            log.action = "Edited User Info For '" + account_object.username + " ' Previously ('" + old_name + "')";
            log.type = "Account";
            createLog(log);

            MsgBox.information(this, "Update Complete", "User information update completed successfully.");
            MsgBox.show();

            if (parent_widget)
            {
                if (currentUser)
                {
                    parent_widget->setUsrname(account_object.username);
                }

                parent_widget->refreshTablesData();
            }

            else
            {
                parent_widget2->setUsername(account_object.username);
            }

            close();
        }

        else if (username_input == old_name)
        {
            bool unchanged = username_input == old_name && firstname_input == old_firstname &&
                lastname_input == old_lastname && email_input == old_email && address_input == old_address &&
                role == old_role && phone_input == old_phone;

            if (unchanged)
            {
                close();

                return;
            }

            account_object.address = address_input;
            account_object.email = email_input;
            account_object.firstName = firstname_input;
            account_object.lastName = lastname_input;
            account_object.phoneNumber = phone_input;
            account_object.position = role;

            updateAccount(account_object);
            Logs log;
            log.account = adminAccount;
            log.time = QDateTime::currentDateTime();
            log.action = "Edited User Info For '" + account_object.username + "'";
            log.type = "Account";
            createLog(log);
            MsgBox.information(this, "Update Complete", "User information update completed successfully.");
            MsgBox.show();

            if (parent_widget)
            {
                parent_widget->refreshTablesData();
                parent_widget->show();
            }
                close();
        }

        else
        {
            MsgBox.critical(this, "Invalid Fields", "That username already exists.");
        }
    }

    else
    {
        MsgBox.critical(this, "Empty Field", "There are empty fields.");
        MsgBox.show();
    }
}

void EditUser::closeEvent(QCloseEvent *event)
{
    if (parent_widget)
    {
        parent_widget->show();
    }

    event->accept();
}

