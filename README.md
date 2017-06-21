# QML调研任务进度

**本文档是对调研任务的进度总结，在工作中会把当天发现进行总结。**

## 调研QML与C++的连接

本探究任务是为了弄清以下问题：

> 是否可以在不修改C++源文件与头文件的前提下，用QML代替原.ui文件，来实现图形界面，并与程序的C++逻辑代码进行交互

通过总结认为，C++与QML的连接主要包括两种类型，即把C++对象注册进QML的方式，或者将QML对象装载到C++里的方法。一般认为C++对QML层应该知道得越少越好。

## 在C++里装载QML的Objects

例如**MyItem.qml**像这样：

```qml
import QtQuick 2.0

Item {
    width: 100; height: 100
}
```

这段QML可以在C++中使用**QQmlComponent**或者**QQuickView**来进行装载。**QQmlComponent**和**QQuickView**都可以将QML的document装载为C++的对象，但是QQuickView继承自QWindow，被装载的对象会被进行可视化渲染。这部分内容的官方文档[在这里](http://doc.qt.io/qt-5/qtqml-cppintegration-interactqmlfromcpp.html#loading-qml-objects-from-c)，请等待页面加载完成并跳转到相应页面。

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

## 有关UI组件

通过调研发现，Qt官方推荐的做法是在.ui.qml中设计UI组件，也就是说应该用Qt Creator的“设计”模式打开.ui.qml文件，然后通过拖动方式设计UI组件。原因解释大致为，为了让Qt Creator的Designer易于管理这些代码，不应写太多复杂的东西在.ui.qml文件中，所以甚至该文件只支持QML语言的一个子集，这点在“*区分QML相关模块*”中有提到。关于.ui.qml文件与.qml文件的一些基本概念的比较以及一些注意事项也在“*区分QML相关模块*”文档中有提及，包括这两个文件虽然后缀名看上去不一样，但是后缀名前面的名字依然应该使用不同的名字等问题（比如Test.qml应该对应TestForm.ui.qml）。然而由于Qt官方似乎没有项目文件组织管理的范例，如何管理项目中的.qml文件，以及.qml文件如何与.ui.qml相协作，还没有查到有价值的信息，或许需要通过Qt的官方示例代码来自己总结。

这里给出一段示例代码简单说明在QML代码中访问.ui.qml中通过拖动方式设计的UI组件。这段实例代码实际上就是Qt Creator里面Qt Quick Application项目初始化的默认代码，只是加上了一个Dialog，以及鼠标点击MouseArea时弹出Dialog的机制，还有一个测试用的Text组件text1。

main.qml

```qml
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    MainForm {
        mouseArea.onDoubleClicked: {
            testdialog.open()
        }
        anchors.fill: parent
        mouseArea.onClicked: {
            console.log(qsTr('Clicked on background. Text: "' + textEdit.text + '"'))
        }
        text1.text: qsTr("oh yes")
    }

    Dialog {
        id: testdialog
        title: qsTr("This is the test dialog")
    }
}
```

这段代码中有一些需要注意的地方。首先，需要import QtQuick里的Dialog类库，而且必须要带上版本号，版本号Qt Creator的自动补全会给出提示。

下面便是QML代码。这里实际上包含了树形结构，Window包含了MainForm，和Dialog。Window的id我们设为root，这是一个好习惯，因为每一个qml文件都会有一个根元素，所以把根元素id设为root方便直接使用root来访问。

这里的MainForm对应的是MainForm.ui.qml文件名不要后缀的样子。当qml文件在同一个目录时，一个qml文件直接使用这种形式就可以引用另外一个qml文件中的元素。如果另外一个qml在另外的目录，那么需要使用import语句和相对路径来将那个文件include进来，正如javascript语法那样。**这里顺便提醒一句，qml和.ui.qml文件的名字应该始终以大写字母开头。**

这里的Dialog是一个标准件，它包含在我们上面import进来的QtQuick.Dialog里面。除此之外，还有ColorDialog，FontDialog等，详见[官方文档](http://doc.qt.io/qt-5/qtquickdialogs-index.html)。我们对Dialog的一些属性进行了设置，包括它的id和对话框的标题。

因为Dialog是写在Window的大括号内部的，所以Window就是Dialog的parent，在QML里，父子关系便是这样通过代码体现了。Dialog内部可以使用parent访问父元素。

接下来的问题要结合MainForm.ui.qml来讲

MainForm.ui.qml

```qml
import QtQuick 2.6

Rectangle {
    property alias textEdit: textEdit

    width: 360
    height: 360
    property alias text1: text1
    property alias mouseArea: mouseArea

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        Text {
            id: text1
            x: 168
            y: 153
            text: qsTr("Text")
            font.pixelSize: 12
        }
    }

    TextEdit {
        id: textEdit
        text: qsTr("Enter some text...")
        verticalAlignment: Text.AlignVCenter
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        Rectangle {
            anchors.fill: parent
            anchors.margins: -10
            color: "transparent"
            border.width: 1
        }
    }
}
```

这段代码中同样有一些值得注意的地方。

首先是property alias语句。你可以看到MouseArea（注意大小写）是Rectangle的一个孩子，所以MouseArea内部的属性、函数等肯定是不在根(root)级别的。property alias语句一方面可以用于为元素里的某些属性设置别名，例如

```qml
property alias text: textEdit.text
```

另一方面，通过该语句后，可以将子元素中的属性和函数等提高到root层级，比如以下两条语句就可以把MouseArea和TextEdit里的属性和函数提高到root层级。

```qml
property alias text1: text1
property alias mouseArea: mouseArea
```

上面提到过，在同一个目录下的qml文件可以直接访问另外文件里的元素，但是比如id为text1的元素里的属性text，在另外的qml文件里却不能使用，因为它不在root层级。通过property alias语句后，使用text1（冒号左边的text1）像这样：text1.text变可以在另外的qml文件中访问设置text1的text属性了。比如下面的语句可以在main.qml中对text1的text属性进行访问和设置：

```qml
text1.text: qsTr("oh yes")
```

注意.ui.qml文件是通过“设计”模式打开的，在Qt Creator的Designer里也有相应的按钮设置，在“导航”里有一个按钮，提示标签为"Toggle whether this item is exported as an alias property of the root item"，打开或者关闭它，Qt Creator遍会自动在.ui.qml文件里添加或删除相应的 property alias 语句。

如此以来，.ui.qml文件便代表了通过拖动方式设计的UI组件。我们现在在Designer中，对MouseArea添加Signal Handler函数。在“导航”窗口中，对相应的组件点击右键遍可以执行操作。我们添加一个双击(double click)的Signal Handler。Qt Creator会在main.qml中添加一段代码：

```qml
mouseArea.onDoubleClicked: {

}
```

我们在这里添加代码：

```qml
testdialog.open()
```

遍可以在mouseArea收到双击事件时，弹出Dialog。这里testdialog为main.qml里子元素Dialog的id，通过id可以直接指定到它。



