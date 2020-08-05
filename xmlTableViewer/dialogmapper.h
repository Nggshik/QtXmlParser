#ifndef DIALOGMAPPER_H
#define DIALOGMAPPER_H

#include <QObject>
#include <QtWidgets>

class DialogMapper : public QWidget
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
    QPushButton* m_pOkBtn;
    QPushButton* m_pCancelBtn;

};

#endif // DIALOGMAPPER_H
