#ifndef DATABASELITE_H
#define DATABASELITE_H

#include <QObject>

class DataBaseLite : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseLite(QObject *parent = nullptr);

signals:

};

#endif // DATABASELITE_H
