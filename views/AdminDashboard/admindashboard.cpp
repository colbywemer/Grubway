#include "admindashboard.h"
#include "ui_admindashboard.h"

#include "AdminArea/resetpass.h"
#include "AdminArea/edituser.h"
#include "AdminArea/viewuser.h"
#include "AdminArea/vieworder.h"

#include <common.h>

#include <AddItem>
#include <Database>
#include <InventoryScreen>
#include <LoginAndRegister>

#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QString>

QString Placeholder = "<Empty>";

QStringList user_table_list =
{
    "First Name",
    "Last Name",
    "Username"
},

order_table_list =
{
    "Order ID",
    "Name",
    "Pickup Date",
    "Status"
},

item_table_list =
{
    "Item ID",
    "Name",
    "Price",
    "Department",
    "Priced Per Pound"
},
log_table_list =
    {
        "Action",
        "Time",
        "Username",
};

int Firstname_Col = 0, Lastname_Col = 1, Username_Col = 2,
    order_OrderID_Col = 0, order_Name_Col = 1, order_PickupDate_Col = 2, order_Status_Col = 3,
    item_ItemID_Col = 0, item_Name_Col = 1, item_Price_Col = 2, item_Department_Col = 3, item_IsPerPound_col = 4, log_Action_Col = 0, log_Time_Col = 1, log_Username_Col = 2, log_Time_size = 239, log_Username_size = 319, log_Action_size = 454,
    order_OrderID_size = 252, order_PickupDate_size = 255, order_Name_size = 252, order_Status_size = 252, user_FirstName_size = 337, user_LastName_size = 337, user_Username_size = 338, items_col_size = 202, items_extra_size = 204;

AdminDashboard::AdminDashboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminDashboard)
{
    ui->setupUi(this);

    ui->keyWordText->hide();
    ui->keywordSelect->hide();
    ui->keywordId->hide();
    ui->filterItemName->hide();
    ui->filterItemDepartment->hide();
    ui->filterEmpText->hide();
    ui->filterEmpRole->hide();
    ui->filterCustText->hide();
    ui->logDate->hide();
    ui->logKeywordText->hide();
}

AdminDashboard::~AdminDashboard()
{
    delete ui;
}

// This is a 10-minute timer to refresh user and order screens
void AdminDashboard::timerEvent(QTimerEvent *event)
{
    refreshTablesData();

    event->accept();
}

void AdminDashboard::refreshCustomerList(QString filter, QString keyword){
    // Temporary disable sorting to prevent mess-up when populating
    ui->CustAccountsTable->setSortingEnabled(false);

    // Delete all rows in Customer Table
    if (ui->CustAccountsTable->rowCount() > Empty)
    {
        for (int count = ui->CustAccountsTable->rowCount(); count > Empty; count--)
        {
            ui->CustAccountsTable->removeRow(count - 1);
        }
    }

    // Set the header labels to table_list
    ui->CustAccountsTable->setHorizontalHeaderLabels(user_table_list);
    ui->CustAccountsTable->setColumnWidth(Firstname_Col, user_FirstName_size);
    ui->CustAccountsTable->setColumnWidth(Lastname_Col, user_LastName_size);
    ui->CustAccountsTable->setColumnWidth(Username_Col, user_Username_size);

    vector<Account> account_vector;
    if(filter == "" || keyword == ""){
        account_vector = selectAllAccounts();
    }else{
        account_vector = selectAllAccountsByFilter(filter, keyword);
    }

    int cust_count = 0;

    for (auto it = account_vector.begin(); it != account_vector.end(); ++it)
    {

        // For every item that is a customer account
        if (it->position == "")
        {
            QTableWidgetItem *item1 = new QTableWidgetItem,
                *item2 = new QTableWidgetItem,
                    *item3 = new QTableWidgetItem;

            item1->setText(it->firstName);
            item2->setText(it->lastName);
            item3->setText(it->username);

            // Create a row with First, Last, Username
            ui->CustAccountsTable->insertRow(cust_count);
            ui->CustAccountsTable->setItem(cust_count, Firstname_Col, item1);
            ui->CustAccountsTable->setItem(cust_count, Lastname_Col, item2);
            ui->CustAccountsTable->setItem(cust_count, Username_Col, item3);

            cust_count++;
        }
    }

    // Set a placeholder item if no customer accounts exist
    if (ui->CustAccountsTable->rowCount() == Empty)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem;

        item1->setText(Placeholder);

        ui->CustAccountsTable->insertRow(Empty);
        ui->CustAccountsTable->setItem(Empty, Username_Col, item1);
    }

    // Set default selected cell and re-enable sorting

    ui->CustAccountsTable->setCurrentCell(Empty, Empty);
    ui->CustAccountsTable->setSortingEnabled(true);
}


