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
