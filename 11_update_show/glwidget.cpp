#include "glwidget.h"

// read value from pro file to get .pro file path
#define CONFIG2STR(R) #R
#define CONFIG2QSTR(R) CONFIG2STR(R)

//#define USING_FIX_DATA

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
    stShaderLocation.colVertex = m_shader->attributeLocation("colVertex");
    stShaderLocation.norVertex = m_shader->attributeLocation("norVertex");

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

void GlWidget::initObjectUsingSpecificData() {
    Vertex vertices[] = {
        {QVector3D(-0.045430, -0.045430, -0.750000), QVector3D(-0.061820, -0.061820, -2.597272), Common::getColor(), QVector2D(0.5, 0.5)},
        {QVector3D(-0.045430, -0.750000, -0.045430), QVector3D(-0.061820, -2.597272, -0.061820), Common::getColor(), QVector2D(0.5, 0.5)},
        {QVector3D(-0.750000, -0.045430, -0.045430), QVector3D(-2.597272, -0.061820, -0.061820), Common::getColor(), QVector2D(0.5, 0.5)},
        {QVector3D(-0.045430, -0.045430, 0.749414), QVector3D(-0.052176, -0.052176, 2.243320), Common::getColor(), QVector2D(0.5, 0.5)},
        {QVector3D(-0.045430, 0.749414, -0.045430), QVector3D(-0.052176, 2.243320, -0.052176), Common::getColor(), QVector2D(0.5, 0.5)},
        {QVector3D(0.749414, -0.045430, -0.045430), QVector3D(2.243320, -0.052176, -0.052176), Common::getColor(), QVector2D(0.5, 0.5)},
    };
    nVertexCount = 6;
#ifdef TRIANGLE_STRIP
    GLushort indices[] = {
            0, 1, 2, 2,     // Face bottom
            3, 3, 2, 1, 1,  // Face front
            4, 4, 0, 2, 2,  // Face left
            4, 4, 2, 3, 3,     // Face up
            0, 0, 5, 1, 1, // Face back
            5, 5, 3, 1, 1, // Face right
            4, 4, 5, 0, 0,
            5, 5, 4, 3, 3
    };
    nIndexCount = 39;
#else
    GLushort indices[] = {
        0, 1, 2,
        3, 2, 1,
        4, 0, 2,
        4, 2, 3,
        0, 5, 1,
        5, 3, 1,
        4, 5, 0,
        5, 4, 3
    };
    nIndexCount = 24;
#endif


    m_vao->bind();

    m_vbo->bind();
    m_vbo->allocate(vertices, sizeof(Vertex) * nVertexCount);
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

    m_texture = new QOpenGLTexture(QImage(m_qsProPath + "/../resources/food.jpg").mirrored());
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
#ifdef USING_FIX_DATA
    initObjectUsingSpecificData();
#else
    initObject();
#endif
    m_vao->bind();
    m_vbo->bind();
    m_ibo->bind();
    m_shader->bind();

    m_shader->setUniformValue("texture", 0);
    // set buffer position
    m_shader->enableAttributeArray(stShaderLocation.posVertex);
    m_shader->enableAttributeArray(stShaderLocation.posTexture);
    m_shader->enableAttributeArray(stShaderLocation.norVertex);
    m_shader->enableAttributeArray(stShaderLocation.colVertex);
#ifndef USING_FIX_DATA
    m_shader->setAttributeBuffer(stShaderLocation.posVertex, GL_FLOAT, offsetof(Vertex, vposition_), 3, sizeof(Vertex));
    m_shader->setAttributeBuffer(stShaderLocation.posTexture, GL_FLOAT, offsetof(Vertex, vtexcoord_), 2, sizeof(Vertex));
    m_shader->setAttributeBuffer(stShaderLocation.norVertex, GL_FLOAT, offsetof(Vertex, vnormal_), 3, sizeof(Vertex));
    m_shader->setAttributeBuffer(stShaderLocation.colVertex, GL_FLOAT, offsetof(Vertex, vcol_), 3, sizeof(Vertex));
#else
    m_shader->setAttributeBuffer(stShaderLocation.posVertex, GL_FLOAT, offsetof(Vertex, vposition_), 3, sizeof(Vertex));
    m_shader->setAttributeBuffer(stShaderLocation.colVertex, GL_FLOAT, offsetof(Vertex, vcol_), 3, sizeof(Vertex));
    m_shader->setAttributeBuffer(stShaderLocation.norVertex, GL_FLOAT, offsetof(Vertex, vnormal_), 3, sizeof(Vertex));
    m_shader->setAttributeBuffer(stShaderLocation.posTexture, GL_FLOAT, offsetof(Vertex, vtexcoord_), 2, sizeof(Vertex));
#endif
    m_shader->release();
    m_ibo->release();
    m_vbo->release();
    m_vao->release();

    m_f->glEnable(GL_DEPTH_TEST);
    //m_f->glEnable(GL_CULL_FACE);
    m_f->glFrontFace(GL_CCW);
}

void GlWidget::paintGL() {
    // set background color
    m_f->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_matrix.setToIdentity();
    //m_matrix.translate(0.0f, 0.0f, -20.0f);
    //m_matrix.rotate(180.0f - (m_vRotAngle[0] / 16.0f), 1, 0, 0);
    //m_matrix.rotate(m_vRotAngle[1] / 16.0f, 0, 1, 0);
    //m_matrix.rotate(m_vRotAngle[2] / 16.0f, 0, 0, 1);

    // draw
    m_vao->bind();
    m_vbo->bind();
    m_ibo->bind();
    m_shader->bind();

    m_texture->bind();
    m_shader->setUniformValue("texture", 0);

    m_shader->setUniformValue(stShaderLocation.mvMatrixUniform, m_matrix);
    m_shader->setUniformValue(stShaderLocation.projMatrixUniform, m_projection);// * camera.getViewMatrix());
    m_shader->setUniformValue(stShaderLocation.lightPos, QVector3D(0, 0, 7));

    QMatrix3x3 normalMatrix = m_matrix.normalMatrix();
    m_shader->setUniformValue(stShaderLocation.normalMatrixUniform, normalMatrix);

    m_f->glDrawElements(GL_TRIANGLES, nIndexCount * 3, GL_UNSIGNED_SHORT, 0);

    m_texture->release();
    m_shader->release();
    m_ibo->release();
    m_vbo->release();
    m_vao->release();
}

void GlWidget::resizeGL(int w, int h) {
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 1.f, zFar = 15.0f, fov = 60.0f;
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

void GlWidget::keyPressEvent(QKeyEvent *e) {
    QOpenGLWidget::keyPressEvent(e);
    camera.moveCamera(e);
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
