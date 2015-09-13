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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadSettings();

    filePathShaderSource.clear();

    compilationTimer = new QTimer(this);
    compilationTimer->setInterval(1000);

    connectGenericActions();

    setupTrickyWidgets();
    setupShaderEditor();

    connect(ui->viewport, &GLWidget::shaderCompiled,
            this, &MainWindow::printCompilationLog);

    //Connect compilation buttons
    connect(ui->actionCompile, &QAction::triggered,
            this, &MainWindow::resendSourceCode);
    //  Connect auto-recompilation flags
    connect(ui->actionAutoRecompile, &QAction::toggled,
            this, &MainWindow::enableAutoCompilation);

    //  Connect dialog-launching actions
    connect(ui->actionAbout, &QAction::triggered,
            this, &MainWindow::launchDialogAbout);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::saveSettings() {

    QSettings settings("Maxim Blinov", "ShaderNinja", this);
    settings.beginGroup("Generic");
    settings.setValue("MainWindowDimensions", geometry());
    settings.setValue("editPaneSplitterSize", ui->splitterEditPane->saveState());
    settings.setValue("workspaceSplitterSize", ui->splitterWorkspace->saveState());
    settings.endGroup();

}

void MainWindow::loadSettings() {

    QSettings settings("Maxim Blinov", "ShaderNinja", this);
    settings.beginGroup("Generic");
    setGeometry( settings.value("MainWindowDimensions", QRect(120, 180, 1000, 500) ).toRect() );
    ui->splitterEditPane->restoreState( settings.value("editPaneSplitterSize").toByteArray() );
    ui->splitterWorkspace->restoreState( settings.value("workspaceSplitterSize").toByteArray() );

    //lastFileSavePath is assigned recursively to make sure that it is never equal to null -
    //this negates the need of using any local variables to store the QString,
    //and the value can be safely retrieved whenever it is needed in the code.
    settings.value("lastFileSavPath", settings.value("lastFileSavePath", QDir::currentPath() ).toString() );
    settings.endGroup();
}

void MainWindow::setupTrickyWidgets() {
    //Adds a spinbox at the end of the toolbar (cannot be done through Qt Designer)
    spinboxCompilationDelay = new QSpinBox(this);
    spinboxCompilationDelay->setMaximum(10000);
    spinboxCompilationDelay->setValue(1000);
    spinboxCompilationDelay->setSingleStep(100);
    ui->toolBar->addWidget(spinboxCompilationDelay);
}

void MainWindow::setupShaderEditor() {

    //Load default shader source
    QFile defaultShader(":/shader/defaultshaders/fragment.txt");
    defaultShader.open(QIODevice::ReadOnly);

    QTextStream strin(&defaultShader);
    ui->textEditShaderProgram->appendPlainText(strin.readAll());
    defaultShader.close();

    //Connect the editor contents to the fragment shader input
    connect(ui->textEditShaderProgram, &QPlainTextEdit::textChanged,
            this, (void(MainWindow::*)()) &MainWindow::resetCompilationTimer);

    //By default, connect the timer trigger to the actual recompilation call.
    //This line is (dis)connected by the enableAutoCompilation slot and respective action.
    connect(compilationTimer, &QTimer::timeout,
            this, &MainWindow::resendSourceCode);

    //Connect compilation delay timer spinbox
    connect(spinboxCompilationDelay, (void(QSpinBox::*)(int)) &QSpinBox::valueChanged,
            this, (void(MainWindow::*)(int)) &MainWindow::resetCompilationTimer);

    //Connect generic text editing actions
    connect(ui->actionUndo, &QAction::triggered,
            ui->textEditShaderProgram, &QPlainTextEdit::undo);
    connect(ui->actionRedo, &QAction::triggered,
            ui->textEditShaderProgram, &QPlainTextEdit::redo);
}

void MainWindow::connectGenericActions() {

    //Connect generic actions
    connect(ui->actionNew, &QAction::triggered,
            this, &MainWindow::createNewFile);
    connect(ui->actionOpen, &QAction::triggered,
            this, &MainWindow::openNewFile);
    connect(ui->actionSave, &QAction::triggered,
            this, &MainWindow::saveFile);
    connect(ui->actionSave_As, &QAction::triggered,
            this, &MainWindow::saveFileAs);
    connect(ui->actionQuit, &QAction::triggered,
            this, &MainWindow::close);
}

