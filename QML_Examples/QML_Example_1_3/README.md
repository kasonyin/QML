# 将C++注册到QML中成为QML对象的一个属性，而不是可成为一个单独的QML类

从该例的main.cpp的写法和QML_Example_1_1还有QML_Example_1_2都不同这一点，应该注意本例子虽然也是将C++类注册进QML，但是只是注册进QML对象中成为QML对象的一个属性。

将main.qml和前两个例子的main.qml相比较，你会发现对testText1的创建代码被注释掉了，但我们已经在mian.cpp里将testText类注册为main.qml中的一个叫testText的属性，通过使用testText属性，我们同样能实现和前两个例子类似的工作

```qml
Component.onCompleted: {
    testText.textContentChanged.connect(setTextField);
}
```

**注意：导出的属性可以直接使用，与属性关联的C++对象，它的信号、槽、可调用方法（使用 Q_INVOKABLE 宏修饰的方法）、属性都可以在QML中被使用，只是如果C++类中有枚举值类型的对象，则不能通过类名来引用枚举值。前面两个例子中将C++类注册进QML并成为独立的QML类的方法中，则可以使用类名来引用枚举值**
