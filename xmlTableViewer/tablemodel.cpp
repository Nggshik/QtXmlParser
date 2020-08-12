#include "tablemodel.h"

TableModel::TableModel(QObject* pobj) : QAbstractTableModel(pobj)
{

}

int TableModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid()) return 0; //no children
    return m_files.count();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid()) return 0; //no children
    return m_keys.count();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Vertical)
    {
        return section;
    }

    return section < m_keys.size() ? m_keys[section] : QString("Unnamed");
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    return index.isValid() ? (flags | Qt::ItemIsEditable) : flags;
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    return (role != Qt::DisplayRole && role != Qt::EditRole)
               ? QVariant()
               : m_files[index.row()][index.column()].second;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        m_files[index.row()][index.column()].second = value;
        emit dataChanged(index, index);
        emit cellDataChanged(index.row()+1,m_keys[index.column()], value);
        return true;
    }
    return false;
}

bool TableModel::appendFile(const QList<QPair<QString, QVariant>>& file)
{
    if(m_keys.isEmpty())
    {
        auto first = m_keys.count();
        auto last = m_keys.count() + file.size()-1;
        beginInsertColumns(QModelIndex(),first, last);
        for(auto& key : file)
            m_keys.append(key.first);
        endInsertColumns();
        emit headerDataChanged(Qt::Horizontal,first, last);
    }

    int row = m_files.count();

    beginInsertRows(QModelIndex(), row,row);
    m_files.append(file);
    endInsertRows();
    return true;
}

bool TableModel::removeRow(int row, const QModelIndex& parent)
{
    Q_UNUSED(parent);

    beginRemoveRows(QModelIndex(), row,row);
    m_files.removeAt(row);
    endRemoveRows();
    emit rowRemoved(row);
    return true;
}



void TableModel::clear()
{
    if(m_files.isEmpty()){
        Q_ASSERT(m_keys.isEmpty());
        return;
    }
    beginRemoveRows(QModelIndex(), 0,m_files.size()-1);
    m_files.clear();
    endRemoveRows();


    beginRemoveColumns(QModelIndex(),0, m_keys.count()-1);
    m_keys.clear();
    endRemoveColumns();

    emit cleared();
}


