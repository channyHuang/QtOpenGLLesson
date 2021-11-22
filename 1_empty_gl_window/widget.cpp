#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(640, 480);
    mainLayout = new QHBoxLayout;

    setLayout(mainLayout);
}

Widget::~Widget()
{
}

