#include "tablemodel.h"

TableModel::TableModel(QObject* pobj) : QAbstractTableModel(pobj)
{

}

int TableModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid()) return 0; //no children
    return m_files.size();
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
               : m_files[index.row()][m_keys[index.column()]];
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        m_files[index.row()][m_keys[index.column()]] = value;
        emit dataChanged(index, index);
        emit cellDataChanged(index.row()+1,m_keys[index.column()], value);
        return true;
    }
    return false;
}

//bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
//{
//    if(parent.isValid())
//    {
//        return false;
//    }

//    beginInsertRows(QModelIndex(), row, row+count-1);
//    for(int i = 0; i < count; ++i)
//    {
//       //TODO
//    }
//}

bool TableModel::appendFile(const QHash<QString, QVariant>& file)
{
    if(m_keys.isEmpty())
    {
        auto first = m_keys.count();
        auto last = m_keys.count() + file.keys().size()-1;
        beginInsertColumns(QModelIndex(),first, last);
        for(auto& key : file.keys())
            m_keys.append(key);
        endInsertColumns();
        emit headerDataChanged(Qt::Horizontal,first, last);
        emit tableCreated(m_keys);
    }

    int row = m_files.count();

    beginInsertRows(QModelIndex(), row,row);
    m_files.append(file);
    endInsertRows();

    return true;
}

void TableModel::removeSelected()
{
    ; //TODO
}

//void TableModel::connectDB()
//{
//    const QMutexLocker locker(&m_mutex);
//    if(m_db->connectDB())
//    {
//        auto files = m_db->selectAll();
//        int first = m_files.count();
//        int last = m_files.count() + files.count() - 1;
//        beginInsertRows(QModelIndex(), first,last);
//        m_files.append(files);
//        endInsertRows();

//        if(m_keys.isEmpty() && m_files.count())
//        {
//            auto first = m_keys.count();
//            auto last = m_keys.count() + m_files[0].keys().size()-1;
//            beginInsertColumns(QModelIndex(),first, last);
//            for(auto& key : m_files[0].keys())
//                m_keys.append(key);
//            endInsertColumns();
//            emit headerDataChanged(Qt::Horizontal,first, last);
//        }
//    }
//}


void TableModel::clear()
{
    if(m_files.isEmpty()){
        Q_ASSERT(m_keys.isEmpty());
        return;
    }
    beginRemoveRows(QModelIndex(), 0,m_files.size()-1);
    m_files.clear();
    endRemoveRows();

    emit cleared();
//    beginRemoveColumns(QModelIndex(),0, m_keys.count()-1);
//    m_keys.clear();
//    endRemoveColumns();
}


