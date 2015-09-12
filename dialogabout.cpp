#include "dialogabout.h"
#include "ui_dialogabout.h"

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    //Magically makes the dialog transparent without screwing itself up or having a black border
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    ui->setupUi(this);

    connect(ui->buttonOk, &QPushButton::clicked,
            this, &QDialog::close);
}

DialogAbout::~DialogAbout()
{
    delete ui;
}
