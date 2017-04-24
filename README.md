# QML调研任务进度

**本文档是对调研任务的进度总结，在工作中会把当天发现进行总结。**

## 调研QML与C++的连接

本探究任务是为了弄清以下问题：

> 是否可以在不修改C++源文件与头文件的前提下，用QML代替原.ui文件，来实现图形界面，并与程序的C++逻辑代码进行交互

## 在C++里装载QML的Objects

例如**MyItem.qml**像这样：

```qml
import QtQuick 2.0

Item {
    width: 100; height: 100
}
```
这段QML可以在C++中使用**QQmlComponent**或者**QQuickView**来进行装载。

```cpp
// Using QQmlComponent
QQmlEngine engine;
QQmlComponent component(&engine,
        QUrl::fromLocalFile("MyItem.qml"));
QObject *object = component.create();
...
delete object;
```

```cpp
// Using QQuickView
QQuickView view;
view.setSource(QUrl::fromLocalFile("MyItem.qml"));
view.show();
QObject *object = view.rootObject();
```

然后可以使用**QObject::setProperty**()或者是**QQmlProperty**来修改对象的属性：

```cpp
object->setProperty("width", 500);
QQmlProperty(object, "width").write(500);
```

或者可以将对象转换成实际的类型，然后调用其方法：

```cpp
QQuickItem *item = qobject_cast<QQuickItem*>(object);
item->setWidth(500);
```

## C++中调用QML对象方法

可以在C++中使用**QMetaObject::invokeMethod**()通过元对象系统调用QML中的对象方法。方法的参数和返回值需要是**QVariant**类型。详见[官方文档](http://doc.qt.io/qt-5/qtqml-cppintegration-interactqmlfromcpp.html#invoking-qml-methods)。

## 关于Signals and Slots

在QML中可以通过**signal**关键字创建signal

```qml
signal submitTextField(string text)
```

可以在QML代码段中通过事件触发signal

```qml
onClicked:
    // emit the submitTextField signal
submitTextField(textField1.text)
```

在C++中，像以前一样写slots

```cpp
class HandleTextField : public QObject
{
    Q_OBJECT
public:
    explicit HandleTextField(QObject *parent = 0);

public slots:
    void handleSubmitTextField(const QString& in);

};
```

slots的实现

```cpp
void HandleTextField::handleSubmitTextField(const QString &in)
{
    qDebug() << "c++: HandleTextField::handleSubmitTextField:" << in;
}
```

要进行connect，需要先使用上面提到的方法对QML对象进行装载，然后使用**QObject::connect**()

```cpp
HandleTextField handleTextField;

QObject *topLevel = engine.rootObjects().value(0);
QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

// connect our QML signal to our C++ slot
QObject::connect(window, SIGNAL(submitTextField(QString)),
                     &handleTextField, SLOT(handleSubmitTextField(QString)));
```

**以上是从QML发送Signals，然后C++用Slots接收和处理，接下来是反过来**

这里如上面所说，如果要用一般的对象作为参数，需要使用**QVariant**，在C++里这样写

```cpp
signals:
  void setTextField(QVariant text);
```

然后使用**emit**发送signals

```cpp
void HandleTextField::handleSubmitTextField(const QString &in)
{
    qDebug() << "c++: HandleTextField::handleSubmitTextField:" << in;
    emit setTextField(in.toUpper());
}
```

然后在QML里编写slot，实际上就是一个function

```qml
// this function is our QML slot
function setTextField(text){
    console.log("setTextField: " + text)
    textField1.text = text
}
```

装载QML的对象**window**后，在C++里使用connect就可以了

```cpp
QObject::connect(&handleTextField, SIGNAL(setTextField(QVariant)),
                 window, SLOT(setTextField(QVariant)));
```
