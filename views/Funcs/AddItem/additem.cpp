#include "additem.h"
#include "ui_additem.h"

#include <Database>

#include <QDebug>
#include <QMessageBox>

// addItem class definition
// This class defines the addItem widget, which is used to create or edit item information in the inventory system.

addItem::addItem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::addItem)
{
    // Set up UI for this widget
    ui->setupUi(this);
}

addItem::~addItem()
{
    // Clean up the UI when the widget is destroyed
    delete ui;
}

// setAdminDash method
// Sets the parent widget for the addItem widget, which is the AdminDashboard.
void addItem::setAdminDash(AdminDashboard *ad)
{
    parent_widget = ad;
}

// setAdminAccount method
// Sets the admin account to be used for logging actions.
void addItem::setAdminAccount(Account account)
{
    adminAccount = account;
}

// setItemId method
// Sets the item ID and loads the item data into the widget, preparing it for editing.
void addItem::setItemId(int id)
{
    // Retrieve item information from the database
    item_object = selectItemById(id);

    // Set the window title to include the item ID
    setWindowTitle("Edit Item Information for: " + QString::number(item_object.id));

    // Load item information into the input fields
    ui->name->setText(item_object.name);
    ui->price->setValue(item_object.price);
    ui->department->setCurrentText(item_object.department);
    ui->isPerPound->setCurrentText(item_object.isPerPound ? "True" : "False");
}

// on_OKBtn_clicked method
// Handles the OK button click, either creating a new item or updating an existing item.
void addItem::on_OKBtn_clicked()
{
    // Create a new item object and populate it with data from the input fields
    Item item;
    item.name = ui->name->text();
    item.department = ui->department->currentText();
    item.price = ui->price->value();
    item.isPerPound = ui->isPerPound->currentText() == "True" ? true : false;

    // Check if input fields are not empty
    if (item.name != "" && item.price != 0 && item.department != "")
    {
        // If the item_object has no name, create a new item
        if (item_object.name == "")
        {
            createItem(item);

            // Create a log entry for the item creation
            Logs log;
            log.account = selectAccountByUsername(adminAccount.username);
            log.time = QDateTime::currentDateTime();
            log.action = "Created Item '" + item.name + "'";
            log.type = "Item";

            createLog(log);
        }
        else
        {
            // Update the existing item
            item.id = item_object.id;

            updateItem(item);

            // Create a log entry for the item update
            Logs log;
            log.account = selectAccountByUsername(adminAccount.username);
            log.time = QDateTime::currentDateTime();
            log.action = "Edited Item '" + item.name + "'";
            log.type = "Item";

            createLog(log);
        }

        // Refresh the item list in the parent_widget (AdminDashboard)
        parent_widget->refreshItemList();

        // Close the addItem widget
        close();
    }
    else
    {
        // Show an error message if input fields are empty
        QMessageBox MsgBox;
        MsgBox.critical(this, "Empty Field", "There are empty fields.");
        MsgBox.show();
    }
}

// on_CancelBtn_clicked method
// Handles the Cancel button click, closing the addItem widget.
void addItem::on_CancelBtn_clicked()
{
    close();
}

// on_department_currentTextChanged method
// Updates the department input field's color to black when its text is changed.
void addItem::on_department_currentTextChanged(const QString &)
{
    ui->department->setStyleSheet("color: black;");
}

void addItem::closeEvent(QCloseEvent *event)
{
    parent_widget->show();

    event->accept();
}

