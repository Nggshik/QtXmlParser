#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

class Modeltest : public QObject
{
    Q_OBJECT

public:
    Modeltest();
    ~Modeltest();

private slots:
    void test_case1();

};

Modeltest::Modeltest()
{

}

Modeltest::~Modeltest()
{

}

void Modeltest::test_case1()
{

}

QTEST_MAIN(Modeltest)

#include "tst_modeltest.moc"
