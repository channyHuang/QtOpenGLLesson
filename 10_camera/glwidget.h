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
#include <QKeyEvent>
#include <QOpenGLTexture>
#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLVersionFunctionsFactory>

#include "objfileloader.h"
#include "camera.h"

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

    virtual void keyPressEvent(QKeyEvent *e) override;
    virtual void keyReleaseEvent(QKeyEvent *e) override;
private:
    void initShader();
    // use mouse to rotate
    void setRotation(int angle, int axis);
    void initTexture();

    // in location in shader
    struct {
        GLuint mvp;
        GLuint offset;
        GLuint tex;
    } stShaderLocation;

private:
    QOpenGLVertexArrayObject *m_vao;
    QOpenGLShaderProgram *m_shader;
    QOpenGLFunctions *mfunOpengl = nullptr;
    QOpenGLFunctions_4_2_Core *mfunOpenglVer = nullptr;

    QVector3D m_vRotAngle = QVector3D(0, 0, 0);
    // project matrix
    QMatrix4x4 m_projection, m_matCamera;
    QVector3D m_vViewPos, m_vTargetPos, m_vUpDir;
    QTimer timer;

    int nIndexCount = 0, nVertexCount = 0;

    // mouse position in screen
    QVector2D mousePressPosition = QVector2D(0, 0);
    QString m_qsProPath;
    QOpenGLTexture *m_texture[4];

    // model file loader
    ObjFileLoader m_fileLoader;
    FileObject m_dataObject;
    Camera camera;
};

#endif // GLWIDGET_H
