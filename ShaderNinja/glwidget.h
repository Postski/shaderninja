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

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <QTime>
#include <QTimer>

#include <QVector>
#include <QResource>

#include <QtDebug>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

    QOpenGLShaderProgram *shaderProgram;

    QTime *time;
    QTimer *framerate;

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void initializeGL();
    void resizeGL();
    void paintGL();

signals:
    void shaderCompiled(const QString compilationLog);

public slots:
    void updateFragmentShader(QString newsource);
};

#endif // GLWIDGET_H
