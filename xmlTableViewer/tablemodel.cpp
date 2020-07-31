#include "tablemodel.h"

TableModel::TableModel(QObject* pobj) : QAbstractTableModel(pobj)
{

}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_files.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
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
    //Debug
    QVariant res;
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        auto row = index.row();
        auto column = index.column();
        auto strColumn = m_keys[column];
        res = m_files[row][strColumn];
        qDebug() << "Taked at row = " << row << "      Taked at column = " << column << "\nResult = " << res;
    }

    return res;

    return (role != Qt::DisplayRole && role != Qt::EditRole)
            ? m_files[index.row()][m_keys[index.column()]]
        : QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        m_files[index.row()][m_keys[index.column()]] = value;
        emit dataChanged(index, index);
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

bool TableModel::appendFile(QHash<QString, QVariant>&& file)
{
    int row = m_files.count();

        beginInsertRows(QModelIndex(), row,row);
        m_files.append(file);
        endInsertRows();
        if(!m_keys.count())
        {
            for(auto& key : file.keys())
                m_keys.append(key);
        }
        return true;

}

void TableModel::removeSelected()
{
    ; //TODO
}

int TableModel::parseXML(const QString& directoryPath)
{
    m_progress.show();

    QDir dir(directoryPath);
    QRegExp fileCheckRX("*.xml");
    fileCheckRX.setPatternSyntax(QRegExp::Wildcard);

    QFileInfoList filesInfo = dir.entryInfoList();
    m_progress.setProgressMax(filesInfo.size()-2);

    for(auto info : filesInfo)
    {
        if(info.isFile())
        {
            if(fileCheckRX.exactMatch(info.fileName()))
            {
                auto xmlParameters = parseXMLfile(info.filePath());
                if(xmlParameters.size()){
                    m_progress.okCountUp(xmlParameters.size());
                    appendFile(std::move(xmlParameters));
                }
            }
            else
            {
                pushProgressError(info.fileName());
            }
            m_progress.progressStepForward();
        }

    }

    return 0;
}

QHash<QString, QVariant> TableModel::parseXMLfile(const QString &filePath)
{
    QRegExp keyValueRX("<(\\w+)>(.+)<\\/(\\w+)>"); //<(\w+)>(.+)<\/(\g1)>
    keyValueRX.setPatternSyntax(QRegExp::RegExp2);

    QHash<QString, QVariant> hash;

    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {

        while(!file.atEnd() && !file.readLine().contains("<root>")); //Skip all lines befor root


        while (!file.atEnd()) {
            auto line = file.readLine();
            if(keyValueRX.indexIn(line) != -1)
            {
                auto caps = keyValueRX.capturedTexts();
                if((caps.size() == 4) && (caps[1] == caps[3]))
                {
                    hash.insert(caps[1],caps[2]);
                }
                else
                {
                    pushProgressError(file.fileName() + "\n At line -" + line);
                    hash.clear();
                    break;
                }
            }
            else
            {
                if(line.contains("</root>"))
                {
                    break;
                }
                else
                {
                    pushProgressError(file.fileName() + "\n At line -" + line);
                    hash.clear();
                    break;
                }
            }

        }
    }
    else
    {
        pushProgressError(file.fileName());
    }
    return  hash;
}

void TableModel::pushProgressError(const QString& err)
{
    qDebug() << "MODEL::FILE::ERROR::" << err;
    m_progress.pushError(err);
}

