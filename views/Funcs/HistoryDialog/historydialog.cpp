#include "historydialog.h"
#include "ui_historydialog.h"

#include <Database>

#include <common.h>

QStringList table_list =
{
    "Datetime",
};

HistoryDialog::HistoryDialog(bool details, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoryDialog),
    m_isDetails(details)
{
    ui->setupUi(this);

    ui->historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->historyTable->setWordWrap(true);
    ui->historyTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->historyTable->setHorizontalHeaderLabels(table_list);
    ui->historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setWindowTitle("History");

    if (!m_isDetails)
    {
        QObject::connect(ui->historyTable, &QTableWidget::doubleClicked, this, &HistoryDialog::viewDetails);
    }
}

HistoryDialog::~HistoryDialog()
{
    delete ui;
}

void HistoryDialog::insertItemToEnd(QTableWidgetItem *item)
{
    int current_post = ui->historyTable->rowCount();

    ui->historyTable->insertRow(current_post);
    ui->historyTable->setItem(current_post, column, item);
}

void HistoryDialog::viewDetails(const QModelIndex &index)
{
    m_detailView = QSharedPointer<HistoryDialog>(new HistoryDialog(true));

    QString history = "";
    double totalPrice = 0.0;

    vector<Receipts> receipts = selectAllRecipts();

    QString item_date = ui->historyTable->model()->data(index).toString();

    m_detailView->setTableTitle(receipts.at(Empty).datetime.date().toString());

    // Add each order to the list widget
    for (const auto& receipt : receipts)
    {
        QString current_item_date = receipt.datetime.date().toString();

        if (QString::compare(current_item_date, item_date, Qt::CaseInsensitive) == Empty)
        {
            QString orderText = "Order #" + QString::number(receipt.id) +
                " - " + receipt.account.username + "\n\r";

            history = orderText;

            for (const auto& pair : receipt.items)
            {
                QString item = pair.first;
                int quantity = pair.second;

                double price = 0.0;

                auto it = receipt.prices.find(item);

                if (it != receipt.prices.end())
                {
                    price = it->second;
                }

                history += item + " X " + QString::number(quantity) + " -$" + QString::number(price) + "\n";
            }

            totalPrice += receipt.total;

            QTableWidgetItem *item1 = new QTableWidgetItem();
            item1->setText(history);

            m_detailView->insertItemToEnd(item1);
        }
    }

    double tax_price = totalPrice * (TAX_RATE / 100.0);
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText("\nTax: $" + QString::number(tax_price) + "\n" +
        "Total Price: $" + QString::number(totalPrice + tax_price));

    m_detailView->insertItemToEnd(item);
    m_detailView->exec();
}

void HistoryDialog::setTableTitle(QString title)
{
    ui->historyTable->setHorizontalHeaderLabels(QStringList(title));
}
