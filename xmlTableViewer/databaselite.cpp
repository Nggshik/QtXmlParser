#include "databaselite.h"

static const char* DATABASE_NAME = "xmlDatabase.db";
static const char* DATABASE_HOSTNAME = "XMLDatabase";
static const char* XMLTABLE = "xmlTable";

DataBaseLite::DataBaseLite(QObject *parent) : QObject(parent)
{

}

DataBaseLite::~DataBaseLite()
{
    if(m_connected){
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
    if(m_connected)
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
 * @brief DataBaseLite::close
 * @return void
 */
void DataBaseLite::close()
{
    m_db.close();
    m_connected = false;
}


bool DataBaseLite::createDB(const QVector<QString>& columnNames)
{
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
            m_connected = true;
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

bool DataBaseLite::connectDB()
{
    auto path = QDir::currentPath() + '\\' +DATABASE_NAME;
    if(QFile(path).exists())
    {
        if(this->open()){
             m_connected = true;
            this->selectAll();
            return true;
        }
    }

    return false;
}

bool DataBaseLite::isConnected() const
{
    return m_connected;
}

void DataBaseLite::selectAll()
{

    QVector<QString> columnNames;
    QSqlQuery query;
    query.prepare("SELECT * FROM " + QString(XMLTABLE));

    if(!query.exec())
    {
        qDebug() << "DATABASELITE::SELECT:ERROR";
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

bool DataBaseLite::insertIntoTable(const QList<QPair<QString, QVariant>>& record)
{
    if(!this->isConnected())
        return false;

    QSqlQuery query;
    QString reqInto =   "INSERT INTO " + QString(XMLTABLE) + "( ";
    QString reqValues = " VALUES ( ";
    /* + *keyIterator + " ) "
                        " VALUES ( :" + *keyIterator + " )";*/
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
        qDebug() << "DATABASELITE::INSERT::ERROR";
        return false;
    }


    return true;
}

bool DataBaseLite::updateIntoTable(int id, const QString& key, const QVariant& value)
{
    if(!this->isConnected())
        return false;

    QSqlQuery query;
    QString req =   QString("UPDATE %1 SET %2 = '%3' WHERE id = %4").arg(XMLTABLE).arg(key).arg(value.toString()).arg(id);

    query.prepare(req);

    if(!query.exec())
    {
        qDebug() << "DATABASELITE::UPDATE::ERROR" << query.lastError();
        return false;
    }


    return true;
}

bool DataBaseLite::removeRow(int row)
{
    if(!this->isConnected())
        return false;

    QSqlQuery query;
    QString req =   QString("DELETE FROM %1 WHERE id = %4").arg(XMLTABLE).arg(row+1);

    query.prepare(req);

    if(!query.exec())
    {
        qDebug() << "DATABASELITE::UPDATE::ERROR" << query.lastError();
        return false;
    }


    return true;
}


void DataBaseLite::clear()
{
    QSqlQuery query;
    query.prepare("DELETE FROM " + QString(XMLTABLE));

    if(!query.exec())
    {
        qDebug() << "DATABASELITE::DELETE:ERROR";
    }

    auto req = QString("DELETE FROM SQLITE_SEQUENCE WHERE name='%1'").arg(XMLTABLE);
    query.prepare(req);

    if(!query.exec())
    {
        qDebug() << "DATABASELITE::DELETE:ERROR";
    }
}

bool DataBaseLite::deleteDataBase()
{
    QSqlQuery query;
    if(query.exec(QString("DROP TABLE IF EXISTS %1").arg(XMLTABLE)))
    {
        this->close();
        return true;
    }
    else
    {
        qDebug() << "DATABASELITE::DELETE::ERROR" << query.lastError();
        return false;
    }
}
