#ifndef DATABASELITE_H
#define DATABASELITE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QtCore>

class DataBaseLite : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseLite(QObject *parent = nullptr);
    virtual ~DataBaseLite();
signals:
    void dataSelected(const QList<QPair<QString, QVariant>>& record);
public:
    bool connectDB();
    bool insertIntoTable(const QList<QPair<QString, QVariant>>& record);
    bool updateIntoTable(int row, const QString& key, const QVariant& value);
    bool createDB(const QVector<QString>& columnNames);
    bool isConnected() const;
    void selectAll();
    bool removeRow(int row);
    void clear();
    bool deleteDataBase();
private:
    bool open();
    void close();


private:
    QSqlDatabase m_db;
    bool m_connected = false;
};

#endif // DATABASELITE_H
