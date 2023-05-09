#ifndef INVENTORYSCREEN_H
#define INVENTORYSCREEN_H

#include <AdminDashboard>
#include <Database>

#include <QStandardItemModel>
#include <QWidget>

namespace Ui {
class InventoryScreen;
}

class InventoryScreen : public QWidget
{
    Q_OBJECT

public:
    explicit InventoryScreen(QWidget *parent = nullptr);
    ~InventoryScreen();
    void setAccount(Account account);
    void setAdminDash(AdminDashboard *ad);

signals:
    void backToCashierScreen();

private slots:
    void on_backButton_clicked();
    void on_updateStockButton_clicked();

    void on_itemFilterButton_clicked();

    void on_itemFilter_currentIndexChanged(int index);

    void on_refreshButton_clicked();

    void closeEvent(QCloseEvent *event);

private:
    Ui::InventoryScreen *ui;
    QStandardItemModel *inventoryModel;

    AdminDashboard *parent_widget = nullptr;

    double totalImported, totalSold;

    int column = 0;

    Account userAccount;

    void updateInventoryData(QString filter = "", QString keyword = "");
    void updateInventoryLabels();
};

#endif // INVENTORYSCREEN_H
