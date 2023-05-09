#include "viewuser.h"
#include "ui_viewuser.h"

ViewUser::ViewUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewUser),
parent_widget(nullptr)
{
    ui->setupUi(this);
}

ViewUser::~ViewUser()
{
    delete ui;
}

void ViewUser::setAdminDash(QWidget *ad)
{
    parent_widget = ad;
}

void ViewUser::setUsername(QString username)
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
    ui->RoleLine->setText(account_object.position);
}

void ViewUser::on_CloseBtn_clicked()
{
    close();
}

void ViewUser::closeEvent(QCloseEvent *event)
{
    parent_widget->show();

    event->accept();
}

