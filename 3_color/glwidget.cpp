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
}

static const GLfloat VERTEX_DATA[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
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
    m_vbo->allocate(VERTEX_DATA, 3*3* sizeof(GLfloat));
    m_f->glEnableVertexAttribArray(stShaderLocation.posVertex);
    m_f->glVertexAttribPointer(stShaderLocation.posVertex, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);

    m_vbo->release();
    m_vao->release();
}

void GlWidget::paintGL() {
    // set background color
    m_f->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_f->glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
    // draw
    m_vao->bind();
    m_shader->bind();
    m_f->glDrawArrays(GL_TRIANGLES, 0, 3);
    m_shader->release();
    m_vao->release();
}

void GlWidget::resizeGL(int w, int h) {

};
