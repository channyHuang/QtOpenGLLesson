#include "glwidget.h"

// read value from pro file to get .pro file path
#define CONFIG2STR(R) #R
#define CONFIG2QSTR(R) CONFIG2STR(R)

GlWidget::GlWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    m_qsProPath = CONFIG2QSTR(PRO_PATH);
    m_projection.setToIdentity();
    m_matrix.setToIdentity();
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
    stShaderLocation.mvMatrixUniform = m_shader->uniformLocation("mv_matrix");
    stShaderLocation.projMatrixUniform = m_shader->uniformLocation("proj_matrix");
    stShaderLocation.lightPos = m_shader->uniformLocation("lightPos");
    stShaderLocation.normalMatrixUniform = m_shader->uniformLocation("normalMatrixUniform");
}

void GlWidget::initObject() {
    if (!m_fileLoader.loadObjFile((m_qsProPath + "/../resources/box.obj").toStdString())) {
        qDebug() << "load obj failed";
    }
    m_dataObject = m_fileLoader.getDataStruct();
    nIndexCount = m_fileLoader.getFaceCount();
    nVertexCount = m_fileLoader.getVertexCount();

    m_vao->bind();

    m_vbo->bind();
    m_vbo->allocate(m_dataObject.vertex.data(), sizeof(Vertex) * nVertexCount);
    m_vbo->release();

    m_ibo->bind();
    m_ibo->allocate(m_dataObject.faces.data(), sizeof(Vector3i) * nIndexCount);
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

    m_shader->setUniformValue("texture", 0);
    // set buffer position
    m_shader->enableAttributeArray(stShaderLocation.posVertex);
    m_shader->enableAttributeArray(stShaderLocation.posTexture);
    m_shader->enableAttributeArray(stShaderLocation.norVertex);

    m_shader->setAttributeBuffer(stShaderLocation.posVertex, GL_FLOAT, offsetof(Vertex, position), 3, sizeof(VertexData));
    m_shader->setAttributeBuffer(stShaderLocation.posTexture, GL_FLOAT, offsetof(Vertex, texcoord), 2, sizeof(VertexData));
    m_shader->setAttributeBuffer(stShaderLocation.norVertex, GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(VertexData));

    m_shader->release();
    m_ibo->release();
    m_vbo->release();
    m_vao->release();

    m_f->glEnable(GL_DEPTH_TEST);
    m_f->glEnable(GL_CULL_FACE);
    m_f->glFrontFace(GL_CCW);
}

void GlWidget::paintGL() {
    // set background color
    m_f->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_matrix.setToIdentity();
    m_matrix.translate(0.0f, 0.0f, -5.0f);
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

    m_shader->setUniformValue(stShaderLocation.mvMatrixUniform, m_matrix);
    m_shader->setUniformValue(stShaderLocation.projMatrixUniform, m_projection);
    m_shader->setUniformValue(stShaderLocation.lightPos, QVector3D(0, 0, 7));

    QMatrix3x3 normalMatrix = m_matrix.normalMatrix();
    m_shader->setUniformValue(stShaderLocation.normalMatrixUniform, normalMatrix);

    m_f->glDrawElements(GL_TRIANGLES, nIndexCount * 3, GL_UNSIGNED_INT, 0);

    m_texture->release();
    m_shader->release();
    m_ibo->release();
    m_vbo->release();
    m_vao->release();
}

void GlWidget::resizeGL(int w, int h) {
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 3.f, zFar = 10.0f, fov = 45.0f;
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
