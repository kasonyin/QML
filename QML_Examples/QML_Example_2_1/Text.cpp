#include "Text.h"

Text::Text(QObject *parent) : QObject(parent)
{

}

void Text::slot_send_back_result(const QString &text)
{
    QVariant variant = QVariant(text);
    emit setText(variant);
}
