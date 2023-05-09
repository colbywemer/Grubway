#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class HistoryDialog;
}

class HistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryDialog(bool details = false, QWidget *parent = nullptr);
    ~HistoryDialog();

    void insertItemToEnd(QTableWidgetItem *item);
    void setTableTitle(QString title);

public slots:
    void viewDetails(const QModelIndex &index);

private:
    Ui::HistoryDialog *ui;
    QSharedPointer<HistoryDialog> m_detailView;

    int column = 0;

    bool m_isDetails;
};

#endif // HISTORYDIALOG_H
