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
#include "Logger.h"

#include <QPlainTextEdit>
#include <QStringBuilder>
#include <QTime>
#include <QDateTime>
#include <QApplication>
#include <QDir>

inline QString current_time_string()
{
    return QTime::currentTime().toString("hh:mm:ss.zzz");
}

inline QString current_date_time_string()
{
    return QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
}

Logger::Logger(QObject *parent /* = 0 */) : QObject(parent)
{

}

Logger::~Logger()
{

}

//======================================================

FileLogger::FileLogger(char const *logfilename, QObject *parent /* = 0 */) : Logger(parent)
{
    mLogFile.open(logfilename);
}

FileLogger::~FileLogger()
{
    //LOG_INFO("FileLogger " << (unsigned int)this << " destroy!");
    mLogFile.close();
}

void FileLogger::appendMessage(const char *msg)
{
    mLogFile << msg << std::endl;
}

void FileLogger::appendSegment(const char *seg)
{
    mLogFile << seg;
}

void FileLogger::appendGenericMessage(const char *msg)
{
    mLogFile << "| " << current_date_time_string().toStdString() << "    INFO | " << msg << std::endl;
}

void FileLogger::appendWarningMessage(char const *msg, char const *fn /* = 0 */, int ln /* = 0 */)
{
    if (fn) mLogFile << "| " << current_date_time_string().toStdString() << " WARNING | " << "in " << fn << ", line " << ln << ": " << msg << std::endl;
    else mLogFile << "| " << current_date_time_string().toStdString() << " WARNING | " << msg << std::endl;
}

void FileLogger::appendErrorMessage(char const *msg, char const *fn /* = 0 */, int ln /* = 0 */)
{
    if (fn) mLogFile << "| " << current_date_time_string().toStdString() << "   ERROR | " << "in " << fn << ", line " << ln << ": " << msg << std::endl;
    else mLogFile << "| " << current_date_time_string().toStdString() << "   ERROR | " << msg << std::endl;
}

void FileLogger::appendDebugMessage(char const *msg, char const *fn /* = 0 */, int ln /* = 0 */)
{
    if (fn) mLogFile << "| " << current_date_time_string().toStdString() << "   DEBUG | " << "in " << fn << ", line " << ln << ": " << msg << std::endl;
    else mLogFile << "| " << current_date_time_string().toStdString() << "   DEBUG | " << msg << std::endl;
}

//======================================================

LogManager::LogManager()
{
    mLogger = nullptr;
    //mLogger = new FileLogger("ExpreQuantTool.log");
}

LogManager::~LogManager()
{
    delete mLogger;
}

LogManager* LogManager::instancePtr()
{
    return &(instance());
}

LogManager& LogManager::instance()
{
    static LogManager theLogManager;
    return theLogManager;
}
