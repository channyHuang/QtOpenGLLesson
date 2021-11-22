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
}

void GlWidget::initializeGL() {
    m_vao = new QOpenGLVertexArrayObject;
    m_shader = new QOpenGLShaderProgram;
    m_f = this->context()->functions();
    m_vbo = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);

    m_vao->create();
    m_shader->create();

    // load shader
    m_vao->bind();
    initShader();
    m_vao->release();
}

void GlWidget::paintGL() {}

void GlWidget::resizeGL(int w, int h) {

};
