#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

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

private:
    void initShader();

    // in location in shader
    struct {
        GLuint posVertex;
        GLuint colVertex;
        GLuint matrixUniform;
    } stShaderLocation;

private:
    QOpenGLVertexArrayObject *m_vao;
    QOpenGLShaderProgram *m_shader;
    QOpenGLFunctions *m_f;
    QOpenGLBuffer *m_vbo;

    QVector3D m_vRotationAngle = QVector3D(0, 0, 0);
    QMatrix4x4 m_matrix;
};

#endif // GLWIDGET_H
