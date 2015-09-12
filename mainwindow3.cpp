#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , compilationTimer(new QTimer(this))
{
    resize(1000, 500);

    createActions();
    createMenuBar();
    createToolBar();
    createStatusBar();
    createWidgets();

    configureShaderEditor();
    configureMessageLog();

    createLayouts();

    createConnections();
}

MainWindow::~MainWindow() {

}

void MainWindow::createActions() {
    actionNewFile = new QAction("New File", this);
    actionOpenFile = new QAction("Open File", this);
    actionSaveFile = new QAction("Save File", this);
    actionSaveFileAs = new QAction("Save File As...", this);
    actionQuit = new QAction("Quit", this);

    actionCopySelection = new QAction("Copy", this);
    actionCutSelection = new QAction("Cut", this);
    actionPasteSelection = new QAction("Paste", this);
    actionCompileShaderProgram = new QAction("Compile Shader Program", this);
}
void MainWindow::createMenuBar() {
    menuFile = new QMenu("File", this);
    menuFile->addAction(actionNewFile);
    menuFile->addAction(actionOpenFile);
    menuFile->addAction(actionSaveFile);
    menuFile->addAction(actionSaveFileAs);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);

    menuBar()->addMenu(menuFile);
}
void MainWindow::createToolBar() {
    toolBarCommon = new QToolBar(this);
    toolBarCommon->isAreaAllowed(Qt::LeftToolBarArea);
    toolBarCommon->setOrientation(Qt::Vertical);
    addToolBar(toolBarCommon);

    toolBarCommon->addAction(actionCopySelection);
    toolBarCommon->addAction(actionCutSelection);
    toolBarCommon->addAction(actionPasteSelection);
    toolBarCommon->addSeparator();
    toolBarCommon->addAction(actionCompileShaderProgram);
}
void MainWindow::createStatusBar() {
    statusBar()->showMessage("Ready", 2000);
}
void MainWindow::createWidgets() {
    viewport = new GLWidget(this);
    textEditShaderProgram = new QPlainTextEdit(this);
    textEditMessageLog = new QPlainTextEdit(this);

    widgetEditPane = new QWidget(this);
    widgetMain = new QWidget(this);

    layoutEditPane = new QVBoxLayout();
    layoutWindow = new QHBoxLayout();
    splitterWorkspace = new QSplitter(this);
    splitterEditPane = new QSplitter(this);
}
void MainWindow::configureShaderEditor() {
    textEditShaderProgram->setWordWrapMode(QTextOption::NoWrap);

    textEditShaderProgram->setFont(QFont("Courier", 9));

    QFontMetrics metrics(textEditShaderProgram->font());
    textEditShaderProgram->setTabStopWidth(4 * metrics.width(' '));

    QFile fInput(":/shader/fragment.txt");
    fInput.open(QIODevice::ReadOnly);

    QTextStream sin(&fInput);
    textEditShaderProgram->appendPlainText(sin.readAll());
    fInput.close();
}
void MainWindow::configureMessageLog() {
    textEditMessageLog->setWordWrapMode(QTextOption::NoWrap);

    textEditMessageLog->setReadOnly(true);
    textEditMessageLog->appendPlainText("Started");
}
void MainWindow::createLayouts() {
    splitterEditPane->setOrientation(Qt::Vertical);
    splitterEditPane->addWidget(textEditShaderProgram);
    splitterEditPane->addWidget(textEditMessageLog);
    splitterEditPane->setCollapsible(0, 0);
    splitterEditPane->setCollapsible(1, 0);

    QList<int> splitterEditPaneSizes = splitterEditPane->sizes();
    splitterEditPaneSizes[0] = 350;
    splitterEditPaneSizes[1] = 150;
    splitterEditPane->setSizes(splitterEditPaneSizes);

    layoutEditPane->addWidget(splitterEditPane);
    widgetEditPane->setLayout(layoutEditPane);

    splitterWorkspace->setOrientation(Qt::Horizontal);
    splitterWorkspace->addWidget(widgetEditPane);
    splitterWorkspace->addWidget(viewport);
    splitterWorkspace->setCollapsible(0, 1);
    splitterWorkspace->setCollapsible(1, 0);

    QList<int> splitterWorkspaceSizes = splitterWorkspace->sizes();
    splitterWorkspaceSizes[0] = 500;
    splitterWorkspaceSizes[1] = 500;
    splitterWorkspace->setSizes(splitterWorkspaceSizes);

    layoutWindow->addWidget(splitterWorkspace);
    widgetMain->setLayout(layoutWindow);

    setCentralWidget(widgetMain);
}


void MainWindow::resendSourceCode() {
    compilationTimer->stop();
    statusBar()->showMessage("Compiling...", 500);

    textEditMessageLog->clear();
    viewport->updateFragmentShader(textEditShaderProgram->toPlainText());
}

void MainWindow::resetCompilationTimer() {
    compilationTimer->setInterval(1000);
    compilationTimer->setSingleShot(true);
    compilationTimer->start();
}

void MainWindow::createConnections() {
    //Compilation messages
    connect(viewport, &GLWidget::shaderCompiled,
            textEditMessageLog, &QPlainTextEdit::appendPlainText);
    //Shader source code update: start timer
    connect(textEditShaderProgram, &QPlainTextEdit::textChanged,
            this, &MainWindow::resetCompilationTimer);
    //Shader source code update: update frag shader upon QTimer timeout()
    connect(compilationTimer, &QTimer::timeout,
            this, &MainWindow::resendSourceCode);
}
