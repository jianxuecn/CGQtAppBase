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
#include "Light.h"
#include "GLUtils.h"

Light::Light() {
    this->setPosition(0.0f, 0.0f, 0.0f, 1.0f);
    this->setAmbient(0.8f, 0.8f, 0.8f, 1.0f);
    this->setDiffuse(0.9f, 0.9f, 0.9f, 1.0f);
    this->setSpecular(0.8f, 0.8f, 0.8f, 1.0f);
}

Light::~Light() {
    
}

void Light::setPosition(GLfloat x, GLfloat y, GLfloat z, GLfloat w /* = 1.0f */) {
    set_float4(mPosition, x, y, z, w);
}

void Light::setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a /* = 1.0f */) {
    set_float4(mAmbient, r, g, b, a);
}

void Light::setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a /* = 1.0f */) {
    set_float4(mDiffuse, r, g, b, a);
}

void Light::setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a /* = 1.0f */) {
    set_float4(mSpecular, r, g, b, a);
}
