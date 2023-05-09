#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QProcess>
#include <QWidget>

namespace Ui {
class AdminDashboard;
}

class AdminDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit AdminDashboard(QWidget *parent = nullptr);
    ~AdminDashboard();

    void refreshItemList(QString filter = "", QString keyword = "");
    void refreshOrderList(QString filter = "", QString keyword = "");
    void refreshCustomerList(QString filter = "", QString keyword = "");
    void refreshEmployeeList(QString filter = "", QString keyword = "");
    void refreshLogsList(QString filter = "", QString keyword = "");
    void refreshTablesData();
    void setUsrname(QString usrname);

private slots:
    void on_ChangeCodeBtn_clicked();
    void on_ChgNameBtn_clicked();
    void on_ChgPasswdBtn_clicked();
    void on_createItemButton_clicked();
    void on_CreateUsrBtn_clicked();
    void on_custFilter_currentIndexChanged(int index);
    void on_DelCustUsrBtn_clicked();
    void on_DelEmpUsrBtn_clicked();
    void on_deleteItemButton_clicked();
    void on_DelOrderBtn_clicked();
    void on_editItemButton_clicked();
    void on_EditUsrBtn_clicked();
    void on_empFilter_currentIndexChanged(int index);
    void on_EndSessionBtn_clicked();
    void on_filter_currentIndexChanged(int index);
    void on_filterCustomerButton_clicked();
    void on_filterEmpButton_clicked();
    void on_inventoryButton_clicked();
    void on_itemFilterButton_clicked();
    void on_itemFilter_currentIndexChanged(int index);
    void on_logFilter_currentIndexChanged(int index);
    void on_logFilterButton_clicked();
    void on_orderFilterButton_clicked();
    void on_RefreshBtn_clicked();
    void on_RefreshBtn_3_clicked();
    void on_RefreshEmpBtn_clicked();
    void on_RefreshOrderBtn_clicked();
    void on_ResetPassBtn_clicked();
    void on_ViewCustUsrBtn_clicked();
    void on_ViewEmpUsrBtn_clicked();
    void on_ViewOrderBtn_clicked();
    void closeEvent(QCloseEvent *event);

    void on_refreshItemsButton_clicked();

private:
    Ui::AdminDashboard *ui;
    QString admin_username;

    void deleteTheItem(int id);
    void deleteTheOrder(int id);
    void deleteUserAccount(QString username);
    void viewUserAccount(QString username);
    void viewOrder(QString id);

    int Timer_Ticks = 600000, timerId = startTimer(Timer_Ticks), column = 2, orderColumn = 0;

protected:
    void timerEvent(QTimerEvent *event);
};

#endif // ADMINDASHBOARD_H