void MainWindow::reloadDefaultShader() {
    //Load default shader source
    QFile defaultShader(":/shader/defaultshaders/fragment.txt");
    defaultShader.open(QIODevice::ReadOnly);

    QTextStream strin(&defaultShader);
    ui->textEditShaderProgram->appendPlainText(strin.readAll());
    defaultShader.close();
}

void MainWindow::printCompilationLog(QString log) {
    ui->textEditMessageLog->appendPlainText(log);
}

void MainWindow::enableAutoCompilation(bool enableflag) {
    if (enableflag) {
        connect(compilationTimer, &QTimer::timeout,
                this, &MainWindow::resendSourceCode);
        spinboxCompilationDelay->setReadOnly(false);
    } else {
        disconnect(compilationTimer, &QTimer::timeout,
                   this, &MainWindow::resendSourceCode);
        spinboxCompilationDelay->setReadOnly(true);
    }
}

void MainWindow::resetCompilationTimer(int delay) {
    compilationTimer->setInterval(delay);
    compilationTimer->setSingleShot(true);
    compilationTimer->start();
}

void MainWindow::resetCompilationTimer() {
    compilationTimer->setSingleShot(true);
    compilationTimer->start();
}

void MainWindow::resendSourceCode() {
    compilationTimer->stop();
    statusBar()->showMessage("Compiling...", 500);

    ui->textEditMessageLog->clear();
    ui->viewport->updateFragmentShader(ui->textEditShaderProgram->toPlainText());
}

void MainWindow::createNewFile() {
    //Clear the file path variable to prevent overwriting the previous file
    filePathShaderSource.clear();
    ui->textEditShaderProgram->clear();
    reloadDefaultShader();
}

void MainWindow::openNewFile() {
    //File filters (not used for anything)
    QString selectedFilter;

    //Load the last used save path for convenience
    QSettings settings("Maxim Blinov", "ShaderNinja");
    settings.beginGroup("Generic");

    filePathShaderSource = QFileDialog::getOpenFileName(this,
                                                        "Open Shader Source",
                                                        settings.value("lastSaveFilePath").toString(),
                                                        "All Files (*) ;; GLSL Source File (*.txt *.glsl *.frag)",
                                                        &selectedFilter,
                                                        nullptr);

    //Resave the (potentially) new last used filepath name to the persistent QSettings
    settings.setValue("lastSaveFilePath", filePathShaderSource);
    settings.endGroup();

    if(!filePathShaderSource.isEmpty()) {
        QFile fileShaderSource(filePathShaderSource);
        fileShaderSource.open(QIODevice::ReadOnly);

        QTextStream strin(&fileShaderSource);

        ui->textEditShaderProgram->clear();
        ui->textEditShaderProgram->appendPlainText(strin.readAll());
        fileShaderSource.close();
    }
}

void MainWindow::saveFile() {
    //Check if trying to save an empty file or inexistant file
    if(!filePathShaderSource.isEmpty() && ui->textEditShaderProgram->toPlainText() != "") {
        QFile fileShaderSource(filePathShaderSource);
        fileShaderSource.open(QIODevice::WriteOnly);
        fileShaderSource.write( ui->textEditShaderProgram->toPlainText().toUtf8().data() );
        fileShaderSource.close();
    } else {
        emit saveFileAs();
    }
}

void MainWindow::saveFileAs() {
    //Confirm that the user is not trying to save an empty file
    if(ui->textEditShaderProgram->toPlainText() != "") {
        //Load up the settings to point dialog to the last directory used, for convenience
        QSettings settings("Maxim Blinov", "ShaderNinja", this);
        settings.beginGroup("Generic");

        filePathShaderSource = QFileDialog::getSaveFileName(this,
                                                            "Save Shader Source",
                                                            settings.value("lastSaveFilePath").toString(),
                                                            "GLSL Source File (*.frag) ;; Text file (*.txt)",
                                                            nullptr,
                                                            nullptr);

        //Resave the (potentially) new last used filepath name to the persistent QSettings
        settings.setValue("lastSaveFilePath", filePathShaderSource);
        settings.endGroup();

        //Now the file is saved
        QFile fileShaderSource(filePathShaderSource);
        fileShaderSource.open(QIODevice::WriteOnly);
        fileShaderSource.write( ui->textEditShaderProgram->toPlainText().toUtf8().data() );
        fileShaderSource.close();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    //By re-implementing closeEvent, the saveSettings function is called even if the window
    //is closed by dialog operations (e.g. by pressing x or double clicking the top-left icon
    saveSettings();
    event->accept();
}


void MainWindow::launchDialogAbout() {
    DialogAbout dialog(this);
    dialog.exec();
}
