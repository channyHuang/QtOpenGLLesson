#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(640, 480);
    mainLayout = new QHBoxLayout;

    glWidget = new GlWidget(this);
    mainLayout->addWidget(glWidget);

    setLayout(mainLayout);
}

Widget::~Widget()
{
}