void AdminDashboard::refreshEmployeeList(QString filter, QString keyword){

    // Temporary disable sorting to prevent mess-up when populating
    ui->EmpAccountsTable->setSortingEnabled(false);

    // Delete all rows in Employee Table
    if (ui->EmpAccountsTable->rowCount() > Empty)
    {
        for (int count = ui->EmpAccountsTable->rowCount(); count > Empty; count--)
        {
            ui->EmpAccountsTable->removeRow(count - 1);
        }
    }

    // Set the header labels to table_list
    ui->EmpAccountsTable->setHorizontalHeaderLabels(user_table_list);
    ui->EmpAccountsTable->setColumnWidth(Firstname_Col, user_FirstName_size);
    ui->EmpAccountsTable->setColumnWidth(Lastname_Col, user_LastName_size);
    ui->EmpAccountsTable->setColumnWidth(Username_Col, user_Username_size);
    vector<Account> account_vector;
    if(filter == "" || keyword == ""){
        account_vector = selectAllAccounts();
    }else{
        account_vector = selectAllAccountsByFilter(filter, keyword);
    }

    int emp_count = 0;

    for (auto it = account_vector.begin(); it != account_vector.end(); ++it)
    {
        // For every item that is an employee account that is not the current user
        if (it->username != admin_username && it->position != "")
        {
            QTableWidgetItem *item1 = new QTableWidgetItem,
                *item2 = new QTableWidgetItem,
                    *item3 = new QTableWidgetItem;

            item1->setText(it->firstName);
            item2->setText(it->lastName);
            item3->setText(it->username);

            // Create a new row with First, Last, Username
            ui->EmpAccountsTable->insertRow(emp_count);
            ui->EmpAccountsTable->setItem(emp_count, Firstname_Col, item1);
            ui->EmpAccountsTable->setItem(emp_count, Lastname_Col, item2);
            ui->EmpAccountsTable->setItem(emp_count, Username_Col, item3);

            emp_count++;
        }
    }

    // Set a placeholder item if no other employee accounts exist
    if (ui->EmpAccountsTable->rowCount() == Empty)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem;

        item1->setText(Placeholder);

        ui->EmpAccountsTable->insertRow(Empty);
        ui->EmpAccountsTable->setItem(Empty, Username_Col, item1);
    }


    // Set default selected cell and re-enable sorting
    ui->EmpAccountsTable->setCurrentCell(Empty, Empty);
    ui->EmpAccountsTable->setSortingEnabled(true);

}



// Iterate through a vector and populate the AccountsTable objects with account usernames
void AdminDashboard::refreshTablesData()
{
    // Call the Customer refresh method to get the data filled in
    refreshCustomerList();
    // Call the Employee refresh method to get the data filled in
    refreshEmployeeList();
    // Call the Order refresh method to get the data filled in
    refreshOrderList();
    // Call the Item refresh method to get the data filled in
    refreshItemList();
    // Call the Logs refresh method to get the data filled in
    refreshLogsList();
}

// Sets the label on the 'Account' tab to display the user's name
void AdminDashboard::setUsrname(QString usrname)
{
    admin_username = usrname;

    ui->UsernameLabel->setText("<html><head/><body><p><span style=\" font-size:24pt;\">" + admin_username + "</span></p></body></html>");

    // Call the refresh method
    refreshTablesData();



}

void AdminDashboard::on_ViewEmpUsrBtn_clicked()
{
    int row = ui->EmpAccountsTable->currentRow();

    QTableWidgetItem *SelectedWidgetItem = ui->EmpAccountsTable->item(row, column);
    QString SelectedItem = SelectedWidgetItem->text();

    viewUserAccount(SelectedItem);
}

void AdminDashboard::on_ChgNameBtn_clicked()
{
    EditUser *eu = new EditUser;
    eu->setUsername(admin_username);
    eu->setAdminAccount(selectAccountByUsername(admin_username));
    eu->setAdminDash(this);
    eu->editingCurrentUser();
    eu->show();

    hide();
}

