#ifndef ADDITEM_H
#define ADDITEM_H

#include <AdminDashboard>
#include <Database>
#include <Item>

#include <QMainWindow>

namespace Ui {
class addItem;
}

class addItem : public QMainWindow
{
    Q_OBJECT

public:
    explicit addItem(QWidget *parent = nullptr);
    ~addItem();

    void setAdminDash(AdminDashboard *ad);
    void setAdminAccount(Account account);
    void setItemId(int id);

private slots:
    void on_CancelBtn_clicked();
    void on_department_currentTextChanged(const QString &);
    void on_OKBtn_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::addItem *ui;
    Item item_object;
    Account adminAccount;
    AdminDashboard *parent_widget = nullptr;
};

#endif // ADDITEM_H
