#include "iteminputdialog.h"

#include <QPushButton>

ItemInputDialog::ItemInputDialog(QWidget *parent)
    : QDialog(parent)
{
    // Create the input widgets
    m_itemNameInput = new QLineEdit(this);
    m_itemPriceInput = new QDoubleSpinBox(this);
    m_itemPriceInput->setRange(0.0, 9999.99);
    m_itemPriceInput->setDecimals(2);
    m_itemPriceInput->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_itemPriceInput->setStyleSheet("QDoubleSpinBox{border-radius: 5px;}");
    m_itemNameInput->setStyleSheet("QLineEdit{border-radius: 5px;}");

    // Create the layout and add the widgets
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("Item Name:"), this));
    layout->addWidget(m_itemNameInput);
    layout->addWidget(new QLabel(tr("Item Price:"), this));
    layout->addWidget(m_itemPriceInput);

    // Create the buttons and connect them to the accept/reject slots
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    QPushButton *okButton = buttons->button(QDialogButtonBox::Ok);
    okButton->setStyleSheet("QPushButton{color: white;background-color: #0e67ed;border-radius: 5px;padding: 5px;}");
    QPushButton *cancelButton = buttons->button(QDialogButtonBox::Cancel);
    cancelButton->setStyleSheet("QPushButton{color: white;background-color: #0e67ed;border-radius: 5px;padding: 5px;}");
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);

    // Set the dialog properties
    setWindowTitle(tr("Exit Item"));
}

QString ItemInputDialog::itemName() const
{
    return m_itemNameInput->text();
}

double ItemInputDialog::itemPrice() const
{
    return m_itemPriceInput->value();
}


