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
#ifndef OPENGLRENDERABLEENTITY_H
#define OPENGLRENDERABLEENTITY_H

#include "SharedPointerTypes.h"
#include "glm/mat4x4.hpp"

#include <QString>

typedef std::vector<float> VertexDataBuffer;
typedef std::vector<unsigned int> IndexDataBuffer;

struct aiMesh;

class QOpenGLVertexArrayObject;
class QOpenGLBuffer;
class QOpenGLContext;

class OpenGLRenderableEntity
{
public:
    OpenGLRenderableEntity();
    ~OpenGLRenderableEntity();

    float const * bounds() const { return mBounds; }
    float const * center() const { return mCenter; }

    bool hasNormal() const { return mHasNormal; }
    bool hasTexCoords() const { return mHasTexCoords; }

    QString const & name() const { return mName; }
    void setName(QString const &name) { mName = name; }

    void setMaterial(OpenGLMaterialEntityPtr m) { mMaterial = m; }
    OpenGLMaterialEntityPtr material() const { return mMaterial.lock(); }

    bool loadData(QOpenGLContext const *glCtx, aiMesh const *mesh);
    void clearData();

    bool setupGL(QOpenGLContext const *glCtx);
    void destroyGL(QOpenGLContext const *glCtx);

    void drawSurface(QOpenGLContext const *glCtx);

private:
    bool setupBuffers();

private:
    QString mName;

    QOpenGLVertexArrayObject *mTriangleVAO;
    QOpenGLBuffer *mVertexBuffer;
    QOpenGLBuffer *mTriangleBuffer;
    QOpenGLContext const *mOpenGLContext;

    VertexDataBuffer mVertexData;
    IndexDataBuffer mIndexData;

    std::weak_ptr<OpenGLMaterialEntity> mMaterial;
    std::vector<unsigned int> mTextureComponents;
    unsigned int mVertexNumber;
    unsigned int mTriangleNumber;
    unsigned int mComponentsPerVertex;
    float mBounds[6];
    float mCenter[3];
    bool mHasNormal;
    bool mHasTexCoords;
    bool mOpenGLSetup;
    bool mDataLoaded;
    bool mBufferSetup;
};

#endif // OPENGLRENDERABLEENTITY_H
