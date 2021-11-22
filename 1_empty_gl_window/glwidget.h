#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>

class GlWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GlWidget(QWidget *parent = nullptr);
    ~GlWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
};

#endif // GLWIDGET_H
