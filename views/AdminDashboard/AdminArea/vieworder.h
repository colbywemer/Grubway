#ifndef VIEWORDER_H
#define VIEWORDER_H

#include <Database>

#include <QCloseEvent>
#include <QWidget>

namespace Ui {
class ViewOrder;
}

class ViewOrder : public QWidget
{
    Q_OBJECT

public:
    explicit ViewOrder(QWidget *parent = nullptr);
    ~ViewOrder();

    void setAdminDash(QWidget *ad);
    void setID(int ID);

private slots:
    void on_CloseBtn_clicked();
    void on_updateStatus_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::ViewOrder *ui;
    Order order_object;
    QWidget *parent_widget;

    QString old_name, old_firstname, old_lastname, old_phone, old_address, old_email;

    double old_total;
};

#endif // VIEWORDER_H