void AdminDashboard::on_ChgPasswdBtn_clicked()
{
    ResetPass *rp = new ResetPass;
    rp->setUsername(admin_username);
    rp->setAdminAccount(selectAccountByUsername(admin_username));
    rp->setAdminDash(this);
    rp->show();

    hide();
}

void AdminDashboard::on_CreateUsrBtn_clicked()
{
    Register *r = new Register;
    r->noReturnToLogin();        // Do not show the login page
    r->setAdminDash(this);
    r->setAdminAccount(selectAccountByUsername(admin_username));
    r->show();

    hide();
}

void AdminDashboard::on_DelEmpUsrBtn_clicked()
{
    int row = ui->EmpAccountsTable->currentRow();

    QTableWidgetItem *SelectedItemWidget = ui->EmpAccountsTable->item(row, column);
    QString SelectedItem = SelectedItemWidget->text();

    deleteUserAccount(SelectedItem);
}

void AdminDashboard::on_EditUsrBtn_clicked()
{
    int row = ui->EmpAccountsTable->currentRow();

    QTableWidgetItem *SelectedItemWidget = ui->EmpAccountsTable->item(row, column);
    QString SelectedItem = SelectedItemWidget->text();

    if (SelectedItem != Placeholder)
    {
        EditUser *eu = new EditUser;
        eu->setUsername(SelectedItem);
        eu->setAdminDash(this);
        eu->setAdminAccount(selectAccountByUsername(admin_username));
        eu->show();

        hide();
    }

    else
    {
        QMessageBox MsgBox;
        MsgBox.critical(this, "No Accounts Available", "No accounts currently exist to be modified.");
        MsgBox.show();
    }
}

void AdminDashboard::on_EndSessionBtn_clicked()
{
    close();
}

void AdminDashboard::on_ResetPassBtn_clicked()
{
    int row = ui->EmpAccountsTable->currentRow();

    QTableWidgetItem *SelectedItemWidget = ui->EmpAccountsTable->item(row, column);
    QString SelectedItem = SelectedItemWidget->text();

    // Check that the selected item is not <Empty>
    if (SelectedItem != Placeholder)
    {
        ResetPass *rp = new ResetPass;
        rp->setUsername(SelectedItem);
        rp->setAdminDash(this);
        rp->setAdminAccount(selectAccountByUsername(admin_username));
        rp->show();

        hide();
    }

    else
    {
        QMessageBox MsgBox;
        MsgBox.critical(this, "No Accounts Available", "No accounts currently exist to be modified.");
        MsgBox.show();
    }
}

void AdminDashboard::on_DelCustUsrBtn_clicked()
{
    int row = ui->CustAccountsTable->currentRow();

    QTableWidgetItem *SelectedItemWidget = ui->CustAccountsTable->item(row, column);
    QString SelectedItem = SelectedItemWidget->text();

    deleteUserAccount(SelectedItem);
}

void AdminDashboard::on_ViewCustUsrBtn_clicked()
{
    int row = ui->CustAccountsTable->currentRow();

    QTableWidgetItem *SelectedWidgetItem = ui->CustAccountsTable->item(row, column);
    QString SelectedItem = SelectedWidgetItem->text();

    viewUserAccount(SelectedItem);
}

// View Account method to view either an employee account or customer account
void AdminDashboard::viewUserAccount(QString username)
{
    // Check that the SelectedItem is not empty
    if (username != Placeholder)
    {
        ViewUser *vu = new ViewUser;
        vu->setAdminDash(this);
        vu->setUsername(username);
        vu->show();

        hide();
    }

    else
    {
        QMessageBox MsgBox;
        MsgBox.critical(this, "No Accounts Available", "No accounts currently exist to be viewed.");
        MsgBox.show();
    }
}

void AdminDashboard::on_RefreshBtn_clicked()
{
    refreshCustomerList();
}

