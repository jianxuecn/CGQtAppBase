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
#ifndef __LogUtils_h
#define __LogUtils_h

#include "Logger.h"
#include <QString>
#include <QByteArray>

inline void log_info(QString const &logmsg) { LOG_INFO(logmsg.toUtf8().constData()); }
inline void log_error(QString const &logmsg) { LOG_ERROR(logmsg.toUtf8().constData()); }
inline void log_warning(QString const &logmsg) { LOG_WARNING(logmsg.toUtf8().constData()); }
inline void log_debug(QString const &logmsg) { LOG_DEBUG(logmsg.toUtf8().constData()); }

#define LOG_INFO_QSTRING(qstring_logmsg)    LOG_INFO(qstring_logmsg.toUtf8().constData())
#define LOG_DEBUG_QSTRING(qstring_logmsg)   LOG_DEBUG(qstring_logmsg.toUtf8().constData())
#define LOG_ERROR_QSTRING(qstring_logmsg)   LOG_ERROR(qstring_logmsg.toUtf8().constData())
#define LOG_WARNING_QSTRING(qstring_logmsg) LOG_WARNING(qstring_logmsg.toUtf8().constData())
#define LOG_SEGMENT_QSTRING(qstring_logmsg) LOG_SEGMENT(qstring_logmsg.toUtf8().constData())


#endif
