# 该示例与QML_Example_1_1的区别

在QML_Example_1_1中的main.qml中，testText1的textContent的值是直接绑定到textField的text属性上的：

```qml
textField {
    text: testText1.textContent
}
```

而在本例中，通过在main.qml中使用connect将textContent被改变时发出的信号连接到修改textField的text属性的一个单独的函数上：

```qml
Component.onCompleted: {
    testText1.textContentChanged.connect(setTextField);
}
```
