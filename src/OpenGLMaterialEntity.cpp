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
#include "OpenGLMaterialEntity.h"
#include "GLUtils.h"
#include "LogUtils.h"

#include <QOpenGLTexture>

#include "assimp/material.h"

OpenGLMaterialEntity::OpenGLMaterialEntity()
{
    this->setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
    this->setDiffuse(0.8f, 0.8f, 0.8f, 1.0f);
    this->setSpecular(0.5f, 0.5f, 0.5f, 1.0f);
    this->setEmission(0.0f, 0.0f, 0.0f, 0.0f);
    this->setShininess(50.0f);
    mDiffuseTexture = nullptr;
    mOpenGLContext = nullptr;
    mIsValid = true;
}

OpenGLMaterialEntity::~OpenGLMaterialEntity()
{

}

void OpenGLMaterialEntity::destroyGL(QOpenGLContext const *glCtx)
{
    if (mOpenGLContext == nullptr) return;
    if (mOpenGLContext != glCtx) {
        LOG_ERROR("Delete OpenGL resources in a different context!");
        return;
    }
    mOpenGLContext = nullptr;
    DELETE_OPENGL_RESOURCE(mDiffuseTexture);
    mIsValid = false;

}

void OpenGLMaterialEntity::setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    set_float4(mAmbient, r, g, b, a);
}

void OpenGLMaterialEntity::setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    set_float4(mDiffuse, r, g, b, a);
}

void OpenGLMaterialEntity::setEmission(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    set_float4(mEmission, r, g, b, a);
}

void OpenGLMaterialEntity::setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    set_float4(mSpecular, r, g, b, a);
}

void OpenGLMaterialEntity::setOpacity(GLfloat o)
{
    mDiffuse[3] = o;
}

void OpenGLMaterialEntity::setShininess(GLfloat s)
{
    mShininess = s;
}

QOpenGLTexture * load_texture(QOpenGLTexture *tex, QString const &imageFilePath)
{
    QImage img = QImage(imageFilePath).mirrored();
    if (!tex) {
        tex = new QOpenGLTexture(img);
    } else {
        tex->destroy();
        tex->setData(img);
    }
    return tex;
}

bool OpenGLMaterialEntity::loadDiffuseTexture(QOpenGLContext const *glCtx, QString const &imageFilePath)
{
    if (glCtx == nullptr) {
        LOG_ERROR("Not in a valid OpenGL context!");
        return false;
    }
    if (mOpenGLContext != nullptr && mOpenGLContext != glCtx) {
        LOG_ERROR("Change texture content in a different OpenGL context!");
        return false;
    } else if (mOpenGLContext == nullptr) {
        mOpenGLContext = glCtx;
    }

    mDiffuseTexture = load_texture(mDiffuseTexture, imageFilePath);
    if (!mDiffuseTexture || !mDiffuseTexture->isCreated() || !mDiffuseTexture->isStorageAllocated()) return false;

    mDiffuseTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    mDiffuseTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    return true;
}

bool OpenGLMaterialEntity::loadData(QOpenGLContext const *glCtx, aiMaterial const *material, QString const &textureFilePath)
{
    mIsValid = false;

    if (material == nullptr) {
        LOG_ERROR("aiMaterial material is null");
        return false;
    }

    material->Get(AI_MATKEY_COLOR_AMBIENT, mAmbient, NULL);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, mDiffuse, NULL);
    material->Get(AI_MATKEY_COLOR_EMISSIVE, mEmission, NULL);
    material->Get(AI_MATKEY_COLOR_SPECULAR, mSpecular, NULL);
    material->Get(AI_MATKEY_OPACITY, mDiffuse[3]);
    material->Get(AI_MATKEY_SHININESS, mShininess);

    if (mShininess <= 0.1) mShininess = 1.0f;

    aiString texFilePath;
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texFilePath) == aiReturn_SUCCESS) {
        if (!this->loadDiffuseTexture(glCtx, QDir(textureFilePath).absoluteFilePath(texFilePath.C_Str()))) {
            return false;
        }
        LOG_INFO("diffuse map texture loaded.");
    } else {
        LOG_INFO("no diffuse map texture.");
    }

    mIsValid = true;
    return mIsValid;
}

inline bool check_texture(QOpenGLTexture *tex)
{
    return (tex && tex->isCreated() && tex->isStorageAllocated());
}

bool OpenGLMaterialEntity::diffuseTextureReady() const
{
    return check_texture(mDiffuseTexture);
}