void AdminDashboard::deleteUserAccount(QString username)
{
    QMessageBox MsgBox;

    // Check that the SelectedItem is not empty
    if (username != Placeholder)
    {
        QMessageBox::StandardButton DialogResult = MsgBox.warning(this, "Delete Confirmation", "Are you sure you wish to delete account '" + username + "'? (This action is irreversible.)", QMessageBox::Yes | QMessageBox::No);

        // Item is to be deleted
        if (DialogResult == MsgBox.Yes)
        {
            // Delete the account
            Account account_object = selectAccountByUsername(username);

            deleteAccount(account_object.id);
            Logs log;
            log.account = selectAccountByUsername(admin_username);
            log.time = QDateTime::currentDateTime();
            log.action = "Deleted Account '" + username + "'";
            log.type = "Account";
            createLog(log);

            // Prompt user with completion notice
            MsgBox.information(this, "Delete Successful", "The account was successfully deleted.");

            // Refresh the username list
            if(account_object.position == ""){
                refreshCustomerList();
            }else{
                refreshEmployeeList();
            }
        }
    }

    else
    {
        MsgBox.critical(this, "No Accounts Available", "No accounts currently exist to be deleted.");
        MsgBox.show();
    }
}

// CloseEvent Signal Handler to go back to login
void AdminDashboard::closeEvent(QCloseEvent *event)
{    
    killTimer(timerId);

    Login *l = new Login;
    l->show();

    event->accept();
}

// This is the code for the Orders tab
void AdminDashboard::on_DelOrderBtn_clicked()
{
    int row = ui->OrdersTable->currentRow();

    //From the customers delete button logic. with modifications
    QTableWidgetItem *SelectedItemWidget = ui->OrdersTable->item(row, orderColumn);
    QString SelectedItem = SelectedItemWidget->text();

    int int_SelectedItem = SelectedItem.toInt();

    deleteTheOrder(int_SelectedItem);
}

void AdminDashboard::on_ViewOrderBtn_clicked()
{
    int row = ui->OrdersTable->currentRow();

    // The column is used so the ID is always selected
    QTableWidgetItem *SelectedWidgetItem = ui->OrdersTable->item(row, orderColumn);
    QString SelectedItem = SelectedWidgetItem->text();

    viewOrder(SelectedItem);
}

// View an order
void AdminDashboard::viewOrder(QString id)
{
    // Check that the SelectedItem is not empty
    if (id != Placeholder)
    {
        ViewOrder *vo = new ViewOrder;
        vo->setAdminDash(this);
        vo->setID(id.toInt());
        vo->show();

        hide();
    }

    else
    {
        QMessageBox MsgBox;
        MsgBox.critical(this, "No Orders Available", "No orders currently exist to be viewed.");
        MsgBox.show();
    }
}

void AdminDashboard::on_RefreshOrderBtn_clicked()
{
    refreshOrderList();
}

// Iterate through a vector and populate the OrdersTable objects with orders
void AdminDashboard::refreshOrderList(QString filter, QString keyword)
{
    // Temporary disable sorting to prevent mess-up when populating
    ui->OrdersTable->setSortingEnabled(false);

    // Delete all rows in Order Table
    if (ui->OrdersTable->rowCount() > Empty)
    {
        for (int count = ui->OrdersTable->rowCount(); count > Empty; count--)
        {
            ui->OrdersTable->removeRow(count + Decrement);
        }
    }

    // Set the header labels
    ui->OrdersTable->setHorizontalHeaderLabels(order_table_list);
    ui->OrdersTable->setColumnWidth(order_OrderID_Col, order_OrderID_size);
    ui->OrdersTable->setColumnWidth(order_PickupDate_Col, order_PickupDate_size);
    ui->OrdersTable->setColumnWidth(order_Status_Col, order_Status_size);
     ui->OrdersTable->setColumnWidth(order_Name_Col, order_Name_size);

    vector<Order> order_vector;

    if (filter == "" || keyword == "")
    {
        order_vector = selectAllOrders();
    }

    else if (filter == "Pickup Date")
    {
        order_vector = selectAllFilteredOrdersByDate(keyword);
    }

    else if (filter == "Status")
    {
        order_vector = selectAllFilteredOrdersByStatus(keyword);
    }

    else if (filter == "Id")
    {
        Order order = selectOrderById(keyword.toInt());
        if(order.id){
            order_vector.push_back(order);
        }

    }

    else
    {
        QStringList nameList = keyword.split(" ", Qt::SkipEmptyParts);
        QString firstName = nameList.at(0), lastName = nameList.mid(1).join(" ");

        order_vector = selectAllFilteredOrdersByName(firstName, lastName);
    }

    int order_count = 0;

    for (auto it = order_vector.begin(); it != order_vector.end(); ++it) {

        QString formattedDate = it->pickupDate.toString("dddd, MMMM d, yyyy"),
            formattedTime = it->pickupDate.toString("h:mm AP"),
            formattedDateTime = formattedDate + " at " + formattedTime;

        QTableWidgetItem *item1 = new QTableWidgetItem(),
            *item2 = new QTableWidgetItem(it->account.firstName + " " + it->account.lastName),
            *item3 = new QTableWidgetItem(formattedDateTime),
            *item4 = new QTableWidgetItem(it->status);

        item1->setData(Qt::DisplayRole, QVariant(it->id));

        // Create a new row and set the table items
        ui->OrdersTable->insertRow(order_count);
        ui->OrdersTable->setItem(order_count, order_OrderID_Col, item1);
        ui->OrdersTable->setItem(order_count, order_Name_Col, item2);
        ui->OrdersTable->setItem(order_count, order_PickupDate_Col, item3);
        ui->OrdersTable->setItem(order_count, order_Status_Col, item4);

        order_count++;
    }

    // Set a placeholder item if no orders exist
    if (ui->OrdersTable->rowCount() == Empty)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem;

        item1->setText(Placeholder);

        ui->OrdersTable->insertRow(Empty);
        ui->OrdersTable->setItem(Empty, order_OrderID_Col, item1);
    }

    // Set default selected cell and re-enable sorting
    ui->OrdersTable->setCurrentCell(Empty, Empty);
    ui->OrdersTable->setSortingEnabled(true);
}

