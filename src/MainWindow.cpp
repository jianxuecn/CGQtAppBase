/**
 * -------------------------------------------------------------------------------
 * This source file is part of CGQtAppBase, one of the examples for
 * Computer Graphics Course of School of Engineering Science,
 * University of Chinese Academy of Sciences (UCAS).
 * Copyright (C) 2020 Xue Jian (xuejian@ucas.ac.cn)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * -------------------------------------------------------------------------------
 */
#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->connect(ui->actionFileOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    this->connect(ui->pushButtonPlay, SIGNAL(clicked()), this, SLOT(playAnimation()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Scene File"), QString(),
                                                    tr("Wavefront Object (*.obj);;Collada (*.dae *.xml);;Blender (*.blend)"));

    if (fileName.isEmpty()) return;
    ui->sceneWidget->loadSceneFromFile(fileName);
}

void MainWindow::playAnimation()
{
    if (ui->sceneWidget->animationStarted()) {
        ui->sceneWidget->stopAnimation();
        ui->pushButtonPlay->setText(">");
    } else {
        ui->sceneWidget->startAnimation();
        ui->pushButtonPlay->setText("||");
    }
}

