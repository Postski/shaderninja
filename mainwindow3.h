#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glwidget.h"

#include <QMainWindow>

#include <QPlainTextEdit>

#include <QAction>

#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>

#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QTimer>

#include <QFile>
#include <QTextStream>

class MainWindow : public QMainWindow {
    Q_OBJECT

    void createActions();

    void createMenuBar();
    void createToolBar();
    void createStatusBar();

    void createLayouts();

    void configureShaderEditor();
    void configureMessageLog();

    void createWidgets();

    void createConnections();


    QAction *actionNewFile;
    QAction *actionOpenFile;
    QAction *actionSaveFile;
    QAction *actionSaveFileAs;
    QAction *actionQuit;

    QAction *actionCopySelection;
    QAction *actionCutSelection;
    QAction *actionPasteSelection;
    QAction *actionCompileShaderProgram;

    QMenu *menuFile;
    QToolBar *toolBarCommon;

    GLWidget *viewport;
    QPlainTextEdit *textEditShaderProgram;
    QPlainTextEdit *textEditMessageLog;

    QWidget *widgetMain;
    QWidget *widgetEditPane;

    QSplitter *splitterEditPane;
    QSplitter *splitterWorkspace;

    QVBoxLayout *layoutEditPane;
    QHBoxLayout *layoutWindow;

    QTimer *compilationTimer;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void resendSourceCode();
    void resetCompilationTimer();
};

#endif // MAINWINDOW_H