void AdminDashboard::deleteTheOrder(int id)
{
    QMessageBox MsgBox;

    // Check that the SelectedItem is not empty
    if (id != Empty)
    {
        QMessageBox::StandardButton DialogResult = MsgBox.warning(this, "Delete Confirmation", "Are you sure you wish to delete order '" + QString::number(id) + "'? (This action is irreversible.)", QMessageBox::Yes | QMessageBox::No);

        // Item is to be deleted
        if (DialogResult == MsgBox.Yes)
        {
            // Update stock and sold value of items for canceled order

             Order order = selectOrderById(id);
            if(order.status != "Complete"){
                 for (const auto& [item, count] : order.items)
                 {
                     double quantity = count;
                     QString name = item;
                     if(item.at(0).isNumber()){
                         QStringList parts = item.split("lb ");
                         quantity = parts[0].toDouble();
                         name = parts[1];
                     }
                     Item selectedItem = selectItemByName(name);
                     if(selectedItem.name != ""){
                         selectedItem.stock+= quantity;
                         selectedItem.sold-= quantity;
                         updateItem(selectedItem);
                     }
                 }
             }

            // Delete the order
            deleteOrder(id);
            Logs log;
            log.account = selectAccountByUsername(admin_username);
            log.time = QDateTime::currentDateTime();
            log.action = "Deleted Order '" + QString::number(order.id) + "' For  '" + order.account.firstName + " " + order.account.lastName + "'";
            log.type = "Order";
            createLog(log);
            // Prompt user with completion notice
            MsgBox.information(this, "Delete Successful", "The order was successfully deleted.");

            // Refresh the order list
            refreshOrderList();
        }
    }

    else
    {
        MsgBox.critical(this, "No Orders Available", "No orders currently exist to be deleted.");
        MsgBox.show();
    }
}

void AdminDashboard::on_createItemButton_clicked()
{
    addItem *aI = new addItem;
    aI->setAdminDash(this);
    aI->setAdminAccount(selectAccountByUsername(admin_username));
    aI->show();


    hide();
}

void AdminDashboard::on_editItemButton_clicked()
{
    int row = ui->ItemsTable->currentRow();

    QTableWidgetItem *SelectedItemWidget = ui->ItemsTable->item(row, 0);
    QString SelectedItem = SelectedItemWidget->text();

    if (SelectedItem != Placeholder)
    {
        int int_SelectedItem = SelectedItem.toInt();

        addItem *aI = new addItem;
        aI->setItemId(int_SelectedItem);
        aI->setAdminDash(this);
        aI->setAdminAccount(selectAccountByUsername(admin_username));
        aI->show();

        hide();
    }

    else
    {
        QMessageBox MsgBox;
        MsgBox.critical(this, "No Items Available", "No Items currently exist to be modified.");
        MsgBox.show();
    }
}

void AdminDashboard::on_deleteItemButton_clicked()
{
    int row = ui->ItemsTable->currentRow();

    //From the customers delete button logic. with modifications
    QTableWidgetItem *SelectedItemWidget = ui->ItemsTable->item(row, orderColumn);
    QString SelectedItem = SelectedItemWidget->text();

    int int_SelectedItem = SelectedItem.toInt();

    deleteTheItem(int_SelectedItem);
}

