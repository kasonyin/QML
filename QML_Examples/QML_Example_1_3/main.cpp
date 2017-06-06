#include <QApplication>
#include <QQmlApplicationEngine>
#include "TestText.h"
#include <QtQuick>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView view;
    TestText testText;
    view.engine()->rootContext()->setContextProperty("testText", &testText);
    view.setSource(QUrl("qrc:/main.qml"));
    //view.show();

    return app.exec();
}
