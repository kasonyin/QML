#include <QApplication>
#include <QQmlApplicationEngine>
#include "TestText.h"
#include <QtQuick>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<TestText>("MyText", 1, 0, "TestText");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
