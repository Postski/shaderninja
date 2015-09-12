#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupTrickyWidgets();

    compilationTimer = new QTimer(this);
    compilationTimer->setInterval(1000);

    filePathShaderSource.clear();

    setupShaderEditor();

    connect(ui->viewport, &GLWidget::shaderCompiled,
            this, &MainWindow::printCompilationLog);

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
}

void MainWindow::openNewFile() {
    //File filters
    QString selectedFilter;

    filePathShaderSource = QFileDialog::getOpenFileName(this,
                                                        "Open Shader Source",
                                                        QDir::currentPath(),
                                                        "All Files (*) ;; GLSL Source File (*.txt *.glsl *.frag)",
                                                        &selectedFilter,
                                                        nullptr);
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
    //Check if trying to save an empty file
    if(ui->textEditShaderProgram->toPlainText() != "") {
        filePathShaderSource = QFileDialog::getSaveFileName(this,
                                                            "Save Shader Source",
                                                            QDir::currentPath(),
                                                            "GLSL Source File (*.frag) ;; Text file (*.txt)",
                                                            nullptr,
                                                            nullptr);
        QFile fileShaderSource(filePathShaderSource);
        fileShaderSource.open(QIODevice::WriteOnly);
        fileShaderSource.write( ui->textEditShaderProgram->toPlainText().toUtf8().data() );
        fileShaderSource.close();
    }
}

void MainWindow::safeQuit() {

}

void MainWindow::launchDialogAbout() {
    DialogAbout dialog(this);
    dialog.exec();
}
