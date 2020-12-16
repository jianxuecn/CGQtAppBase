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
#ifndef Light_h
#define Light_h

#include "GLInc.h"

#include "glm/vec4.hpp"

/**
 * @brief Class for light source.
 */
class Light
{
public:
    /**
     * @brief constructor
     *
     * Construct the light object, initialize with:
     * - position: (0.0, 0.0, 0.0, 1.0)
     * - ambient: (0.8, 0.8, 0.8, 1.0)
     * - diffuse: (1.0, 1.0, 1.0, 1.0)
     * - specular: (0.8, 0.8, 0.8, 1.0)
     */
    Light();

    /**
     * @brief destructor
     */
    ~Light();

    /**
     * @brief position of the light
     * @return the pointor to the array containing (x, y, z, w).
     */
    GLfloat const * position() const { return mPosition; }

    void getPosition(glm::vec4 &pos) {
        pos.x = mPosition[0];
        pos.y = mPosition[1];
        pos.z = mPosition[2];
        pos.w = mPosition[3];
    }

    /**
     * @brief ambient color of the light source
     * @return the pointor to the array containing ambient color (r, b, g, a)
     */
    GLfloat const * ambient() const { return mAmbient; }

    /**
     * @brief diffuse color of the light source
     * @return the pointor to the array containing diffuse color (r, b, g, a)
     */
    GLfloat const * diffuse() const { return mDiffuse; }

    /**
     * @brief specular color of the light source
     * @return the pointor to the array containing specular color (r, b, g, a)
     */
    GLfloat const * specular() const { return mSpecular; }

    /**
     * @brief set the position of the light
     * @param x x-coordinate of the position
     * @param y y-coordinate of the position
     * @param z z-coordinate of the position
     * @param w w-coordinate of the position: 1 means positional light, 0 means directional light
     */
    void setPosition(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f);

    /**
     * @brief set the position of the light (provided for convenience)
     * @param pos an array containing position coordinates (x, y, z, w)
     */
    void setPosition(GLfloat pos[4]) { this->setPosition(pos[0], pos[1], pos[2], pos[3]); }

    /**
     * @brief set the ambient color of the light source
     * @param r red component (in [0.0, 1.0])
     * @param g green component (in [0.0, 1.0])
     * @param b blue component (in [0.0, 1.0])
     * @param a alpha component (not used)
     */
    void setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

    /**
     * @brief set the ambient color of the light source (provided for convenience)
     * @param amb an array containing ambient color (r, g, b, a)
     */
    void setAmbient(GLfloat amb[4]) { this->setAmbient(amb[0], amb[1], amb[2], amb[3]); }

    /**
     * @brief set the diffuse color of the light source
     * @param r red component (in [0.0, 1.0])
     * @param g green component (in [0.0, 1.0])
     * @param b blue component (in [0.0, 1.0])
     * @param a alpha component (not used)
     */
    void setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

    /**
     * @brief set the diffuse color of the light source (provided for convenience)
     * @param amb an array containing diffuse color (r, g, b, a)
     */
    void setDiffuse(GLfloat dif[4]) { this->setDiffuse(dif[0], dif[1], dif[2], dif[3]); }

    /**
     * @brief set the specular color of the light source
     * @param r red component (in [0.0, 1.0])
     * @param g green component (in [0.0, 1.0])
     * @param b blue component (in [0.0, 1.0])
     * @param a alpha component (not used)
     */
    void setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

    /**
     * @brief set the specular color of the light source (provided for convenience)
     * @param amb an array containing specular color (r, g, b, a)
     */
    void setSpecular(GLfloat spe[4]) { this->setSpecular(spe[0], spe[1], spe[2], spe[3]); }

protected:
    GLfloat mPosition[4];
    GLfloat mAmbient[4];
    GLfloat mDiffuse[4];
    GLfloat mSpecular[4];
};

#endif // Light_h
