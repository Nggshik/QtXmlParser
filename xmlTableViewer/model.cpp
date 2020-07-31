#include "model.h"

#include <QFileDialog>

Model::Model(QObject *parent) : QObject(parent)
{

}

int Model::parseXML(const QString& directoryPath)
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
                    m_table.append(std::move(xmlParameters));
                }
            }
            else
            {
                pushProgressError(info.fileName());
            }
            m_progress.progressStepForward();
        }

    }
    emit xmlParsed(m_table);
    return 0;
}

QVector<QPair<QString, QString>> Model::parseXMLfile(const QString &filePath)
{
    QRegExp keyValueRX("<(\\w+)>(.+)<\\/(\\w+)>"); //<(\w+)>(.+)<\/(\g1)>
    keyValueRX.setPatternSyntax(QRegExp::RegExp2);

    QVector<QPair<QString, QString>> vec;

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
                    vec.append(qMakePair(caps[1],caps[2]));
                }
                else
                {
                    pushProgressError(file.fileName() + "\n At line -" + line);
                    vec.clear();
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
                    vec.clear();
                    break;
                }
            }

        }
    }
    else
    {
        pushProgressError(file.fileName());
    }
    return  vec;
}

void Model::pushProgressError(const QString& err)
{
    qDebug() << "MODEL::FILE::ERROR::" << err;
    m_progress.pushError(err);
}
