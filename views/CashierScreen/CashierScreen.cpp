#include "CashierScreen.h"
#include "ui_CashierScreen.h"

#include "inventoryscreen.h"

#include <AdminDashboard>
#include <Database>
#include <EditUser>
#include <ItemInputDialog>
#include <Login>
#include <ResetPass>
#include <ViewUser>

#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTableWidget>
#include <QUrlQuery>

double toPercent(double input)
{
    double percent = 100;

    return input / percent;
}

CashierScreen::CashierScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CashierScreen)
{
    ui->setupUi(this);
    
    connect(ui->inventoryButton, &QPushButton::clicked, this, &CashierScreen::openInventoryScreen);
    ui->checkOutButton->setEnabled(false);
    ui->keyword->setPlaceholderText("Id");
}

CashierScreen::~CashierScreen()
{
    delete ui;
}

void CashierScreen::setUsername(QString newUsername)
{
    username = newUsername;
}

void CashierScreen::on_ExitItem_clicked()
{
    // Get the item and price from the line edits
    ItemInputDialog dialog(this);

    if (dialog.exec() != QDialog::Accepted)
    {
        // The user cancelled, so don't do anything
        return;
    }

    // Create a new item with the entered name and price
    QString name = dialog.itemName();
    double price = dialog.itemPrice();

    // Add the item and price to the cart list
    ui->cartListWidget->addItem(name + " - $" + QString::number(price));
    ui->checkOutButton->setEnabled(true);
}

void CashierScreen::on_viewCartButton_clicked()
{
    // Clear the cart text edit
    ui->cartListWidget->clear();

    // Loop through the items in the cart list and append them to the cart text edit
    for (int i = 0; i < ui->cartListWidget->count(); i++)
    {
        ui->cartListWidget->addItem(ui->cartListWidget->item(i)->text());
    }
}

void CashierScreen::on_checkOutButton_clicked()
{
    /* Store Order to Database */
    Account current_account = selectAccountByUsername(username);
    Receipts new_receipt;

    new_receipt.account.id = current_account.id;
    new_receipt.datetime = QDateTime::currentDateTime();
    /***************************/

    // Calculate the total price of all items in the cart list
    double totalPrice = 0.0;

    for (int i = 0; i < ui->cartListWidget->count(); i++)
    {
        QString item = ui->cartListWidget->item(i)->text();
        QStringList parts = item.split(" - $");
        QString itemName = parts[0];
        QString name = itemName;
        double quantity = 1;
        double price = parts[1].toDouble();
        
        if(itemName.at(0).isNumber()){
            parts = itemName.split("lb ");
            quantity = parts[0].toDouble();
            name = parts[1];
        }

        vector<Item> items = selectAllItemsByName(name);
        if(!items.empty()){
            Item selectedItem;
            for(const auto& item : items){
                if(item.price == (price/quantity)){
                    selectedItem = item;
                }
            }
            if(selectedItem.name == ""){
                selectedItem = items[0];
            }
            selectedItem.sold += quantity;
            selectedItem.stock -= quantity;
            updateItem(selectedItem);
        }

        totalPrice += price;
        new_receipt.items[itemName]++;
        new_receipt.prices[itemName]+= price;
    }

    new_receipt.total = totalPrice;

    createReceipt(new_receipt);

    // Add the checkout amount to the daily total
    dailyTotal += totalPrice;

    // Create a receipt string with the current date, time, and items in the cart list
    QString receipt = "Date: " + QDate::currentDate().toString() + "\n" +
        "Time: " + QTime::currentTime().toString() + "\n\n" +
        "Items:\n";

    for (int i = 0; i < ui->cartListWidget->count(); i++)
    {
        receipt += ui->cartListWidget->item(i)->text() + "\n";
    }

    double tax_price = totalPrice * (toPercent(TAX_RATE));
    receipt += "\nTax: $" + QString::number(tax_price) +
        "\nTotal Price: $" + QString::number(totalPrice + tax_price);

    // Display the receipt in a message box
    QMessageBox::information(this, "Receipt", receipt);

    // Clear the cart list widget and cartItems vector
    ui->cartListWidget->clear();

    // Disable the Edit and Delete buttons
    ui->editItemButton->setEnabled(false);
    ui->deleteItemButton->setEnabled(false);
    ui->checkOutButton->setEnabled(false);
}

