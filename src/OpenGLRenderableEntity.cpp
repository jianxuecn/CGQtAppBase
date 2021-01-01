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
#include "OpenGLRenderableEntity.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLContext>

#include "AssimpHelper.h"
#include "LogUtils.h"
#include "GLUtils.h"

OpenGLRenderableEntity::OpenGLRenderableEntity()
{
    mTriangleVAO = nullptr;
    mVertexBuffer = nullptr;
    mTriangleBuffer = nullptr;
    mOpenGLContext = nullptr;
    mCenter[0] = mCenter[1] = mCenter[2] = 0.0f;
    mBounds[0] = mBounds[1] = mBounds[2] = mBounds[3] = mBounds[4] = mBounds[5] = 0.0f;
    mComponentsPerVertex = 0;
    mHasNormal = false;
    mHasTexCoords = false;
    mOpenGLSetup = false;
    mDataLoaded = false;
    mBufferSetup = false;
}

OpenGLRenderableEntity::~OpenGLRenderableEntity()
{
}

inline void add_vector_2d(VertexDataBuffer &vbuf, aiVector2D const &v) {
    vbuf.emplace_back(v.x);
    vbuf.emplace_back(v.y);
}

inline void add_vector_2d(VertexDataBuffer &vbuf, aiVector3D const &v) {
    vbuf.emplace_back(v.x);
    vbuf.emplace_back(v.y);
}

inline void add_vector_3d(VertexDataBuffer &vbuf, aiVector3D const &v) {
    vbuf.emplace_back(v.x);
    vbuf.emplace_back(v.y);
    vbuf.emplace_back(v.z);
}

inline void add_vector(VertexDataBuffer &vbuf, aiVector3D const &v, unsigned int compNum) {
    if (compNum >= 1) vbuf.emplace_back(v.x);
    if (compNum >= 2) vbuf.emplace_back(v.y);
    if (compNum >= 3) vbuf.emplace_back(v.z);
}