void AdminDashboard::deleteTheItem(int id)
{
    QMessageBox MsgBox;

    // Check that the SelectedItem is not empty
    if (id != Empty)
    {
        QMessageBox::StandardButton DialogResult = MsgBox.warning(this, "Delete Confirmation", "Are you sure you wish to delete item '" + QString::number(id) + "'? (This action is irreversible.)", QMessageBox::Yes | QMessageBox::No);

        // Item is to be deleted
        if (DialogResult == MsgBox.Yes)
        {
            Item item = selectItemById(id);
            // Delete the item
            deleteItem(id);
            Logs log;
            log.account = selectAccountByUsername(admin_username);
            log.time = QDateTime::currentDateTime();
            log.action = "Deleted Item '" + item.name + "'";
            log.type = "Item";
            createLog(log);

            // Prompt user with completion notice
            MsgBox.information(this, "Delete Successful", "The item was successfully deleted.");

            // Refresh the item list
            refreshItemList();
        }
    }

    else
    {
        MsgBox.critical(this, "No Items Available", "No Items currently exist to be deleted.");
        MsgBox.show();
    }
}

void AdminDashboard::refreshItemList(QString filter, QString keyword)
{
    // Temporary disable sorting to prevent mess-up when populating
    ui->ItemsTable->setSortingEnabled(false);

    // Delete all rows in Items Table
    if (ui->ItemsTable->rowCount() > Empty)
    {
        for (int count = ui->ItemsTable->rowCount(); count > Empty; count--)
        {
            ui->ItemsTable->removeRow(count + Decrement);
        }
    }

    // Set the header labels
    ui->ItemsTable->setHorizontalHeaderLabels(item_table_list);
    ui->ItemsTable->setColumnWidth(item_ItemID_Col, items_col_size);
    ui->ItemsTable->setColumnWidth(item_Name_Col, items_extra_size);
    ui->ItemsTable->setColumnWidth(item_Price_Col, items_col_size);
    ui->ItemsTable->setColumnWidth(item_Department_Col, items_col_size);
    ui->ItemsTable->setColumnWidth(item_IsPerPound_col, items_col_size);

    vector<Item> item_vector;
    if (filter == "" || keyword == "")
    {
        item_vector = selectAllItems();
    }else{
        item_vector = selectAllItemsByFilter(filter, keyword);
    }


    int item_count = 0;

    for (auto it = item_vector.begin(); it != item_vector.end(); ++it)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(it->id)),
            *item2 = new QTableWidgetItem(it->name),
            *item3 = new QTableWidgetItem(),
            *item4 = new QTableWidgetItem(it->department),
            *item5 = new QTableWidgetItem(it->isPerPound ? "True" : "False");

        item1->setData(Qt::DisplayRole, QVariant(it->id));
        item3->setData(Qt::DisplayRole, QVariant(it->price));

        // Create a new row and set the table items
        ui->ItemsTable->insertRow(item_count);
        ui->ItemsTable->setItem(item_count, item_ItemID_Col, item1);
        ui->ItemsTable->setItem(item_count, item_Name_Col, item2);
        ui->ItemsTable->setItem(item_count, item_Price_Col, item3);
        ui->ItemsTable->setItem(item_count, item_Department_Col, item4);
        ui->ItemsTable->setItem(item_count, item_IsPerPound_col, item5);

        item_count++;
    }

    // Set a placeholder item if no items exist
    if (ui->ItemsTable->rowCount() == Empty)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem;

        item1->setText(Placeholder);

        ui->ItemsTable->insertRow(Empty);
        ui->ItemsTable->setItem(Empty, item_ItemID_Col, item1);
    }

    // Set default selected cell and re-enable sorting
    ui->ItemsTable->setCurrentCell(Empty, Empty);
    ui->ItemsTable->setSortingEnabled(true);
}

