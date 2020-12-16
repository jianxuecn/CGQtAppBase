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
#ifndef GLUTILS_H
#define GLUTILS_H

#include "GLInc.h"
#include <QString>
#include <functional>

class QOpenGLShaderProgram;

#define DELETE_OPENGL_RESOURCE(x) do { delete x; x = nullptr; } while (0)

inline void set_float4(GLfloat v[4], GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = w;
}

inline void get_float4(GLfloat &x, GLfloat &y, GLfloat &z, GLfloat &w, GLfloat v[4]) {
    x = v[0];
    y = v[1];
    z = v[2];
    w = v[3];
}

inline char const * primitive_type_string(GLenum type)
{
    switch (type) {
    case GL_POINTS: return "POINTS";
    case GL_LINES: return "LINES";
    case GL_TRIANGLES: return "TRIANGLES";
    case GL_QUADS: return "QUADS";
    case GL_POLYGON: return "POLYGONS";
    }

    return "UNKNOWN";
}

inline int primitive_vertex_number(GLenum primType)
{
    switch (primType) {
    case GL_POINTS: return 1;
    case GL_LINES: return 2;
    case GL_TRIANGLES: return 3;
    case GL_QUADS: return 4;
    case GL_POLYGON: return 0;
    default: return -1;
    }
}

bool initialize_shader_program(QString const &pn, QOpenGLShaderProgram *p, QString const &vs, QString const &fs);

// for compatibility with old OpenGL/GLSL before 3.3
bool initialize_shader_program_comp(QString const &pn, QOpenGLShaderProgram *p, QString const &vs, QString const &fs,
                                    std::function<void (QOpenGLShaderProgram *)> bindAttributes);

#endif // GLUTILS_H
