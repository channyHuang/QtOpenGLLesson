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
#include <QMouseEvent>
#include <QOpenGLTexture>

class GlWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GlWidget(QWidget *parent = nullptr);
    ~GlWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
private:
    void initShader();
    void initObject();
    // use mouse to rotate
    void setRotation(int angle, int axis);
    void initTexture();

    // in location in shader
    struct {
        GLuint posVertex;
        GLuint norVertex;
        GLuint posTexture;
        GLuint mvMatrixUniform;
        GLuint projMatrixUniform;
        GLuint lightPos;
        GLuint normalMatrixUniform;
    } stShaderLocation;

    struct VertexData {
        QVector3D position;
        QVector3D normal;
        QVector2D texcoord;
    };

private:
    QOpenGLVertexArrayObject *m_vao;
    QOpenGLShaderProgram *m_shader;
    QOpenGLFunctions *m_f;
    // using both vertex buffer and index buffer
    QOpenGLBuffer *m_vbo, *m_ibo;

    QVector3D m_vRotAngle;
    // project matrix
    QMatrix4x4 m_projection, m_matrix;
    qreal m_rRotateSpeed = 1.f;
    // use quaternion is better than vertex or matrix in rotation
    QQuaternion m_rotation;
    QTimer timer;

    int nIndexCount = 0, nVertexCount = 0;

    // mouse position in screen
    QVector2D mousePressPosition;
    QString m_qsProPath;
    QOpenGLTexture *m_texture;
};

#endif // GLWIDGET_H
