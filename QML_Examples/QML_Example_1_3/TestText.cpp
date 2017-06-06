#include "TestText.h"

TestText::TestText()
{

}

void TestText::setTextContent(QString str)
{
    textContent = str;
    emit textContentChanged();
}

QString TestText::getTextContent()
{
    return textContent;
}
