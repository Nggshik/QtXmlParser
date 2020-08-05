#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QtWidgets>

#include "progressimport.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(QObject* pobj = 0);
    virtual ~TableModel(){;};

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
//    bool insertRows(int row, int count, const QModelIndex& parent);
    bool appendFile(QHash<QString, QVariant>&& file);
    int parseXML(const QString& directoryPath);


public slots:
    void removeSelected();
    void clear();
private:
    QHash<QString, QVariant> parseXMLfile(const QString &filePath);
    void pushProgressError(const QString& err);
private:
    typedef  QHash<QString, QVariant> FileData;
    typedef  QList<FileData> Files;
    Files m_files;
    QVector<QString> m_keys;
    ProgressImport* m_pProgress = nullptr;
    QMutex m_mutex;
};

#endif // TABLEMODEL_H
