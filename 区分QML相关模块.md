# 区分QML相关模块与概念
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
