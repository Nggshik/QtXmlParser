#ifndef DIALOGMAPPER_H
#define DIALOGMAPPER_H

#include <QObject>
#include <QtWidgets>

class DialogMapper : public QDialog
{
    Q_OBJECT
public:
    explicit DialogMapper(QWidget *parent = nullptr);

public:
    void setModel(QAbstractItemModel* model);
public slots:
    void editRecord(int row);
signals:
private:
    void updateBtn(int row);
    void setLineEditorsData(Qt::Orientation orintation, int first, int last);
    void okButtonClicked();
private:
    QDataWidgetMapper* m_pMapper;

};

#endif // DIALOGMAPPER_H
