#include "editpane.h"

EditPane::EditPane(QWidget *parent) : QMainWindow(parent) {
    createActions();
    createMenuBar();
//    createToolBar();
    createStatusBar();
    createWidgets();
    createLayouts();
}

void EditPane::createActions() {
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

void EditPane::createMenuBar() {
    menuFile = new QMenu(this);
    menuFile->addAction(actionNewFile);
    menuFile->addAction(actionOpenFile);
    menuFile->addAction(actionSaveFile);
    menuFile->addAction(actionSaveFileAs);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);

    menuBar()->addMenu(menuFile);
    bool test = menuBar()->isHidden();
    bool test2 = menuBar()->isEnabled();

}

void EditPane::createToolBar() {
    toolBarCommon = new QToolBar(this);
    toolBarCommon->addAction(actionCopySelection);
    toolBarCommon->addAction(actionCutSelection);
    toolBarCommon->addAction(actionPasteSelection);
    toolBarCommon->addSeparator();
    toolBarCommon->addAction(actionCompileShaderProgram);
}



void EditPane::createWidgets() {
    widgetMain = new QWidget();

    textEditShaderProgram = new QPlainTextEdit();
    textEditShaderProgram->setMaximumWidth(480);
    textEditShaderProgram->appendPlainText("#version 330 core\n"
                                "varying out vec3 color;\n"
                                "\n"
                                "void main() { \n"
                                "    color = vec3(1, 0, 0);\n"
                                "} \n");

    textEditMessageLog = new QPlainTextEdit();
    textEditMessageLog->setMaximumHeight(120);
    textEditMessageLog->setMaximumWidth(480);
    textEditMessageLog->setReadOnly(true);
    textEditMessageLog->appendPlainText("Started\n");

    splitterWorkspace = new QSplitter();
    splitterWorkspace->setOrientation(Qt::Vertical);
    splitterWorkspace->addWidget(textEditShaderProgram);
    splitterWorkspace->addWidget(textEditMessageLog);
}

void EditPane::createLayouts() {
    layoutMain = new QHBoxLayout();
    layoutMain->addWidget(splitterWorkspace);

    widgetMain->setLayout(layoutMain);
    setCentralWidget(widgetMain);
}

