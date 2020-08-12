#include "databaselite.h"

static const char* DATABASE_NAME = "xmlDatabase.db";
static const char* DATABASE_HOSTNAME = "XMLDatabase";
static const char* XMLTABLE = "xmlTable";

DataBaseLite::DataBaseLite(QObject *parent) : QObject(parent)
{

}

DataBaseLite::~DataBaseLite()
{
    if(m_db.isOpen()){
        this->close();
    }
}
/**
 * @brief Open DataBase SqlLite
 *
 * @return
 */
bool DataBaseLite::open()
{
    if(m_db.isOpen())
        return true;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setHostName(DATABASE_HOSTNAME);
    m_db.setDatabaseName(DATABASE_NAME);
    if(m_db.open())
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief DataBaseLite close
 * @return void
 */
void DataBaseLite::close()
{
    m_db.close();
}

/**
 * @brief DataBaseLite::createDB
 *        Create DataBase table with columns names
 * @param columnNames - Vector names of columns
 * @return
 */
bool DataBaseLite::createDB(const QVector<QString>& columnNames)
{
    /*If table exists, don't need to create*/
    if(isTableExist())
        return true;

    /*Try to open before create table*/
    if(this->open())
    {
        QString createQuery = "CREATE TABLE IF NOT EXISTS " + QString(XMLTABLE) + "( ";
        createQuery += "id INTEGER PRIMARY KEY AUTOINCREMENT, ";
        for(auto& name : columnNames)
        {
            createQuery += name + " VARCHAR(255)    NOT NULL,";
        }
        createQuery +=" )";
        ;
        createQuery.replace(createQuery.lastIndexOf(','),1, ' ');

        QSqlQuery query;
        if(query.exec(createQuery))
        {
            m_bTableExists = true;
            return true;
        }
        else
        {
            qDebug() << "DATABASELITE::CREATE_DATABASE::ERROR" << query.lastError();
            return false;
        }
    }
    else
    {
        qDebug() << "DATABASELITE::CREATE_DATABASE::ERROR";
        return false;
    }
}

/**
 * @brief DataBaseLite::connectDB
 *        Check exsisting SQLite DataBase with DATABASE_NAME
 *        and open it, then select all data from table
 * @return true - if exists, false if not (DataBase will be create when first data coming from outside)
 */
bool DataBaseLite::connectDB()
{
    auto path = QDir::currentPath() + '\\' +DATABASE_NAME;
    if(QFile(path).exists())
    {
        if(this->open()){
            m_bTableExists = this->checkTableExists(XMLTABLE);
            this->selectAll();
            return true;
        }
    }

    return false;
}

/**
 * @brief DataBaseLite::isOpen
 * @return true or false
 */
bool DataBaseLite::isOpen() const
{
    return m_db.isOpen();
}

/**
 * @brief DataBaseLite::isTableExist
 * @return true or false
 */
bool DataBaseLite::isTableExist() const
{
    return m_bTableExists;
}

/**
 * @brief DataBaseLite::checkTableExists
 *        Checks if a table with input name exists
 *        in DataBase
 * @param name - name of talbe
 * @return true or false
 */
bool DataBaseLite::checkTableExists(const QString& name)
{
    QSqlQuery query;
    query.prepare(QString("SELECT name FROM SQLITE_MASTER WHERE TYPE = 'TABLE' AND name = '%1'").arg(name));

    if(!query.exec())
    {
        qDebug() << "DATABASELITE::EXISTSTABLE:ERROR" << query.lastError();
        return false;
    }

    if(query.record().count()) //SqlQuery has member that means table exists
        return true;
    return false;
}


/**
 * @brief DataBaseLite::selectAll
 *        SELECT all data from DataBase with XMLTABLE name
 *        Signal every time as row readed
 */
void DataBaseLite::selectAll()
{
    if(!this->isTableExist())
        return;
    QVector<QString> columnNames;
    QSqlQuery query;
    query.prepare("SELECT * FROM " + QString(XMLTABLE));

    if(!query.exec())
    {
        qDebug() << "DATABASELITE::SELECT:ERROR" << query.lastError();
    }

    QSqlRecord rec = query.record();
    auto columns = rec.count();
    for(int i = 1; i < columns; ++i)
    {
        columnNames.append(rec.fieldName(i));
    }
    while(query.next())
    {
        QList<QPair<QString, QVariant>> file;
        for(int i = 1; i < columns; ++i)
            file.append(qMakePair(columnNames[i-1],query.value(i)));
        emit dataSelected(file);
    }
}


/**
 * @brief DataBaseLite::insertIntoTable
 *        SqlQuery to insert new row in DataBase
 * @param record - row of parameters
 * @return true(if inserted), false(if not)
 */
bool DataBaseLite::insertIntoTable(const QList<QPair<QString, QVariant>>& record)
{
    if(!this->isTableExist())
    {
        QVector<QString> columns;
        for(auto& pair : record)
        {
            columns.append(pair.first);
        }
        if(!this->createDB(columns))
            return false;
    }

    QSqlQuery query;
    QString reqInto =   "INSERT INTO " + QString(XMLTABLE) + "( ";
    QString reqValues = " VALUES ( ";

    for(auto &pair : record)
    {
        reqInto += pair.first + ", ";
        reqValues += ":"+pair.first + ", ";
    }
    reqInto += " ) ";
    reqValues += " ) ";
    reqInto.replace(reqInto.lastIndexOf(","),1,' ');
    reqValues.replace(reqValues.lastIndexOf(","),1,' ');

    auto req = reqInto+reqValues;
    query.prepare(req);

    for(auto& pair : record)
    {
        query.bindValue(":"+pair.first, pair.second);
    }

    if(!query.exec())
    {
        qDebug() << "DATABASELITE::INSERT::ERROR" << query.lastError();
        return false;
    }


    return true;
}


/**
 * @brief DataBaseLite::updateIntoTable
 *        QSqlQuery to update cell in table of DataBase
 * @param row - number row to update
 * @param key - name of column
 * @param value - update value
 * @return
 */
bool DataBaseLite::updateIntoTable(int row, const QString& key, const QVariant& value)
{
    if(!this->isTableExist())
        return false;

    QSqlQuery query;
    //id = row + 1; because row in model started with 0 but id in table with 1
    QString req =   QString("UPDATE %1 SET %2 = '%3' WHERE id = %4").arg(XMLTABLE).arg(key).arg(value.toString()).arg(row+1);

    query.prepare(req);

    if(!query.exec())
    {
        qDebug() << "DATABASELITE::UPDATE::ERROR" << query.lastError();
        return false;
    }


    return true;
}


/**
 * @brief DataBaseLite::removeRow
 *        QSqlQuery to remove row from DataBase
 * @param row - number of row to remove
 * @return
 */
bool DataBaseLite::removeRow(int row)
{
    if(!this->isTableExist())
        return false;

    QSqlQuery query;
    //id = row + 1; because row in model started with 0 but id in table with 1
    QString req =   QString("DELETE FROM %1 WHERE id = %4").arg(XMLTABLE).arg(row+1);

    query.prepare(req);

    if(!query.exec())
    {
        qDebug() << "DATABASELITE::UPDATE::ERROR" << query.lastError();
        return false;
    }


    return true;
}


/**
 * @brief DataBaseLite::clear
 *        QSqlQuery to clear all data from table including primary key sequence
 */
void DataBaseLite::clear()
{
    QSqlQuery query;
    query.prepare("DELETE FROM " + QString(XMLTABLE));

    if(!query.exec())
    {
        qDebug() << "DATABASELITE::DELETE:ERROR" << query.lastError();
    }

    auto req = QString("DELETE FROM SQLITE_SEQUENCE WHERE name='%1'").arg(XMLTABLE);
    query.prepare(req);

    if(!query.exec())
    {
        qDebug() << "DATABASELITE::DELETE:ERROR" << query.lastError();
    }
}


/**
 * @brief DataBaseLite::deleteTable
 *        QSqlQuery to DROP(delete) XMLTABLE from DataBase
 * @return
 */
bool DataBaseLite::deleteTable()
{
    QSqlQuery query;
    if(query.exec(QString("DROP TABLE IF EXISTS %1").arg(XMLTABLE)))
    {
        this->close();
        m_bTableExists = false;
        return true;
    }
    else
    {
        qDebug() << "DATABASELITE::DELETE::ERROR" << query.lastError();
        return false;
    }
}
