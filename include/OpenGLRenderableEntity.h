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
    /**
     * @brief constructor
     */
    OpenGLRenderableEntity();

    /**
     * @brief destructor
     */
    ~OpenGLRenderableEntity();

    /**
     * @brief bounding box of this renderable entity
     * @return a float pointer to the bounding box array containing 6 float values: [xmin, xmax, ymin, ymax, zmin, zmax]
     */
    float const * bounds() const { return mBounds; }

    /**
     * @brief center of (the bounding box of) this renderale entity
     * @return a float pointer to the center array containing 3 float values: [cx, cy, cz]
     */
    float const * center() const { return mCenter; }

    /**
     * @brief whether the mesh data of this renderable entity contains normal attribute for each vertex
     * @return true if the mesh data of this renderable entity contains normal attribute for each vertex
     */
    bool hasNormal() const { return mHasNormal; }

    /**
     * @brief whether the mesh data of this renderable entity contains texture coordinate attribute for each vertex
     * @return true if the mesh data of this renderable entity contains texture coordinate attribute for each vertex
     */
    bool hasTexCoords() const { return mHasTexCoords; }

    /**
     * @brief the name of this renderable entity
     * @return the name of this renderable entity
     */
    QString const & name() const { return mName; }

    /**
     * @brief set the name of this renderable entity
     * @param name the name to be set
     */
    void setName(QString const &name) { mName = name; }

    /**
     * @brief set the material for rendering this entity
     * @param m the shared ptr to the material
     */
    void setMaterial(OpenGLMaterialEntityPtr m) { mMaterial = m; }

    /**
     * @brief the material for rendering this entity
     * @return the shared ptr of the meterial for rendering this entity
     */
    OpenGLMaterialEntityPtr material() const { return mMaterial.lock(); }

    /**
     * @brief load data from an aiMesh (read from a model file by Assimp)
     * @param glCtx current OpenGL context (must not be null)
     * @param mesh the pointer to the aiMesh to load data from
     * @return true if the loading process succeeded
     * @note this function will call setupGL() and setupBuffers()
     */
    bool loadData(QOpenGLContext const *glCtx, aiMesh const *mesh);

    /**
     * @brief clear the mesh data (on CPU side)
     */
    void clearData();

    /**
     * @brief setup (initialize) some OpenGL objects (e.g. Vertex Buffer Object) for rendering
     * @param glCtx current OpenGL context (must not be null)
     * @return true if succeeded
     */
    bool setupGL(QOpenGLContext const *glCtx);

    /**
     * @brief destroy the OpenGL resources
     * @param glCtx current OpenGL context (must be the same as the context in which setupGL() was called)
     */
    void destroyGL(QOpenGLContext const *glCtx);

    /**
     * @brief draw the surface
     * @param glCtx current OpenGL context (must be the same as the context in which setupGL() was called)
     */
    void drawSurface(QOpenGLContext const *glCtx);

    /**
     * @brief do some perturbation on the position coordinates of the vertices
     */
    void perturbVertices();

private:
    bool setupBuffers();

private:
    QString mName;

    QOpenGLVertexArrayObject *mTriangleVAO;
    QOpenGLBuffer *mVertexBuffer;
    QOpenGLBuffer *mTriangleBuffer;
    QOpenGLContext const *mOpenGLContext;

    VertexDataBuffer mVertexData;
    VertexDataBuffer mOriginalVertexData;
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
    bool mVertexDataModified;
};

#endif // OPENGLRENDERABLEENTITY_H
