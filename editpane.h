#ifndef EDITPANE_H
#define EDITPANE_H

#include <QMainWindow>

#include <QToolBar>

#include <QAction>
#include <QMenu>
#include <QMenuBar>

#include <QStatusBar>

#include <QPlainTextEdit>
#include <QSplitter>

#include <QHBoxLayout>

class EditPane : public QMainWindow {
    Q_OBJECT

    void createActions();

    void createMenuBar();
    void createToolBar();
    void createStatusBar();

    void createLayouts();
    void createWidgets();

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

    QPlainTextEdit *textEditShaderProgram;
    QPlainTextEdit *textEditMessageLog;
    QSplitter *splitterWorkspace;

    QWidget *widgetMain;
    QHBoxLayout *layoutMain;

public:
    explicit EditPane(QWidget *parent = 0);

signals:

public slots:
};

#endif // EDITPANE_H
