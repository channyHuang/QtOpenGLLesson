#include "glwidget.h"

GlWidget::GlWidget(QWidget *parent) : QOpenGLWidget(parent)
{

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
    stShaderLocation.matrixUniform = m_shader->uniformLocation("matrix");
}

static const GLfloat VERTEX_DATA[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};

static GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

void GlWidget::initializeGL() {
    m_vao = new QOpenGLVertexArrayObject;
    m_shader = new QOpenGLShaderProgram;
    m_f = this->context()->functions();
    m_vbo = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);

    m_vao->create();
    m_shader->create();
    m_vbo->create();

    // load shader
    m_vao->bind();
    initShader();

    m_vbo->bind();
    // allocate space of vbo
    m_vbo->allocate(VERTEX_DATA, sizeof(VERTEX_DATA) + sizeof(colors));
    // write color data to buffer
    m_vbo->write(sizeof(VERTEX_DATA), colors, sizeof(colors));

    // set buffer position
    m_shader->setAttributeBuffer(stShaderLocation.posVertex, GL_FLOAT, 0, 3);
    m_shader->setAttributeBuffer(stShaderLocation.colVertex, GL_FLOAT, sizeof(VERTEX_DATA), 3);
    m_shader->enableAttributeArray(stShaderLocation.posVertex);
    m_shader->enableAttributeArray(stShaderLocation.colVertex);

    m_vbo->release();
    m_vao->release();

    m_matrix.setToIdentity();
    m_matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    m_matrix.translate(0.0f, 0.0f, -2.0f);
}

void GlWidget::paintGL() {
    // set background color
    m_f->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_f->glClearColor(0.0f, 0.2f, 0.0f, 1.0f);

    m_matrix.rotate(m_vRotationAngle.x(), 0.0f, 1.0f, 0.0f);
    m_vRotationAngle[0] += 1.f;

    // draw
    m_vao->bind();
    m_shader->bind();
    m_shader->setUniformValue(stShaderLocation.matrixUniform, m_matrix);
    m_f->glDrawArrays(GL_TRIANGLES, 0, 3);
    m_shader->release();
    m_vao->release();
    update();
}

void GlWidget::resizeGL(int w, int h) {

};