bool OpenGLRenderableEntity::loadData(QOpenGLContext const *glCtx, aiMesh const *mesh)
{
    if (mesh == nullptr) {
        return false;
    }

    if (mesh->mNumVertices <= 0) {
        LOG_ERROR("0 vertices in mesh!");
        return false;
    }

    if (mesh->mNumFaces <= 0) {
        LOG_ERROR("0 triangle face in mesh!");
        return false;
    }

    this->clearData();

    mComponentsPerVertex = 3;

    mHasNormal = (mesh->mNormals != nullptr);
    if (mHasNormal) mComponentsPerVertex += 3;

    unsigned int texNum = mesh->GetNumUVChannels();
    mHasTexCoords = (texNum > 0);
    mTextureComponents.clear();
    for (unsigned int i=0; i<texNum; ++i) {
        mTextureComponents.emplace_back(mesh->mNumUVComponents[i]);
        mComponentsPerVertex += mTextureComponents[i];
    }

    mVertexNumber = mesh->mNumVertices;
    for (unsigned int i = 0; i < mVertexNumber; ++i) {
        add_vector_3d(mVertexData, mesh->mVertices[i]);

        if (mHasNormal) add_vector_3d(mVertexData, mesh->mNormals[i].NormalizeSafe());

        for (size_t ti=0; ti<mTextureComponents.size(); ++ti) {
            add_vector(mVertexData, mesh->mTextureCoords[ti][i], mTextureComponents[ti]);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        if (mesh->mFaces[i].mNumIndices != 3) continue; // ignore non-triangle face
        mIndexData.emplace_back(mesh->mFaces[i].mIndices[0]);
        mIndexData.emplace_back(mesh->mFaces[i].mIndices[1]);
        mIndexData.emplace_back(mesh->mFaces[i].mIndices[2]);
    }
    mTriangleNumber = mIndexData.size() / 3;

    mDataLoaded = true;
    if (!this->setupGL(glCtx)) return false;

    mBufferSetup = false;

    return this->setupBuffers();
}

void OpenGLRenderableEntity::clearData()
{
    mCenter[0] = mCenter[1] = mCenter[2] = 0.0f;
    mBounds[0] = mBounds[1] = mBounds[2] = mBounds[3] = mBounds[4] = mBounds[5] = 0.0f;
    mComponentsPerVertex = 0;
    mHasNormal = false;
    mHasTexCoords = false;
    mDataLoaded = false;
    mBufferSetup = false;

    mVertexData.clear();
    mIndexData.clear();
}

bool OpenGLRenderableEntity::setupGL(QOpenGLContext const *glCtx)
{
    if (mOpenGLSetup) return true;
    if (glCtx == nullptr) return false;

    mOpenGLContext = glCtx;

    if (mVertexBuffer == nullptr) {
        mVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        mVertexBuffer->create();
        mVertexBuffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    }

    if (mTriangleBuffer == nullptr) {
        mTriangleBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
        mTriangleBuffer->create();
        mTriangleBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    }

    if (mTriangleVAO == nullptr) {
        mTriangleVAO = new QOpenGLVertexArrayObject;
        mTriangleVAO->create();
    }

    mOpenGLSetup = true;
    return mOpenGLSetup;
}

void OpenGLRenderableEntity::destroyGL(QOpenGLContext const *glCtx)
{
    if (mOpenGLContext == nullptr) return;
    if (mOpenGLContext != glCtx) {
        LOG_ERROR("Delete OpenGL resources in a different context!");
        return;
    }

    mOpenGLSetup = false;
    mOpenGLContext = nullptr;
    DELETE_OPENGL_RESOURCE(mVertexBuffer);
    DELETE_OPENGL_RESOURCE(mTriangleBuffer);
    DELETE_OPENGL_RESOURCE(mTriangleVAO);
}

void OpenGLRenderableEntity::drawSurface(QOpenGLContext const *glCtx)
{
    if (mOpenGLContext != glCtx) return;
    QOpenGLFunctions *glFuncs = mOpenGLContext->functions();
    QOpenGLVertexArrayObject::Binder triangleVAOBinder(mTriangleVAO);
    glFuncs->glDrawElements(GL_TRIANGLES, mTriangleNumber*3, GL_UNSIGNED_INT, 0);
}

bool OpenGLRenderableEntity::setupBuffers()
{
    if (!mOpenGLSetup || !mDataLoaded) return false;

    for (unsigned int i = 0; i < mVertexNumber; ++i) {
        GLfloat vix = mVertexData[i*mComponentsPerVertex+0];
        GLfloat viy = mVertexData[i*mComponentsPerVertex+1];
        GLfloat viz = mVertexData[i*mComponentsPerVertex+2];
        if (i == 0) {
            mBounds[0] = mBounds[1] = vix;
            mBounds[2] = mBounds[3] = viy;
            mBounds[4] = mBounds[5] = viz;
        } else {
            if (vix < mBounds[0]) mBounds[0] = vix;
            else if (vix > mBounds[1]) mBounds[1] = vix;
            if (viy < mBounds[2]) mBounds[2] = viy;
            else if (viy > mBounds[3]) mBounds[3] = viy;
            if (viz < mBounds[4]) mBounds[4] = viz;
            else if (viz > mBounds[5]) mBounds[5] = viz;
        }
    }
    mCenter[0] = (mBounds[0] + mBounds[1]) * 0.5f;
    mCenter[1] = (mBounds[2] + mBounds[3]) * 0.5f;
    mCenter[2] = (mBounds[4] + mBounds[5]) * 0.5f;

    QOpenGLFunctions *glFuncs = mOpenGLContext->functions();
    QOpenGLVertexArrayObject::Binder triangleVAOBinder(mTriangleVAO);
    mVertexBuffer->bind();
    mVertexBuffer->allocate(mVertexData.data(), mVertexData.size()*sizeof(float));
    mTriangleBuffer->bind();
    mTriangleBuffer->allocate(mIndexData.data(), mIndexData.size()*sizeof(unsigned int));

    glFuncs->glEnableVertexAttribArray(VertexAttribute::POSITION);
    glFuncs->glVertexAttribPointer(VertexAttribute::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(float)*mComponentsPerVertex, 0);
    glFuncs->glEnableVertexAttribArray(VertexAttribute::NORMAL);
    glFuncs->glVertexAttribPointer(VertexAttribute::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(float)*mComponentsPerVertex, (const float*)0 + 3);
    if (!mTextureComponents.empty()) {
        // currently, only one texture is used
        GLint texCoordComps = mTextureComponents[0];
        glFuncs->glEnableVertexAttribArray(VertexAttribute::TEXCOORD);
        glFuncs->glVertexAttribPointer(VertexAttribute::TEXCOORD, texCoordComps, GL_FLOAT, GL_FALSE, sizeof(float)*mComponentsPerVertex, (const float*)0 + 6);
    }
    triangleVAOBinder.release();

    mBufferSetup = true;
    return mBufferSetup;
}