void CashierScreen::on_cartListWidget_itemClicked(QListWidgetItem *)
{
    ui->editItemButton->setEnabled(true);
    ui->deleteItemButton->setEnabled(true);
    ui->checkOutButton->setEnabled(true);
}


void CashierScreen::on_editItemButton_clicked()
{
    // Get the currently selected item in the cart list widget
    QListWidgetItem *item = ui->cartListWidget->currentItem();

    // Get the item and price from the selected item's text
    QString itemText = item->text();

    int index = itemText.lastIndexOf(" - $");

    QString itemName = itemText.left(index),
        itemPrice = itemText.mid(index + 4);

    ItemInputDialog dialog(this);

    dialog.m_itemNameInput->setText(itemName);
    dialog.m_itemPriceInput->setValue(itemPrice.toDouble());

    if (dialog.exec() != QDialog::Accepted)
    {
        // The user cancelled, so don't do anything
        return;
    }

    // Remove the selected item from the cart list widget
    delete item;

    QString name = dialog.itemName();
    double price = dialog.itemPrice();

    // Add the item and price to the cart list
    ui->cartListWidget->addItem(name + " - $" + QString::number(price));
}

void CashierScreen::on_deleteItemButton_clicked()
{
    // Remove the currently selected item from the cart list
    delete ui->cartListWidget->currentItem();
    if (ui->cartListWidget->count() == 0) {
        ui->deleteItemButton->setEnabled(false);
        ui->checkOutButton->setEnabled(false);
    }
}
void CashierScreen::on_Total_clicked()
{
    // Display the daily total in a message box
    QMessageBox::information(this, "Daily Total", QString("Daily Total: $%1").arg(dailyTotal));
}

// Needing to modify.
/* -Add Exit button in CasheirScreen tab to connect
it with data base when the item already in stock/ inventory.( example in inventory the item: a have id 123 , then I click Exit button it will ask for id item. Then I input 123 , it will show up item a via price, name , and minus it out of inventory ) ( if the item is meat or vegetables need to weigh it will show up window input item code, weigh of item)*/
void CashierScreen::on_addItemButton_clicked()
{
    // Ask for the item ID
    QString filter = ui->filter->currentText(),
        keyword = ui->keyword->text();

    Item item;

    if (filter == "Id")
    {
        item = selectItemById(keyword.toInt());
    }

    else
    {
        item = selectItemByName(keyword);
    }

    if(item.name != ""){
        // If the item is weighted, ask for the weight
        double itemWeight = 1.0;

        if (item.isPerPound)
        {
            bool ok;

            itemWeight = QInputDialog::getDouble(this, "Exit Item", "Enter the item weight:", 1.0, 0.1, 9999.9, 1, &ok);

            if (!ok)
            {
                // The user cancelled, so don't do anything
                return;
            }
        }
        // Calculate the total price
        double totalPrice = item.price * itemWeight;

        item.stock-=itemWeight;
        item.sold +=itemWeight;

        updateItem(item);

        if (!item.isPerPound)
        {
            ui->cartListWidget->addItem(item.name + " - $" + QString::number(item.price));
        }

        else
        {
            ui->cartListWidget->addItem(QString::number(itemWeight) + "lb " + item.name + " - $" + QString::number(totalPrice));
        }

        ui->checkOutButton->setEnabled(true);
    }

    else
    {
        // The item doesn't exist in the database, so show an error message
        QMessageBox::critical(this, "Error", "Item not found in database.");
    }

    ui->keyword->clear();
    ui->filter->setCurrentIndex(0);
    ui->keywordLabel->setText("Id:");
}

