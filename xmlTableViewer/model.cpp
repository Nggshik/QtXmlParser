#include "model.h"
#include <QFileDialog>

Model::Model(QObject *parent) : QObject(parent)
{
    parseXML(QFileDialog::getExistingDirectory());
}

int Model::parseXML(const QString& directoryPath)
{
    QDir dir(directoryPath);
    QRegExp fileCheckRX("*.xml");
    fileCheckRX.setPatternSyntax(QRegExp::Wildcard);

    QFileInfoList filesInfo = dir.entryInfoList();
    for(auto info : filesInfo)
    {
        if(info.isFile())
        {
            if(fileCheckRX.exactMatch(info.fileName()))
            {
                auto xmlParameters = parseXMLfile(info.filePath());
                if(xmlParameters.size())
                    m_table.append(std::move(xmlParameters));
            }
            else
            {
                qDebug() << "MODEL::FILE::ERROR::" << info.fileName();
                m_errorFiles.append(info.fileName());
            }
        }

    }
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
                    qDebug() << "MODEL::FILEREAD::ERROR::" << file.fileName() << "/n At line -" << line;
                    m_errorFiles.append(file.fileName());
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
                    qDebug() << "MODEL::FILEREAD::ERROR::" << file.fileName() << "/n At line -" << line;
                    m_errorFiles.append(file.fileName());
                    vec.clear();
                    break;
                }
            }

        }
    }
    else
    {
        qDebug() << "MODEL::FILE::ERROR::" << file.fileName();
        m_errorFiles.append(file.fileName());
    }
    return  vec;
}
