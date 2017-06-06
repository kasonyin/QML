#ifndef TEXT_H
#define TEXT_H

#include <QObject>
#include <QVariant>

class Text : public QObject
{
    Q_OBJECT
public:
    explicit Text(QObject *parent = 0);

signals:
    void setText(QVariant text);

public slots:
    void slot_send_back_result(const QString& text);
};

#endif // TEXT_H
