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
#include "LogUtils.h"
#include "GLInc.h"
#include "AppInfo.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QDir>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(APP_ORG_NAME);
    QCoreApplication::setOrganizationDomain(APP_ORG_DOMAIN);
    QCoreApplication::setApplicationName(APP_NAME);

#ifdef USE_OPENGLES
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
#endif

    QApplication a(argc, argv);

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
#if defined(USE_COMPATIBILITY_PROFILE) || defined(USE_OPENGLES)
    // 兼容传统OpenGL API
    //format.setProfile(QSurfaceFormat::CompatibilityProfile);
#   ifdef USE_OPENGLES
        format.setRenderableType(QSurfaceFormat::OpenGLES);
#   endif
#else
    // 不兼容传统OpenGL API，可设置具体版本
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
#endif

    // 设置帧缓存相关的其他参数
    format.setDepthBufferSize(24); // 深度缓存位数
    format.setStencilBufferSize(8); // 模板缓存位数
    format.setSamples(16); // 全屏反走样的采样数
    QSurfaceFormat::setDefaultFormat(format);

    QString logFilePath = QCoreApplication::applicationDirPath();
    QDir logFileDir(logFilePath);
    FileLogger *logger = new FileLogger(logFileDir.absoluteFilePath(APP_NAME ".log").toLocal8Bit().constData());
    LogManager::instance().setLogger(logger);

    QString arch;
    if (sizeof(void*) == 4) arch = "x86";
    else if (sizeof(void*) == 8) arch = "x64";
    else arch = "unknown arch";
    QString appinfo = QString("Start %1 %2.%3.%4 (%5)")
        .arg(APP_NAME)
        .arg(APP_VERSION_MAJOR)
        .arg(APP_VERSION_MINOR)
        .arg(APP_VERSION_PATCH)
        .arg(arch);
    log_info(appinfo);
    log_info(QString("Working dir: %1").arg(QCoreApplication::applicationDirPath()));

    MainWindow w;
    w.show();

    int retCode = a.exec();
    log_info(QString(APP_NAME " exited with code %1.").arg(retCode));

    return retCode;
}
