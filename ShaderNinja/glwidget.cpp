/*
 * ShaderNinja - A Live OpenGL fragment shader compiler and viewer.
 * Copyright (C) 2015 Maxim Blinov
 *
 * This file is part of ShaderNinja.
 *
 * ShaderNinja is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ShaderNinja is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ShaderNinja.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      shaderProgram(new QOpenGLShaderProgram(parent))
{
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