void AdminDashboard::on_filter_currentIndexChanged(int index)
{
    switch (index)
    {
        case 0:
            ui->keyWordDate->show();
            ui->keyWordText->hide();
            ui->keywordSelect->hide();
            ui->keywordId ->hide();
            break;

        case 1:
            ui->keyWordDate->hide();
            ui->keyWordText->hide();
            ui->keywordSelect->show();
            ui->keywordId->hide();
            break;

        case 2:
            ui->keyWordDate->hide();
            ui->keyWordText->show();
            ui->keywordSelect->hide();
            ui->keywordId->hide();
            break;

        default:
            ui->keyWordDate->hide();
            ui->keyWordText->hide();
            ui->keywordSelect->hide();
            ui->keywordId->show();
            break;
    }
}

void AdminDashboard::on_orderFilterButton_clicked()
{
    QString filter = ui->filter->currentText(),
        keyword = "";

    if (filter == "Pickup Date")
    {
        QDate selectedDate = ui->keyWordDate->date();

        keyword = selectedDate.toString("MM/dd/yyyy");
    }

    else if (filter == "Status")
    {
        keyword = ui->keywordSelect->currentText();
    }

    else if (filter == "Id")
    {
        keyword = ui->keywordId->text();
    }

    else
    {
        keyword = ui->keyWordText->text();
    }

    refreshOrderList(filter, keyword);
}

void AdminDashboard::on_inventoryButton_clicked()
{
    InventoryScreen *is = new InventoryScreen;
    is->setAccount(selectAccountByUsername(admin_username));
    is->setAdminDash(this);
    is->show();
}

void AdminDashboard::on_ChangeCodeBtn_clicked()
{
    Account account = selectAccountByUsername(admin_username);
    Account checkAccount;
    bool ok = false;
    do{
        if(ok){
            QMessageBox MsgBox;
            MsgBox.critical(this, "Invalid Code", "Code Already In Use!");
            MsgBox.show();
        }
        QInputDialog inputDialog(this);
        inputDialog.setInputMode(QInputDialog::TextInput);
        inputDialog.setLabelText("Enter new code value:");
        inputDialog.setTextValue(account.code);
        inputDialog.setStyleSheet("QLineEdit{border-radius: 5px;} QPushButton{color: white;background-color: #0e67ed;border-radius: 5px;padding: 5px;}");
        ok = inputDialog.exec();
        QString newCode = inputDialog.textValue();
        if (ok && !newCode.isEmpty())
        {
            account.code = newCode;
        }
        checkAccount = accountCodeExists(newCode);
    }while(account.code == checkAccount.code && account.username != checkAccount.username && ok);
    if(ok){
        updateAccount(account);
    }
}


void AdminDashboard::on_itemFilterButton_clicked()
{
    QString filter = ui->itemFilter->currentText().toLower(),
        keyword = "";

    if (filter == "id")
    {
        keyword = ui->filterItemId->text();
    }

    else if (filter == "name")
    {
        keyword = ui->filterItemName->text();
    }

    else
    {
        keyword = ui->filterItemDepartment->currentText();
    }


    refreshItemList(filter, keyword);
}


void AdminDashboard::on_itemFilter_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        ui->filterItemId->show();
        ui->filterItemName->hide();
        ui->filterItemDepartment->hide();
        break;

    case 1:
        ui->filterItemId->hide();
        ui->filterItemName->show();
        ui->filterItemDepartment->hide();
        break;

    default:
        ui->filterItemId->hide();
        ui->filterItemName->hide();
        ui->filterItemDepartment->show();
        break;
    }
}


void AdminDashboard::on_filterEmpButton_clicked()
{
    QString filter = ui->empFilter->currentText().toLower(),
        keyword = "";

    if (filter == "id")
    {
        keyword = ui->filterEmpId->text();
    }

    else if (filter == "first name")
    {
        filter = "firstName";
        keyword = ui->filterEmpText->text();
    }

    else if (filter == "last name")
    {
        filter = "lastName";
       keyword = ui->filterEmpText->text();
    }

    else if(filter == "username")
    {
        keyword = ui->filterEmpText->text();
    }

    else{
        keyword = ui->filterEmpRole->currentText();
    }

    refreshEmployeeList(filter, keyword);
}


void AdminDashboard::on_custFilter_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        ui->filterCustId->show();
        ui->filterCustText->hide();
        break;

    default:
        ui->filterCustId->hide();
        ui->filterCustText->show();
        break;
    }
}


void AdminDashboard::on_empFilter_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        ui->filterEmpId->show();
        ui->filterEmpText->hide();
        ui->filterEmpRole->hide();
        break;

    case 1:
    case 2:
    case 3:
        ui->filterEmpId->hide();
        ui->filterEmpText->show();
        ui->filterEmpRole->hide();
        break;

    default:
        ui->filterEmpId->hide();
        ui->filterEmpText->hide();
        ui->filterEmpRole->show();
        break;
    }
}


