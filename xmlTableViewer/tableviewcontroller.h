#ifndef TABLEVIEWCONTROLER_H
#define TABLEVIEWCONTROLER_H

#include <QWidget>
#include <QtWidgets>
#include "tablemodel.h"

class TableViewController : public QWidget
{
    Q_OBJECT
public:
    explicit TableViewController(QWidget *parent = nullptr);

    void createUI();
signals:

private:
    void importData();
    void clear();
    void customMenuRequest();
    void editRecord();

private:
    TableModel* m_pModel;
    QTableView* m_pTableView;
};

#endif // TABLEVIEWCONTROLER_H
