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

#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include "glwidget.h"
//glwidget.h is also included as part of QOpenGLWidget being promoted to GLWidget from ui design
#include "dialogabout.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>

#include <QTimer>
#include <QString>
#include <QSpinBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;

    void setupTrickyWidgets();
    void setupShaderEditor();
    void setupMessageLog();

    QTimer *compilationTimer;
    QSpinBox *spinboxCompilationDelay;

    QString filePathShaderSource;
    bool isModified;

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
public slots:
    void resetCompilationTimer(int delay);
    void resetCompilationTimer();

    void resendSourceCode();
    void enableAutoCompilation(bool enableflag);

    void createNewFile();
    void openNewFile();
    void saveFile();
    void saveFileAs();

    void printCompilationLog(QString);

    void launchDialogAbout();

    void safeQuit();
};

#endif // MAINWINDOW2_H
