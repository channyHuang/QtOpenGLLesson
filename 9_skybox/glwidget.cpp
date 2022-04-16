#include "glwidget.h"

// read value from pro file to get .pro file path
#define CONFIG2STR(R) #R
#define CONFIG2QSTR(R) CONFIG2STR(R)

GlWidget::GlWidget(QWidget *parent) : QOpenGLWidget(parent)
{
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
    stShaderLocation.offset = m_shader->uniformLocation("offset");
    stShaderLocation.mvp = m_shader->uniformLocation("mvp");
    stShaderLocation.tex = m_shader->uniformLocation("tex");
}

void GlWidget::initTexture() {
    m_vao->bind();
    m_shader->bind();

    for (int i = 0; i < 2; ++i ) {
        m_texture[i] = new QOpenGLTexture(QImage(m_qsProPath + (i == 0 ? "/../resources/sky.jpg" : "/../resources/yellow.jpg")).mirrored());
        m_texture[i]->setMinificationFilter(QOpenGLTexture::Nearest);
        m_texture[i]->setMagnificationFilter(QOpenGLTexture::Linear);
        m_texture[i]->setWrapMode(QOpenGLTexture::Repeat);

        m_texture[i]->bind();
        m_shader->setUniformValue("tex", 0);
        m_texture[i]->release();
    }

    m_shader->release();
    m_vao->release();
}

void GlWidget::initializeGL() {
    m_vao = new QOpenGLVertexArrayObject;
    m_shader = new QOpenGLShaderProgram;
    //mfunOpenglVer = this->context()->functions();
    mfunOpenglVer = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_2_Core>(this->context());
    mfunOpenglVer->initializeOpenGLFunctions();

    m_vao->create();
    m_shader->create();

    // load shader
    initShader();
    initTexture();

    m_vao->bind();
    m_shader->bind();

    m_shader->setUniformValue("tex", 0);

    m_shader->release();
    m_vao->release();

    mfunOpenglVer->glEnable(GL_DEPTH_TEST);
    mfunOpenglVer->glEnable(GL_CULL_FACE);
    mfunOpenglVer->glFrontFace(GL_CCW);
}

void GlWidget::paintGL() {
    m_shader->bind();
    m_vao->bind();

    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    // set background color
    mfunOpenglVer->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    mfunOpenglVer->glClearColor(black[0], black[1], black[2], black[3]);

    m_shader->setUniformValue(stShaderLocation.offset, 0);

    for (int i = 0; i < 4; ++i) {
        QMatrix4x4 m_matrix;
        m_matrix.rotate(90.0f * (float)i, 0, 0, 1);
        m_matrix.translate(-0.5f, 0.f, -10.f);
        m_matrix.rotate(90.f, 0, 1, 0);
        m_matrix.scale(30.f, 1.f, 30.f);

        m_texture[0]->bind();
        m_shader->setUniformValue(stShaderLocation.mvp, m_projection * m_matrix);

		mfunOpenglVer->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        m_texture[0]->release();
    }

    for (int i = 0; i < 2; ++i) {
        QMatrix4x4 m_matrix;
        m_matrix.rotate(180.0f * (float)i, 0, 1, 0);
        m_matrix.translate(-0.5f, 0.f, -10.f);
        m_matrix.scale(30.f, 1.f, 1.f);

        m_texture[1]->bind();
        m_shader->setUniformValue(stShaderLocation.mvp, m_projection * m_matrix);

        mfunOpenglVer->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        m_texture[1]->release();
    }

    m_shader->release();
    m_vao->release();
}

void GlWidget::resizeGL(int w, int h) {
    aspect = qreal(w) / qreal(h ? h : 1);

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
