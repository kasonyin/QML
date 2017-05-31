#ifndef TESTDIALOG_H
#define TESTDIALOG_H
#include <QObject>

class TestDialog : public QObject
{
    Q_OBJECT
public:
    TestDialog();
    Q_INVOKABLE void setText();

signals:
    void changeText();
};

#endif // TESTDIALOG_H
