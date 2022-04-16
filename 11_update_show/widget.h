#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>

#include "glwidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QHBoxLayout *mainLayout;
    GlWidget *glWidget;
};
#endif // WIDGET_H