void CashierScreen::on_Exit_clicked()
{
    close ();
}

void CashierScreen::on_changePassword_clicked()
{
    ResetPass *resetpass = new ResetPass();
    resetpass->setUsername(username);
    resetpass->setAdminDash(this);
    resetpass->show();

    hide();
}

void CashierScreen::on_history_clicked() {
    // Get all orders from the database
    m_historyView = QSharedPointer<HistoryDialog>(new HistoryDialog());

    QString history = "";

    double totalPrice = 0.0;

    vector<Receipts> receipts = selectAllRecipts();

    ui->cartListWidget->clear();

    QMap<QDate, int> datetimelist;

    // Add each order to the list widget
    for (const auto& receipt : receipts)
    {
        QString orderText = "Order #" + QString::number(receipt.id) + " - " + receipt.account.username +
            " - " + receipt.datetime.toString() + "\n";

        history += orderText;

        for (const auto& pair : receipt.items)
        {
            QString item = pair.first;
            int quantity = pair.second;

            double price = 0.0;

            auto it = receipt.prices.find(item);

            if (it != receipt.prices.end())
            {
                price = it->second;
            }

            history += item + " X " + QString::number(quantity) + " -$" + QString::number(price) + "\n";
        }

        totalPrice += receipt.total;
        history += "\n";

        if (!datetimelist.contains(receipt.datetime.date()))
        {
            QTableWidgetItem *item1 = new QTableWidgetItem();
            item1->setText(receipt.datetime.date().toString());

            m_historyView->insertItemToEnd(item1);

            datetimelist.insert(receipt.datetime.date(), 0);
        }
    }

    history += QString("Total Price: $") + QString::number(totalPrice);

    qDebug() << history;
//    QMessageBox::information(this, "History", history);
    m_historyView->exec();
}

void CashierScreen::on_profile_clicked()
{
    EditUser *eu = new EditUser;
    eu->setUsername(username);
    eu->editingCurrentUser();
    eu->setCashier(this);

    eu->show();
}

void CashierScreen::on_logout_clicked()
{
    // Create a new dialog object for the log out screen
    QDialog *logoutDialog = new QDialog(this);

    // Set the layout for the dialog
    QVBoxLayout *layout = new QVBoxLayout(logoutDialog);

    // Add a message to the layout
    QLabel *messageLabel = new QLabel("Are you sure you want to log out?", logoutDialog);
    layout->addWidget(messageLabel);

    // Add confirm and cancel buttons to the layout
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *confirmButton = new QPushButton("Log out", logoutDialog),
        *cancelButton = new QPushButton("Cancel", logoutDialog);

    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);

    // Connect the clicked() signal of the confirm button to a slot that closes the dialog and returns to the login screen
    connect(confirmButton, &QPushButton::clicked, logoutDialog, &QDialog::accept);

    // Display the dialog and wait for the user's response
    int result = logoutDialog->exec();

    // If the user confirmed the log out, close the cashier screen and show the login screen
    if (result == QDialog::Accepted)
    {
        Login *login = new Login();
        login->show();

        close();
    }
}

void CashierScreen::closeEvent(QCloseEvent *event)
{
    Login *l = new Login;
    l->show();

    event->accept();
}

void CashierScreen::on_filter_currentIndexChanged(int index)
{
    if (index == 1)
    {
        ui ->keywordLabel->setText("Name:");
        ui->keyword->setPlaceholderText("Name");
    }

    else
    {
        ui ->keywordLabel->setText("Id:");
        ui->keyword->setPlaceholderText("Id");
    }
}

void CashierScreen::openInventoryScreen() {
    InventoryScreen *inventoryScreen = new InventoryScreen();
    inventoryScreen->setAccount(selectAccountByUsername(username));
    inventoryScreen->show();
}

