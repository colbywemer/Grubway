#include "vieworder.h"
#include "ui_vieworder.h"

#include <QMessageBox>

#include <string>

using namespace std;

ViewOrder::ViewOrder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewOrder)
{
    ui->setupUi(this);
}

ViewOrder::~ViewOrder()
{
    delete ui;
}

void ViewOrder::setAdminDash(QWidget *ad)
{
    parent_widget = ad;
}

//void ViewOrder::setUsername(QString username)
void ViewOrder::setID(int id)
{
    order_object = selectOrderById(id);

    setWindowTitle("Order " + QString::number(id) + " Information: ");

    old_name = order_object.account.username;
    old_firstname = order_object.account.firstName;
    old_lastname = order_object.account.lastName;
    old_email = order_object.account.email;
    old_address = order_object.account.address;
    old_phone = order_object.account.phoneNumber;
    old_total = order_object.total;

    ui->FirstnameLine->setText(order_object.account.firstName + " " + order_object.account.lastName);
    ui->AddressLine->setText(order_object.account.address);
    ui->PhoneLine->setText(order_object.account.phoneNumber);
    ui->EmailLine->setText(order_object.account.email);
    ui->TotalLine->setText(QString::number(order_object.total, 'f', 2));
    ui->notes->setText(order_object.notes);

    int index = ui->status->findText(order_object.status);

    ui->status->setCurrentIndex(index);

    QString formattedDate = order_object.pickupDate.toString("dddd, MMMM d, yyyy"),
        formattedTime = order_object.pickupDate.toString("h:mm AP"),
        formattedDateTime = formattedDate + " at " + formattedTime;

    ui->pickup->setText(formattedDateTime);

    for (const auto& [item, count] : order_object.items)
    {
        QListWidgetItem *listItem = new QListWidgetItem(item + " X " + QString::number(count));

        listItem->setFlags(listItem->flags() | Qt::ItemIsUserCheckable);
        listItem->setCheckState(Qt::Unchecked);

        ui->listWidget->addItem(listItem);
    }

}

void ViewOrder::on_CloseBtn_clicked()
{
    close();
}

void ViewOrder::closeEvent(QCloseEvent *event)
{
    parent_widget->show();

    event->accept();
}

void ViewOrder::on_updateStatus_clicked()
{
    QString status = ui->status->currentText(), title = "Update Order Status";

    order_object.status = status;

    updateOrder(order_object);

    QMessageBox MsgBox;
    MsgBox.information(this, title, "Order Status Updated");
    MsgBox.show();
}

