#include "TestDialog.h"
#include <QDebug>

TestDialog::TestDialog()
{

}

void TestDialog::setText()
{
    qDebug() << "Set Text Function Called!";
    emit changeText();
}
