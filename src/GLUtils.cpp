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
#include "GLUtils.h"
#include "LogUtils.h"

#include <QOpenGLShaderProgram>
//#include <QMessageBox>

QString const SHADER_PATH = ":/shaders/";

bool compile_shader_program(QString const &pn, QOpenGLShaderProgram *p, QString const &vs, QString const &fs)
{
    if (!p) return false;

    QString logtitle, logstr;
    if (!p->addShaderFromSourceFile(QOpenGLShader::Vertex, SHADER_PATH+vs)) {
        logtitle = QString("%1: Vertex Shader %2 Error").arg(pn).arg(vs);
        logstr =  p->log();
        //QMessageBox::critical(this, logtitle, logstr);
        LOG_ERROR_QSTRING(logtitle);
        LOG_SEGMENT_QSTRING(logstr);
        return false;
    }
    logstr = p->log();
    if (!logstr.isEmpty()) {
        logtitle = QString("%1: Vertex Shader %2 Warning").arg(pn).arg(vs);
        LOG_WARNING_QSTRING(logtitle);
        LOG_SEGMENT_QSTRING(logstr);
    }

    if (!p->addShaderFromSourceFile(QOpenGLShader::Fragment, SHADER_PATH+fs)) {
        logtitle = QString("%1: Fragment Shader %2 Error").arg(pn).arg(fs);
        logstr =  p->log();
        //QMessageBox::critical(this, logtitle, logstr);
        LOG_ERROR_QSTRING(logtitle);
        LOG_SEGMENT_QSTRING(logstr);
        return false;
    }
    logstr = p->log();
    if (!logstr.isEmpty()) {
        logtitle = QString("%1: Fragment Shader %2 Warning").arg(pn).arg(fs);
        LOG_WARNING_QSTRING(logtitle);
        LOG_SEGMENT_QSTRING(logstr);
    }

    return true;
}

bool link_shader_program(QString const &pn, QOpenGLShaderProgram *p)
{
    if (!p) return false;

    QString logtitle, logstr;
    if (!p->link()) {
        logtitle = QString("%1: Link Error").arg(pn);
        logstr =  p->log();
        //QMessageBox::critical(this, logtitle, logstr);
        LOG_ERROR_QSTRING(logtitle);
        LOG_SEGMENT_QSTRING(logstr);
        return false;
    }
    logstr = p->log();
    if (!logstr.isEmpty()) {
        logtitle = QString("%1: Link Warning").arg(pn);
        LOG_WARNING_QSTRING(logtitle);
        LOG_SEGMENT_QSTRING(logstr);
    }

    return true;
}

bool initialize_shader_program(QString const &pn, QOpenGLShaderProgram *p, QString const &vs, QString const &fs)
{
    if (!p) return false;

    if (!compile_shader_program(pn, p, vs, fs)) return false;
    if (!link_shader_program(pn, p)) return false;

    LOG_INFO_QSTRING(QString("Shader program %1 (%2, %3) initialized successfully!").arg(pn).arg(vs).arg(fs));

    return true;
}

bool initialize_shader_program_comp(const QString &pn, QOpenGLShaderProgram *p, const QString &vs, const QString &fs, std::function<void (QOpenGLShaderProgram *)> bindAttributes)
{
    if (!p) return false;

    if (!compile_shader_program(pn, p, vs, fs)) return false;
    if (bindAttributes) bindAttributes(p);
    if (!link_shader_program(pn, p)) return false;

    LOG_INFO_QSTRING(QString("Shader program %1 (%2, %3) initialized successfully!").arg(pn).arg(vs).arg(fs));

    return true;
}
