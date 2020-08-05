#ifndef TABLEVIEWCONTROLER_H
#define TABLEVIEWCONTROLER_H

#include <QWidget>
#include <QtWidgets>

#include "tablemodel.h"
#include "dialogmapper.h"

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
    void customContextMenuRequested(QPoint position);
    void editRecord();

private:
    TableModel* m_pModel;
    QTableView* m_pTableView;
    DialogMapper* m_pMapper;

};

#endif // TABLEVIEWCONTROLER_H
