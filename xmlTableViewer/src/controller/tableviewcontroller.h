#ifndef TABLEVIEWCONTROLER_H
#define TABLEVIEWCONTROLER_H

//System include
#include <QWidget>
#include <QtWidgets>

//User include
#include "tablemodel.h"
#include "dialogmapper.h"
#include "xmlparser.h"
#include "databaselite.h"
#include "progressimport.h"

class TableViewController : public QWidget
{
    Q_OBJECT
public:
    explicit TableViewController(QWidget *parent = nullptr);
    virtual ~TableViewController();
    void createUI();
signals:
    void importXML(const QString& dir);
private:
    void importData();
    void clear();
    void customContextMenuRequested(QPoint position);
    void editRecord();
    void exportRecord();
    void removeRow();

private:
    TableModel* m_pModel;
    QTableView* m_pTableView;
    DialogMapper* m_pMapper;
    XmlParser* m_pXmlParser;
    DataBaseLite* m_pDataBase;
    ProgressImport* m_pProgress;
};

#endif // TABLEVIEWCONTROLER_H
