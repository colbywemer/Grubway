#include "inventoryscreen.h"
#include "ui_inventoryscreen.h"

#include <common.h>

#include <Database>

#include <QInputDialog>

using namespace std;

// Define the column header labels for the inventory table
QStringList Inventory_Table_Items =
{
    "Id",
    "Item",
    "Stock",
    "Sold"
};

// Define constants for column indices and sizes
int Top_Ten = 10,
    Id_Col_Count = 0, Item_Col_Count = 1, Stock_Col_Count = 2, Sold_Col_Count = 3, column_size = 217, extra_column_size = 218;

// InventoryScreen constructor
InventoryScreen::InventoryScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InventoryScreen),
    totalImported(0),
    totalSold(0)
{
    // Set up the user interface for the InventoryScreen
    ui->setupUi(this);

    // Set the column headers and widths for the inventory table
    ui->inventoryTableWidget->setHorizontalHeaderLabels(Inventory_Table_Items);
    ui->inventoryTableWidget->setColumnWidth(Id_Col_Count, extra_column_size);
    ui->inventoryTableWidget->setColumnWidth(Item_Col_Count, column_size);
    ui->inventoryTableWidget->setColumnWidth(Stock_Col_Count, column_size);
    ui->inventoryTableWidget->setColumnWidth(Sold_Col_Count, column_size);

    // Hide filter input fields initially
    ui->filterItemName->hide();
    ui->filterItemDepartment->hide();

    // Update the inventory labels and data
    updateInventoryLabels();
    updateInventoryData();

    // Set the window title for the InventoryScreen
    setWindowTitle("Inventory Screen");
}

// InventoryScreen destructor
InventoryScreen::~InventoryScreen()
{
    // Delete the user interface object
    delete ui;
}

// Set the parent AdminDashboard widget
void InventoryScreen::setAdminDash(AdminDashboard *ad)
{
    parent_widget = ad;
}

// Set the user account
void InventoryScreen::setAccount(Account account)
{
    userAccount = account;
}

// Update the inventory data based on the provided filter and keyword
void InventoryScreen::updateInventoryData(QString filter, QString keyword)
{
    // Disable sorting while updating the table data
    ui->inventoryTableWidget->setSortingEnabled(false);

    // Fetch the inventory data from the database
    vector<Item> items;
    if (filter == "" || keyword == "")
    {
        items = selectAllItems();
    }else{
        items = selectAllItemsByFilter(filter, keyword);
    }

    // Initialize variables for item count and empty value
    int itemCount = 0;
    int Empty = 0;

    // Reset the total imported and total sold quantities
    totalImported = 0;
    totalSold = 0;

    // Clear the current inventory model
    if (ui->inventoryTableWidget->rowCount() > Empty)
    {
        for (int count = ui->inventoryTableWidget->rowCount(); count > Empty; count--)
        {
            ui->inventoryTableWidget->removeRow(count + Decrement);
        }
    }

    // Update the inventory model with the fetched data
    for (const Item& item : items) {
        // Create a new row in the inventory model with the item, imported quantity, and sold quantity
        QTableWidgetItem *item1 = new QTableWidgetItem(),
            *item2 = new QTableWidgetItem(item.name),
            *item3 = new QTableWidgetItem(),
            *item4 = new QTableWidgetItem();

        item1->setData(Qt::DisplayRole, QVariant(item.id));
        item3->setData(Qt::DisplayRole, QVariant(item.stock));
        item4->setData(Qt::DisplayRole, QVariant(item.sold));

        ui->inventoryTableWidget->insertRow(itemCount);
        ui->inventoryTableWidget->setItem(itemCount, Id_Col_Count, item1);
        ui->inventoryTableWidget->setItem(itemCount, Item_Col_Count, item2);
        ui->inventoryTableWidget->setItem(itemCount, Stock_Col_Count, item3);
        ui->inventoryTableWidget->setItem(itemCount, Sold_Col_Count, item4);

        itemCount++;

        // Highlight the row if the imported count is less than 10
        if (item.stock < Top_Ten)
        {
            for (int column = 0; column < ui->inventoryTableWidget->columnCount(); column++)
            {
                QTableWidgetItem *item = ui->inventoryTableWidget->item(ui->inventoryTableWidget->rowCount() +
                    Decrement, column);

                QBrush brush(Qt::red);
                item->setBackground(brush);
            }
        }

        // Update the total imported and total sold quantities
        totalImported += item.stock;
        totalSold += item.sold;
    }

    // Enable sorting after updating the table data
    ui->inventoryTableWidget->setSortingEnabled(true);

    // Update the UI labels after updating the data
    updateInventoryLabels();
}

// Handle the backButton click event
void InventoryScreen::on_backButton_clicked()
{
    // Return to cashier screen
    close();
    if (parent_widget)
    {
        parent_widget->refreshTablesData();
    }
}

// Update the inventory labels with the current total imported and total sold quantities
void InventoryScreen::updateInventoryLabels()
{
    ui->totalImportedLabel->setText(QString("Total Stock: %1").arg(totalImported));
    ui->totalSoldLabel->setText(QString("Total Sold: %1").arg(totalSold));
}

// Handle the updateStockButton click event
void InventoryScreen::on_updateStockButton_clicked()
{
    int row = ui->inventoryTableWidget->currentRow();

    if (row != Decrement)
    {
        QTableWidgetItem *SelectedItemWidget = ui->inventoryTableWidget->item(row, column);

        int itemId = SelectedItemWidget->text().toInt();

        Item item = selectItemById(itemId);

        bool ok;

        item.stock = QInputDialog::getDouble(this, tr("Update Stock"), tr("Enter new stock value:"), item.stock, Empty,
            numeric_limits<double>::max(), 2, &ok);

        if (ok)
        {
            updateItem(item);

            Logs log;
            log.time = QDateTime::currentDateTime();
            log.account = userAccount;
            log.action = "Updated Stock For '" + item.name + "'";
            log.type = "Item";
            createLog(log);

            updateInventoryData();
        }
    }
}

// Handle the itemFilterButton click event
void InventoryScreen::on_itemFilterButton_clicked()
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

    updateInventoryData(filter, keyword);
}

// Handle the itemFilter comboBox index change event
void InventoryScreen::on_itemFilter_currentIndexChanged(int index)
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

// Handle the refreshButton click event
void InventoryScreen::on_refreshButton_clicked()
{
    updateInventoryData();
}

// Handle the closeEvent for the InventoryScreen
void InventoryScreen::closeEvent(QCloseEvent *event)
{
    if (parent_widget)
    {
        parent_widget->refreshTablesData();
    }

    event->accept();
}

