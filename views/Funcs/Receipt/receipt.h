#ifndef RECEIPT_H
#define RECEIPT_H

#include <CashierScreen>

#include <QString>
#include <QDateTime>
#include <QList>

class ReceiptItem {
public:
    ReceiptItem(const QString& itemName, float itemPrice, int itemCount);

    QString getItemName() const;

    float getItemPrice() const;
    float getItemTotal() const;

    int getItemCount() const;


private:
    QString m_itemName;
    float m_itemPrice;
    int m_itemCount;
};

class Receipt {
public:
    Receipt(const QDateTime& dateTime, const QList<ReceiptItem>& items);

    QDateTime getDateTime() const;
    QList<ReceiptItem> getItems() const;
    float getTotal() const;
    int getId() const;

private:
    int id;
    QDateTime m_dateTime;
    QList<ReceiptItem> m_items;
};

#endif // RECEIPT_H