void AdminDashboard::on_filterCustomerButton_clicked()
{
    QString filter = ui->custFilter->currentText().toLower(),
        keyword = "";

    if (filter == "id")
    {
        keyword = ui->filterCustId->text();
    }

    else if (filter == "first name")
    {
        filter = "firstName";
        keyword = ui->filterCustText->text();
    }

    else if (filter == "last name")
    {
        filter = "lastName";
        keyword = ui->filterCustText->text();
    }

    else
    {
        keyword = ui->filterCustText->text();
    }

    refreshCustomerList(filter, keyword);
}

void AdminDashboard::refreshLogsList(QString filter, QString keyword)
{
    // Temporary disable sorting to prevent mess-up when populating
    ui->logsTable->setSortingEnabled(false);

    // Delete all rows in Order Table
    if (ui->logsTable->rowCount() > Empty)
    {
        for (int count = ui->logsTable->rowCount(); count > Empty; count--)
        {
            ui->logsTable->removeRow(count + Decrement);
        }
    }

    // Set the header labels
    ui->logsTable->setHorizontalHeaderLabels(log_table_list);
    ui->logsTable->setColumnWidth(log_Action_Col, log_Action_size);
    ui->logsTable->setColumnWidth(log_Time_Col, log_Time_size);
    ui->logsTable->setColumnWidth(log_Username_Col, log_Username_size);


    vector<Logs> log_vector;
    if(filter == ""|| keyword == ""){
        log_vector = selectAllLogs();
    }
    else if(filter == "date"){
        log_vector = selectAllLogsByDate(keyword);
    }
    else{
        log_vector = selectAllLogsByFilter(filter, keyword);
    }


    int log_count = 0;

    for (auto it = log_vector.begin(); it != log_vector.end(); ++it) {

        QString formattedDate = it->time.toString("dddd, MMMM d, yyyy"),
            formattedTime = it->time.toString("h:mm AP"),
            formattedDateTime = formattedDate + " at " + formattedTime;

        QTableWidgetItem *item1 = new QTableWidgetItem(it->action),
            *item2 = new QTableWidgetItem(formattedDateTime),
                *item3 = new QTableWidgetItem(it->account.username);

        // Create a new row and set the table items
        ui->logsTable->insertRow(log_count);
        ui->logsTable->setItem(log_count, log_Action_Col, item1);
        ui->logsTable->setItem(log_count, log_Time_Col, item2);
        ui->logsTable->setItem(log_count, log_Username_Col, item3);

        log_count++;
    }

    // Set a placeholder item if no orders exist
    if (ui->logsTable->rowCount() == Empty)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem;

        item1->setText(Placeholder);

        ui->logsTable->insertRow(Empty);
        ui->logsTable->setItem(Empty, order_OrderID_Col, item1);
    }

    // Set default selected cell and re-enable sorting
    ui->logsTable->setCurrentCell(Empty, Empty);
    ui->logsTable->setSortingEnabled(true);
}


void AdminDashboard::on_RefreshBtn_3_clicked()
{
    refreshLogsList();
}


void AdminDashboard::on_logFilter_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        ui->filterLogType->show();
        ui->logDate->hide();
        ui->logKeywordText->hide();
        break;

    case 1:
        ui->filterLogType->hide();
        ui->logDate->show();
        ui->logKeywordText->hide();
        break;

    default:
        ui->filterLogType->hide();
        ui->logDate->hide();
        ui->logKeywordText->show();
        break;
    }
}


void AdminDashboard::on_logFilterButton_clicked()
{
    QString filter = ui->logFilter->currentText().toLower(),
        keyword = "";

    if (filter == "type")
    {
        keyword = ui->filterLogType->currentText();
    }

    else if (filter == "date")
    {
        QDate selectedDate = ui->logDate->date();
        keyword = selectedDate.toString("MM/dd/yyyy");
    }

    else
    {
        keyword = ui->logKeywordText->text();
    }

    refreshLogsList(filter, keyword);

}


void AdminDashboard::on_RefreshEmpBtn_clicked()
{
    refreshEmployeeList();
}


void AdminDashboard::on_refreshItemsButton_clicked()
{
    refreshItemList();
}

