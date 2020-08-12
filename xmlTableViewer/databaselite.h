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
public slots:
    bool insertIntoTable(const QList<QPair<QString, QVariant>>& record);
    bool updateIntoTable(int row, const QString& key, const QVariant& value);
    bool removeRow(int row);
    bool deleteDataBase();

private:
    bool open();
    void close();
    bool checkTableExists(const QString& name);
    bool isTableExist() const;
    bool isOpen() const;
    bool createDB(const QVector<QString>& columnNames);
    void selectAll();
    void clear();

private:
    QSqlDatabase m_db;
    bool m_bTableExists = false;
};

#endif // DATABASELITE_H
