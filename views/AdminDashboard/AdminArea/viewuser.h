#ifndef VIEWUSER_H
#define VIEWUSER_H

#include <AdminDashboard>
#include <Database>

#include <QCloseEvent>
#include <QWidget>

namespace Ui {
class ViewUser;
}

class ViewUser : public QWidget
{
    Q_OBJECT

public:
    explicit ViewUser(QWidget *parent = nullptr);
    ~ViewUser();

    void setAdminDash(QWidget *ad);
    void setUsername(QString username);

private slots:
    void on_CloseBtn_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::ViewUser *ui;
    Account account_object;
    QWidget *parent_widget;
    QString old_name, old_firstname, old_lastname, old_phone, old_address, old_email, old_role;
};

#endif // VIEWUSER_H
