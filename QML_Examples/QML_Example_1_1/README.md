# QML与C++的连接调研示例程序1

该示例程序使用Qt Creator创建的一个Qt Quick Control Application初始化项目为基础编写演示代码，你可以创建一个该项目，然后与该示例项目代码进行对照，明确哪些内容是后面添加的。阅读该文档时请对照着代码阅读。

## main.cpp

在该源文件中添加的代码为

```cpp
qmlRegisterType<TestText>("MyText", 1, 0, "TestText");
```

首先，本质上来说，QML中的各种类型，其底层也是C++的实现。Qt提供了一种机制，可以通过C++来扩展QML，即通过将C++类进行注册，注册到QML中，使其可以在QML中进行调用。详见[官方文档](http://doc.qt.io/qt-5/qtqml-cppintegration-definetypes.html)。

这句代码的基本含义为，"MyText"就是在QML中可以跟在import语句后的字段，1和0都是版本号的一部分，而TestText则是需要注册到QML的C++类名。例如，在main.qml中，使用语句

```javascript
import MyText 1.0
```

就可以将这里通过qmlRegisterType注册的内容include到qml里。

## main.qml

将注册的MyText进行import的语句上面已经提到。接下来可以看到，我们可以在该文件中使用QML语句创建该类的实例

```qml
TestText {
    id: testText1
}
```

其中，TestText是C++中对应的类名，testText是该实例的id，该类可以创建多个实例，但每个实例的id必须不一样。当我们要调用实例的方法时，需要使用id

在该QML源文件里，还有这样一段代码：

```qml
MainForm {
    anchors.fill: parent
    button1.onClicked: messageDialog.show(qsTr("Button 1 pressed"))
    button2.onClicked: messageDialog.show(qsTr("Button 2 pressed"))
    textField {
        text: testText1.textContent
    }
}
```

这是在main.qml中对MainForm.ui.qml中的实例进行一些设置的窗口。由于.ui.qml文件是Qt Creator只建议通过“设计”模式进行编辑的，所以只适合通过拖拽ui组建的方式进行编辑。而需要进行一些设置，比如为拖拽的按钮添加onClicked事件处理时，就应该写在qml里的MainForm中。这里还需要注意，由于一个qml源文件中的实例需要使用property alias语句后，才能在其他qml源文件中进行访问，所以这里的MainForm中的button1, button2, textField等，都是在.ui.qml中有相应的property alias语句的，而且该语句可以在“设计”模式中，在“导航”窗口那里通过按钮添加。如果你用文档编辑模式打开.ui.qml文件，你就会看见自己通过点击按钮添加的property alias语句。

在这里的MainForm里，textField可以看作是通过拖拽添加的一个TextField的ui组件的id，这里我们对该ui组件的text属性进行设置。

设置的内容里，testText1就是上面我们自己注册的一个用C++实现的QML类的一个实例，通过.textContent就可以得到testText1的成员变量textContent的内容。这里相当于用testText1的textContent的内容作为ui组件textField的text属性的内容。

另外在main.qml中还有一些其他内容，我们在MenuBar里面新添加了一个Menu，其Title为Test，，该Menu下有一个MenuItem，显示Text为TestDialog，并设置其onTriggered事件内容为open一个dialog1，dialog1是我们在该QML源文件最下方创建的一个QML标准组件Dialog的id。所以表现为，点击窗口上方的菜单按钮"Test"，选择并点击TestDialog，就会弹出dialog1

在dialog1上点击ok按钮后，主窗口上的TextField的内容就会被设置，这里设置为字符串"result"，所以dialog1的onAccepted事件的内容为设置testText1的成员变量textContent的内容为"result"，实质上是调用了一个set方法，而上面的textField组件的text属性则是实质上是通过一个get方法取得的textContent的内容。你会发现这里使用点+成员变量名就取得了textContent的内容，但实际上确实调用了set和get方法，而不是直接修改或取得textContent的内容，set和get方法是由qml的Engine调用的，这里后面会提到。

## TestText.h

TestText，这个就是我们注册到QML里的那个C++类了。看其头文件，你会发现，这里有setTextContent，也有getTextContent，但是刚才在qml里，我们直接通过

```qml
text: testText1.textContent
```

就得到了textContent的内容（注意textContent为private），这是怎么回事？

这是因为这里使用了Q_PROPERTY宏：

```cpp
Q_PROPERTY(QString textContent READ getTextContent WRITE setTextContent NOTIFY textContentChanged)
```

其意思如下：

QString textContent

这里为需要让QML访问的变量名

READ getTextContent

这个是告诉QML的engine，如果需要得到textContent的内容，调用getTextContent这个C++函数就好。这也就是为啥上面直接写testText1.textContent就可以得到其内容，因为QML的engine已经自动调用该get函数了。

WRITE setTextContent

这个就是告诉QML的engine，当在QML语句里对textContent进行赋值时，engine需要使用setTextContent这一C++函数来修改textContent的内容。而对于QML编写人来说，直接写"="号就可以了。

NOTIFY textContentChanged

你会发现，TestText.h里有一个signal，名为textContentChanged，而在TestText.cpp中你可以发现，在set函数里，修改了textContent的内容之后，emit了该signal。这个NOTIFY参数可以告诉QML的engine，当收到该信号时，表示textContent的内容改变了。对于QML的engine来说，它会自动马上调用get函数，得到其新值，保证ui上面的内容和C++的类的实例对象中的内容一致。

另外还有一个需要注意的地方，如果要从QML中直接调用C++对象的方法，该方法在头文件中需要加上Q_INVOKABLE前缀，有该前缀的函数才能在QML里直接调用。比如这里的

```cpp
Q_INVOKABLE void setTextContent(QString str);
```

如果你需要在QML中自己调用该函数来修改textContent的内容，而不是直接写"="号，就可以这样写：

```qml
Dialog {
    id: dialog1
    onAccepted: testText1.setTextContent(qsTr("result"));
}
```
