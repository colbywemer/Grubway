#ifndef ITEMINPUTDIALOG_H
#define ITEMINPUTDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

class ItemInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ItemInputDialog(QWidget *parent = nullptr);

    QString itemName() const;

    QLineEdit *m_itemNameInput;
    QDoubleSpinBox *m_itemPriceInput;

    double itemPrice() const;
};

#endif // ITEMINPUTDIALOG_H
