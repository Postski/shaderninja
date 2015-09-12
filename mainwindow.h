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
