#include "glwidget.h"

GlWidget::GlWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

GlWidget::~GlWidget() {}

void GlWidget::initializeGL() {}
void GlWidget::paintGL() {}
void GlWidget::resizeGL(int w, int h) {};
