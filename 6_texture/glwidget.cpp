#include "glwidget.h"

// read value from pro file to get .pro file path
#define CONFIG2STR(R) #R
#define CONFIG2QSTR(R) CONFIG2STR(R)

GlWidget::GlWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    // rotate every second
    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, this, [&]{

    });
    m_qsProPath = CONFIG2QSTR(PRO_PATH);
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
    stShaderLocation.posTexture = m_shader->attributeLocation("posTexture");
    stShaderLocation.mvpMatrixUniform = m_shader->uniformLocation("mvp_matrix");
}

void GlWidget::initObject() {
    VertexData vertices[] = {
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.5f, 0.5f)},
        {QVector3D(1.0f, -1.0f,  1.0f), QVector2D(0.5f, 0.5f)},
        {QVector3D(1.0f, -1.0f,  -1.0f), QVector2D(0.5f, 0.5f)},
        {QVector3D(-1.0f, -1.0f,  -1.0f), QVector2D(0.5f, 0.5f)},

        {QVector3D(-1.0f, 1.0f,  1.0f), QVector2D(0.5f, 1.f)},
        {QVector3D(1.0f, 1.0f, 1.0f), QVector2D(0.f, 0.5f)},
        {QVector3D(1.0f, 1.0f, -1.0f), QVector2D(0.f, 0.5f)},
        {QVector3D(-1.0f, 1.0f, -1.0f), QVector2D(0.f, 0.5f)}
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

void GlWidget::initTexture() {
    m_vao->bind();
    m_vbo->bind();
    m_ibo->bind();
    m_shader->bind();

    m_texture = new QOpenGLTexture(QImage(m_qsProPath + "/image.jpg").mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);

    m_texture->bind();
    m_shader->setUniformValue("texture", 0);
    m_texture->release();

    m_shader->release();
    m_ibo->release();
    m_vbo->release();
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
    initTexture();
    initObject();

    m_vao->bind();
    m_vbo->bind();
    m_ibo->bind();
    m_shader->bind();

    // set buffer position
    m_shader->enableAttributeArray(stShaderLocation.posVertex);
    m_shader->enableAttributeArray(stShaderLocation.posTexture);

    m_shader->setAttributeBuffer(stShaderLocation.posVertex, GL_FLOAT, offsetof(VertexData, position), 3, sizeof(VertexData));
    m_shader->setAttributeBuffer(stShaderLocation.posTexture, GL_FLOAT, offsetof(VertexData, texcoord), 2, sizeof(VertexData));


    m_shader->release();
    m_ibo->release();
    m_vbo->release();
    m_vao->release();

    m_f->glEnable(GL_DEPTH_TEST);
    m_f->glEnable(GL_CULL_FACE);
}

void GlWidget::paintGL() {
    // set background color
    m_f->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_matrix.setToIdentity();
    m_matrix.translate(0.0f, 0.0f, -3.0f);
    m_matrix.rotate(180.0f - (m_vRotAngle[0] / 16.0f), 1, 0, 0);
    m_matrix.rotate(m_vRotAngle[1] / 16.0f, 0, 1, 0);
    m_matrix.rotate(m_vRotAngle[2] / 16.0f, 0, 0, 1);

    // draw
    m_vao->bind();
    m_vbo->bind();
    m_ibo->bind();
    m_shader->bind();

    m_texture->bind();
    m_shader->setUniformValue("texture", 0);

    m_shader->setUniformValue(stShaderLocation.mvpMatrixUniform, m_projection * m_matrix);
    m_f->glDrawElements(GL_TRIANGLE_STRIP, nIndexCount, GL_UNSIGNED_SHORT, 0);

    m_texture->release();
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

void GlWidget::mousePressEvent(QMouseEvent *e) {
    QOpenGLWidget::mousePressEvent(e);

    mousePressPosition = QVector2D(e->localPos());
}
void GlWidget::mouseMoveEvent(QMouseEvent *e) {
    QOpenGLWidget::mouseMoveEvent(e);

    int dx = e->x() - mousePressPosition.x();
    int dy = e->y() - mousePressPosition.y();
    if (e->buttons() & Qt::LeftButton) {
        setRotation(m_vRotAngle[0] + 8 * dy, 0);
        setRotation(m_vRotAngle[1] + 8 * dx, 1);
    } else if (e->buttons() & Qt::RightButton) {
        setRotation(m_vRotAngle[0] + 8 * dy, 0);
        setRotation(m_vRotAngle[2] + 8 * dx, 2);
    }
    mousePressPosition = QVector2D(e->localPos());
    update();
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GlWidget::setRotation(int angle, int axis)
{
    qNormalizeAngle(angle);
    if (angle != m_vRotAngle[axis]) {
        m_vRotAngle[axis] = angle;
        update();
    }
}
