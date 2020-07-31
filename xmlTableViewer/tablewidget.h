#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include <QtWidgets>

class TableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TableWidget(QWidget *parent = nullptr);
    virtual ~TableWidget();
    void fillTable(const QVector<QVector<QPair<QString, QString>>>& table);
signals:
    void importData();
private:
    QTableWidget* m_pTable;
};

#endif // TABLEWIDGET_H
