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
#ifndef __Logger_h
#define __Logger_h

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>

#include <QString>

#include "GLInc.h"

#include "glm/mat4x4.hpp"
#include "glm/mat3x3.hpp"

class QPlainTextEdit;

class Logger : public QObject
{
    Q_OBJECT
public:
    Logger(QObject *parent = 0);
    virtual ~Logger();
    virtual void appendMessage(char const *msg) { std::cout << msg << std::endl; }
    virtual void appendSegment(char const *seg) { std::cout << seg; }
    virtual void appendGenericMessage(char const *msg) { std::cout << "|    INFO | " << msg << std::endl; }
    virtual void appendErrorMessage(char const *msg, char const *fn = 0, int ln = 0) { 
        if (fn) std::cout << "|   ERROR | " << "in " << fn << ", line " << ln << ": " << msg << std::endl;
        else std::cout << "|   ERROR | " << msg << std::endl;
    }
    virtual void appendWarningMessage(char const *msg, char const *fn = 0, int ln = 0) {
        if (fn) std::cout << "| WARNING | " << "in " << fn << ", line " << ln << ": " << msg << std::endl;
        else std::cout << "| WARNING | " << msg << std::endl;
    }
    virtual void appendDebugMessage(char const *msg, char const *fn = 0, int ln = 0) {
        if (fn) std::cout << "|   DEBUG | " << "in " << fn << ", line " << ln << ": " << msg << std::endl;
        else std::cout << "|   DEBUG | " << msg << std::endl;
    }

};

class FileLogger : public Logger
{
    Q_OBJECT
public:
    FileLogger(char const *logfilename, QObject *parent = 0);
    virtual ~FileLogger();
    virtual void appendMessage(char const *msg);
    virtual void appendSegment(char const *seg);
    virtual void appendGenericMessage(char const *msg);
    virtual void appendErrorMessage(char const *msg, char const *fn = 0, int ln = 0);
    virtual void appendWarningMessage(char const *msg, char const *fn = 0, int ln = 0);
    virtual void appendDebugMessage(char const *msg, char const *fn = 0, int ln = 0);

protected:
    std::ofstream mLogFile;
};

class LogManager
{
public:
    static LogManager * instancePtr();
    static LogManager & instance();

    void setLogger(Logger *l) { mLogger = l; }
    Logger * logger() { return mLogger; }

    void appendMessage(char const *msg) { if (mLogger) mLogger->appendMessage(msg); }
    void appendSegment(char const *seg) { if (mLogger) mLogger->appendSegment(seg); }
    
    void appendGenericMessage(char const *msg) { if (mLogger) mLogger->appendGenericMessage(msg); }
    void appendErrorMessage(char const *msg, char const *fn = 0, int ln = 0)   { if (mLogger) mLogger->appendErrorMessage(msg, fn, ln); }
    void appendWarningMessage(char const *msg, char const *fn = 0, int ln = 0) { if (mLogger) mLogger->appendWarningMessage(msg, fn, ln); }
    void appendDebugMessage(char const *msg, char const *fn = 0, int ln = 0)   { if (mLogger) mLogger->appendDebugMessage(msg, fn, ln); }

protected:
    LogManager();
    virtual ~LogManager();

    Logger *mLogger;
};

inline std::ostream &operator<< (std::ostream &o, glm::mat4x4 const &m) {
    o << m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << std::endl;
    o << m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << std::endl;
    o << m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << std::endl;
    o << m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3] << std::endl;

    return o;
}

inline std::ostream &operator<< (std::ostream &o, glm::mat3x3 const &m) {
    o << m[0][0] << " " << m[1][0] << " " << m[2][0] << std::endl;
    o << m[0][1] << " " << m[1][1] << " " << m[2][1] << std::endl;
    o << m[0][2] << " " << m[1][2] << " " << m[2][2] << std::endl;

    return o;
}

/*
#define LOG_INFO(x)    do { std::ostringstream msg; msg << "|    INFO | " << x; LogManager::instancePtr()->appendMessage(msg.str().c_str()); } while (0)
#define LOG_DEBUG(x)   do { std::ostringstream msg; msg << "|   DEBUG | in " __FILE__ ", line " << __LINE__ << ": "<< x; LogManager::instancePtr()->appendMessage(msg.str().c_str()); } while (0)
#define LOG_ERROR(x)   do { std::ostringstream msg; msg << "|   ERROR | in " __FILE__ ", line " << __LINE__ << ": "<< x; LogManager::instancePtr()->appendMessage(msg.str().c_str()); } while (0)
#define LOG_WARNING(x) do { std::ostringstream msg; msg << "| WARNING | in " __FILE__ ", line " << __LINE__ << ": "<< x; LogManager::instancePtr()->appendMessage(msg.str().c_str()); } while (0)
#define LOG_SEGMENT(x) do { std::ostringstream msg; msg << x; LogManager::instancePtr()->appendSegment(msg.str().c_str()); } while (0)
*/

#define LOG_INFO(x)    do { std::ostringstream _log_msg; _log_msg << x; LogManager::instancePtr()->appendGenericMessage(_log_msg.str().c_str()); } while (0)
#define LOG_DEBUG(x)   do { std::ostringstream _log_msg; _log_msg << x; LogManager::instancePtr()->appendDebugMessage(_log_msg.str().c_str(), __FILE__, __LINE__); } while (0)
#define LOG_ERROR(x)   do { std::ostringstream _log_msg; _log_msg << x; LogManager::instancePtr()->appendErrorMessage(_log_msg.str().c_str(), __FILE__, __LINE__); } while (0)
#define LOG_WARNING(x) do { std::ostringstream _log_msg; _log_msg << x; LogManager::instancePtr()->appendWarningMessage(_log_msg.str().c_str(), __FILE__, __LINE__); } while (0)
#define LOG_SEGMENT(x) do { std::ostringstream _log_msg; _log_msg << x; LogManager::instancePtr()->appendSegment(_log_msg.str().c_str()); } while (0)
#define LOG_TEST(x)    do { if (!(x)) LOG_ERROR("Test for \"" #x "\" failed!"); } while (0)
#define LOG_ASSERT(x)  do { if (!(x)) LOG_ERROR("Assert for \"" #x "\" failed!"); } while (0)

#define LOG_MATRIX(n, m) LOG_INFO(n << std::endl << m)
#define GENERIC_MESSAGE(x) LOG_INFO(x)
#define DEBUG_MESSAGE(x) LOG_DEBUG(x)
#define ERROR_MESSAGE(x) LOG_ERROR(x)
#define WARNING_MESSAGE(x) LOG_WARNING(x)

#define DEBUG_OPENGL do { \
    GLenum err = glGetError(); \
    if (err != GL_NO_ERROR) { \
        LOG_DEBUG("[OpenGL ] ERROR " << std::hex << err << std::dec); \
    } else { \
        LOG_DEBUG("[OpenGL ] NOERROR"); \
    } \
} while (0)

#define DEBUG_ON_OPENGL_ERROR do { \
    GLenum err = glGetError(); \
    if (err != GL_NO_ERROR) { \
        LOG_DEBUG("[OpenGL ] ERROR " << std::hex << err << std::dec); \
    } \
} while (0)

#endif
