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
