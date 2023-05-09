#ifndef CASHIERSCREEN_H
#define CASHIERSCREEN_H

#include <HistoryDialog>

#include <QCloseEvent>
#include <QListWidget>
#include <QSharedPointer>
#include <QWidget>

namespace Ui {
    class CashierScreen;
}

class CashierScreen : public QWidget
{
    Q_OBJECT

public:
    explicit CashierScreen(QWidget *parent = nullptr);
    ~CashierScreen();

    void setUsername(QString username);

private slots:
    void on_addItemButton_clicked();
    void on_cartListWidget_itemClicked(QListWidgetItem *item);
    void on_changePassword_clicked();
    void on_checkOutButton_clicked();
    void on_deleteItemButton_clicked();
    void on_editItemButton_clicked();
    void on_Exit_clicked();
    void on_ExitItem_clicked();
    void on_filter_currentIndexChanged(int index);
    void on_history_clicked();
    void on_logout_clicked();
    void on_profile_clicked();
    void on_Total_clicked();
    void on_viewCartButton_clicked();
    void closeEvent(QCloseEvent *event);

    void openInventoryScreen();


private:
    Ui::CashierScreen *ui;
    QListWidget *cartListWidget;

    QString username;
    QSharedPointer<HistoryDialog> m_historyView;

    void viewUserAccount(QString username);

    double dailyTotal = 0.0;
};

#endif // CASHIERSCREEN_H
