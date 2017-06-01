#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include "Text.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    Text text;

    QObject *topLevel = engine.rootObjects().value(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    QObject::connect(window, SIGNAL(sig_send_back_result(QString)), &text, SLOT(slot_send_back_result(QString)));

    QObject::connect(&text, SIGNAL(setText(QVariant)), window, SLOT(setText(QVariant)));

    return app.exec();
}
