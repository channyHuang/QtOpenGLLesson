#include "glwidget.h"

GlWidget::GlWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    // rotate every second
    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, this, [&]{
        m_rRotateSpeed *= 0.99;
        if (m_rRotateSpeed < 0.01) {
            m_rRotateSpeed = 1.0;
        } else {
            m_rotation = QQuaternion::fromAxisAndAngle(m_vRotationAxis, m_rRotateSpeed);
        }
        update();
    });
}

GlWidget::~GlWidget() {}

void GlWidget::initShader() {
    m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl");
    m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl");
    if (!m_shader->link()) {
        qDebug() << "shader link failed";
    }

    // get location number of attribute members in shader
    stShaderLocation.posVertex = m_shader->attributeLocation("posVertex");
    stShaderLocation.colVertex = m_shader->attributeLocation("colVertex");
    stShaderLocation.mvpMatrixUniform = m_shader->uniformLocation("mvp_matrix");
}

void GlWidget::initObject() {
    VertexData vertices[] = {
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector3D(1.f, 0.f, 1.f)},
        {QVector3D(1.0f, -1.0f,  1.0f), QVector3D(1.f, 0.f, 1.f)},
        {QVector3D(1.0f, -1.0f,  -1.0f), QVector3D(1.f, 0.f, 0.f)},
        {QVector3D(-1.0f, -1.0f,  -1.0f), QVector3D(1.f, 0.f, 0.f)},

        {QVector3D(-1.0f, 1.0f,  1.0f), QVector3D(0.f, 1.f, 0.f)},
        {QVector3D(1.0f, 1.0f, 1.0f), QVector3D(0.f, 0.f, 1.f)},
        {QVector3D(1.0f, 1.0f, -1.0f), QVector3D(0.f, 0.f, 1.f)},
        {QVector3D(-1.0f, 1.0f, -1.0f), QVector3D(0.f, 1.f, 0.f)}
    };
    GLushort indices[] = {
            0, 1, 3, 2, 2,     // Face bottom
            2, 2, 6, 3, 7, 7,  // Face front
            3, 3, 7, 0, 4, 4,  // Face left
            4, 4, 5, 7, 6, 6,     // Face up
            0, 0, 1, 4, 5, 5, // Face back
            1, 1, 2, 5, 6, 6 // Face right
        };
    nIndexCount = 34;
    nVertexCount = 8;

    m_vao->bind();

    m_vbo->bind();
    m_vbo->allocate(vertices, sizeof(VertexData) * nVertexCount);
    m_vbo->release();

    m_ibo->bind();
    m_ibo->allocate(indices, sizeof(GLushort) * nIndexCount);
    m_ibo->release();

    m_vao->release();
}

void GlWidget::initializeGL() {
    m_vao = new QOpenGLVertexArrayObject;
    m_shader = new QOpenGLShaderProgram;
    m_f = this->context()->functions();
    m_vbo = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
    m_ibo = new QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer);

    m_vao->create();
    m_shader->create();
    m_vbo->create();
    m_ibo->create();

    // load shader
    initShader();

    initObject();

    m_vao->bind();
    m_vbo->bind();
    m_ibo->bind();
    m_shader->bind();

    // set buffer position
    m_shader->enableAttributeArray(stShaderLocation.posVertex);
    m_shader->enableAttributeArray(stShaderLocation.colVertex);

    m_shader->setAttributeBuffer(stShaderLocation.posVertex, GL_FLOAT, offsetof(VertexData, position), 3, sizeof(VertexData));
    m_shader->setAttributeBuffer(stShaderLocation.colVertex, GL_FLOAT, offsetof(VertexData, col), 3, sizeof(VertexData));


    m_shader->release();
    m_ibo->release();
    m_vbo->release();
    m_vao->release();

    m_f->glEnable(GL_DEPTH_TEST);
    m_f->glEnable(GL_CULL_FACE);
    timer.start();

    m_matrix.setToIdentity();
    m_matrix.translate(0.0f, 0.0f, -3.0f);
}

void GlWidget::paintGL() {
    // set background color
    m_f->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_matrix.rotate(m_rotation);

    // draw
    m_vao->bind();
    m_vbo->bind();
    m_ibo->bind();
    m_shader->bind();
    m_shader->setUniformValue(stShaderLocation.mvpMatrixUniform, m_projection * m_matrix);
    m_f->glDrawElements(GL_TRIANGLE_STRIP, nIndexCount, GL_UNSIGNED_SHORT, 0);

    m_shader->release();
    m_ibo->release();
    m_vbo->release();
    m_vao->release();
}

void GlWidget::resizeGL(int w, int h) {
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 2.f, zFar = 15.0f, fov = 60.0f;
    m_projection.setToIdentity();
    m_projection.perspective(fov, aspect, zNear, zFar);
};
