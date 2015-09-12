#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      shaderProgram(new QOpenGLShaderProgram(parent))
{
    setMinimumWidth(240);

    time = new QTime(0, 0, 0, 0);
    framerate = new QTimer(parent);
    framerate->setInterval(1000/45);
    framerate->setSingleShot(false);
    connect(framerate, &QTimer::timeout,
            this, (void (GLWidget::*)())&GLWidget::update);
    framerate->start();
}

GLWidget::~GLWidget() {
    delete time;
}

void GLWidget::initializeGL() {
    QOpenGLFunctions::initializeOpenGLFunctions();

    shaderProgram->bind();
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/defaultshaders/vertex.txt");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/defaultshaders/fragment.txt");
    shaderProgram->link();

    time->start();
}

void GLWidget::resizeGL() {

}

void GLWidget::paintGL() {

    static QVector<GLfloat> quadVertices = {
       -1.0f, 1.0f, 0.0f,
       -1.0f, -1.0f, 0.0f,
       1.0f,  -1.0f, 0.0f,

       -1.0f, 1.0f, 0.0f,
       1.0f,  -1.0f, 0.0f,
       1.0f,  1.0f, 0.0f,
    };

    shaderProgram->bind();
    shaderProgram->link();

    int vertexLocation = shaderProgram->attributeLocation("vertex");
    int timeLocation = shaderProgram->uniformLocation("time");
    int resolutionLocation = shaderProgram->uniformLocation("resolution");

    shaderProgram->enableAttributeArray(vertexLocation);
    shaderProgram->setAttributeArray(vertexLocation, quadVertices.data(), 3);
    shaderProgram->setUniformValue(timeLocation, time->elapsed() / 1000.0f);
    shaderProgram->setUniformValue(resolutionLocation, width(), height());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

    shaderProgram->disableAttributeArray(shaderProgram->attributeLocation("vertex"));
}

void GLWidget::updateFragmentShader(QString newsource) {
    makeCurrent();

    shaderProgram->bind();
    shaderProgram->removeAllShaders();

    if(!shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/defaultshaders/vertex.txt")) {
        emit shaderCompiled(shaderProgram->log());
    }
    if(!shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, newsource)) {
        emit shaderCompiled(shaderProgram->log());
    }
    if(!shaderProgram->link()) {
        emit shaderCompiled(shaderProgram->log());
    }

    time->restart();
    update();
}
