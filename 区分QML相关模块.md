# 区分QML与Qt Quick相关模块与概念(Distinguish QML and Qt Quick Concepts)
在调研过程中发现Qt文档中有很多概念容易混淆，这里列出官网的定义，以帮助进行区分。相关页面请[点击这里](http://doc.qt.io/qt-5/qmlapplications.html)，或者分别查看各模块或概念的定义与介绍页面。

## QML
QML is a declarative ***language*** that allows user interfaces to be described in terms of their visual components and how they interact and relate with one another.

## Qt QML
The Qt QML module provides a ***framework*** for developing applications and libraries with the [QML language](http://doc.qt.io/qt-5/qmlapplications.html). It defines and implements the language and engine infrastructure, and provides an API to enable application developers to extend the QML language with custom types and integrate QML code with JavaScript and C++. The Qt QML module provides both a [QML API](http://doc.qt.io/qt-5/qtqml-qmlmodule.html) and a [C++ API](http://doc.qt.io/qt-5/qtqml-module.html). Note that while the Qt QML module provides the language and infrastructure for QML applications, the [Qt Quick](http://doc.qt.io/qt-5/qtquick-index.html) module provides many visual components, model-view support, an animation framework, and much more for building user interfaces.

## Qt Quick
The Qt Quick module is the standard ***library*** for writing QML applications. Qt Quick is the standard library of types and functionality for QML.

For creating or customizing graphical user interfaces, Qt Quick adds visual types, animation types, and other QML types in addition to the standard QML types from Qt QML.

Qt Quick only provides basic visual types and much of Qt's functionality is exposed to QML through other modules. If you require the functionality of those modules, you should browse their QML documentation. The [All QML APIs by Module](http://doc.qt.io/qt-5/modules-qml.html) contains a list of all QML modules in Qt 5.

## Qt Quick Controls
The Qt Quick Controls provide ***a set of UI controls*** to create user interfaces in Qt Quick. For a set of basic UI controls, the [Qt Quick Controls](http://doc.qt.io/qt-5/qtquickcontrols-index.html) module implements several controls such as buttons, menus, and views. These controls mimic the native behavior found in different platforms such as Windows, macOS, and Linux.

## Qt Quick Controls 2
The differences between **Qt Quick Controls** and **Qt Quick Controls 2** can be seen [here](https://doc.qt.io/qt-5/qtquickcontrols2-differences.html) 

## .qml
qml source file

在该文件中可以定义GUI，可以用QML编写一些功能代码等。.qml是QML语言代码文件的专用后缀名

## .ui.qml
Qt Quick UI forms

关于.ui.qml文件的官方描述[在这里](http://doc.qt.io/qtcreator/creator-quick-ui-forms.html)

除了官方文档，还有一些StackOverflow上面的优秀解答同样具有参考价值:[Working with .ui.qml files](http://stackoverflow.com/questions/27456804/working-with-ui-qml-files)还有[What is the use of the ui.qml files in Qt5 QML (QML)?](http://stackoverflow.com/questions/30652537/what-is-the-use-of-the-ui-qml-files-in-qt5-qml)

总结而言，.ui.qml可以这样说，它内部的代码内容是QML语言的一个子集，有一些QML的功能和类型它是没有的，具体有哪些功能和类型没有，在上面的官网相关链接上给出了描述。

官网推荐在Qt Creator中通过Design模式来编辑该类文件，类似于在Qt Widget方式中通过Design模式来编辑.ui文件一样，同样是通过拖拽的方式来进行UI设计。通过尝试操作，认为可以把.ui.qml当做专门的用来描述GUI样式的文件。

经过调研后认为，与UI元素进行交互，应该使用property alias对.ui.qml里的Object进行export，让其可以在.qml文件中通过QML代码进行操作。然后.qml文件中的QML代码再与C++代码进行交互。详细内容会在QML调研文档中描写，这个文档只是用来对容易混淆的模块与概念进行对比

## An informal C++/QML taxonomy:
* QtObject - a QML side name for the plain old QObject, not necessarily visual

* Item - a QQuickItem on the C++ side, which is a QObject (and thus a QtObject), root of all visual items

* component - a QQuickComponent on the C++ side, an inline Component item or an imported item in QML source

* document - a string (most often coming from a .qml file) defining a component
