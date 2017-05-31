#ifndef TESTTEXT_H
#define TESTTEXT_H
#include <QObject>

class TestText : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString textContent READ getTextContent WRITE setTextContent NOTIFY textContentChanged)
public:
    TestText();
    Q_INVOKABLE void setTextContent(QString str);
    QString getTextContent();

signals:
    void textContentChanged();

private:
    QString textContent;
};

#endif // TESTTEXT_H
