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
#ifndef OPENGLMATERIALENTITY_H
#define OPENGLMATERIALENTITY_H

#include "GLInc.h"
#include "SharedPointerTypes.h"

#include <QString>

class QOpenGLTexture;
class QOpenGLContext;

struct aiMaterial;

/**
 * @brief Class of material for rendering object surface
 */
class OpenGLMaterialEntity
{
public:
    /**
     * @brief the texture unit number for each kind of texture (started from 0)
     */
    enum
    {
        TEXUNIT_DIFFUSE,        ///< texture unit number for diffuse map
        TEXUNIT_NORMAL,         ///< texture unit number for normal map
        TEXUNIT_REFLECTION,     ///< texture unit number for reflection image
        NUM_TEXUNITS            ///< max number of texture units being taken (should be <= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
    };

    /**
     * @brief constructor
     */
    OpenGLMaterialEntity();

    /**
     * @brief destructor
     */
    ~OpenGLMaterialEntity();

    /**
     * @brief release the related OpenGL resources (e.g. textures)
     */
    void destroyGL(QOpenGLContext const *glCtx);

    /**
     * @brief if the material object valid
     */
    bool isValid() const { return mIsValid; }

    QString const & name() const { return mName; }
    void setName(QString const &name) { mName = name; }

    /**
     * @brief ambient color of the surface material
     * @return the pointer to the array containing ambient color (r, b, g, a)
     */
    GLfloat const * ambient() const { return mAmbient; }

    /**
     * @brief diffuse color of the surface material
     * @return the pointer to the array containing diffuse color (r, b, g, a)
     */
    GLfloat const * diffuse() const { return mDiffuse; }

    /**
     * @brief emission color of the surface material
     * @return the pointer to the array containing emission color (r, b, g, a)
     */
    GLfloat const * emission() const { return mEmission; }

    /**
     * @brief specular color of the surface material
     * @return the pointer to the array containing specular color (r, b, g, a)
     */
    GLfloat const * specular() const { return mSpecular; }

    /**
     * @brief opacity of the surface material (taking the alpha component of the diffuse color)
     * @return the value of opacity
     */
    GLfloat opacity() const { return mDiffuse[3]; }

    /**
     * @brief shininess of the surface material
     * @return the value of shininess
     */
    GLfloat shininess() const { return mShininess; }

    /**
     * @brief set the ambient color of the surface material
     * @param r red component (in [0.0, 1.0])
     * @param g green component (in [0.0, 1.0])
     * @param b blue component (in [0.0, 1.0])
     * @param a alpha component (not used)
     */
    void setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

    /**
     * @brief set the diffuse color of the surface material
     * @param r red component (in [0.0, 1.0])
     * @param g green component (in [0.0, 1.0])
     * @param b blue component (in [0.0, 1.0])
     * @param a alpha component (i.e. the opacity, in [0.0, 1.0])
     */
    void setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

    /**
     * @brief set the emission color of the surface material
     * @param r red component (in [0.0, 1.0])
     * @param g green component (in [0.0, 1.0])
     * @param b blue component (in [0.0, 1.0])
     * @param a alpha component (not used)
     */
    void setEmission(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

    /**
     * @brief set the specular color of the surface material
     * @param r red component (in [0.0, 1.0])
     * @param g green component (in [0.0, 1.0])
     * @param b blue component (in [0.0, 1.0])
     * @param a alpha component (not used)
     */
    void setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

    /**
     * @brief set the opacity of the surface material (stored in the alpha component of the diffuse color)
     * @param o the value of opacity (in [0.0, 1.0])
     */
    void setOpacity(GLfloat o);

    /**
     * @brief set the shininess of the surface material
     * @param s the value of shininess (> 0)
     */
    void setShininess(GLfloat s);

    /**
     * @brief the diffuse texture of the surface material
     * @return the pointer to the texture
     */
    QOpenGLTexture* diffuseTexture() const { return mDiffuseTexture; }

    /**
     * @brief load diffuse texture from a file
     * @param imageFilePath the full path-name of the file
     * @param glCtx the OpenGL context in which this function is performed
     * @return true if succeed
     */
    bool loadDiffuseTexture(QOpenGLContext const *glCtx, QString const &imageFilePath);

    /**
     * @brief load material information from the aiMaterial struct of Assimp
     * @param material the pointer to the aiMaterial struct
     * @param textureFilePath the path containing texture files (texture data is not included in aiMaterial)
     * @return true if succeed
     */
    bool loadData(QOpenGLContext const *glCtx, aiMaterial const *material, QString const &textureFilePath = QString());

private:
    QString mName;

    GLfloat mAmbient[4];
    GLfloat mDiffuse[4];
    GLfloat mEmission[4];
    GLfloat mSpecular[4];
    GLfloat mShininess;
    GLfloat mRefractIntensity;

    QOpenGLTexture *mDiffuseTexture;
    QOpenGLContext const *mOpenGLContext;

    bool mIsValid;
};

#endif // OPENGLMATERIALENTITY_H
