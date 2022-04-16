#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QTimer>
#include <QQuaternion>

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
    void initObject();

    // in location in shader
    struct {
        GLuint posVertex;
        GLuint colVertex;
        GLuint mvpMatrixUniform;
    } stShaderLocation;

    struct VertexData {
        QVector3D position;
        QVector3D normal;
        QVector3D col;
    };

private:
    QOpenGLVertexArrayObject *m_vao;
    QOpenGLShaderProgram *m_shader;
    QOpenGLFunctions *m_f;
    // using both vertex buffer and index buffer
    QOpenGLBuffer *m_vbo, *m_ibo;

    QVector3D m_vRotationAxis = QVector3D(1.f, 1.f, 1.f);
    // project matrix
    QMatrix4x4 m_projection, m_matrix;
    qreal m_rRotateSpeed = 1.f;
    // use quaternion is better than vertex or matrix in rotation
    QQuaternion m_rotation;
    QTimer timer;

    int nIndexCount = 0, nVertexCount = 0;
};

#endif // GLWIDGET_H
