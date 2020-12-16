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
#include "TrackBall.h"

unsigned int MAX_UPDATE_COUNT = 97;

inline void set_identity(glm::quat &q)
{
    q.x = 0.0f;
    q.y = 0.0f;
    q.z = 0.0f;
    q.w = 1.0f;
}

static float project_to_sphere(float r, float x, float y)
{
    float d, z;

    d = x*x + y*y;
    if (d < r * r * 0.5f) // d < r / sqrt(2) --> inside sphere
    {
        z = sqrt(r*r - d);
    }
    else // On hyperbola
    {
        z = 0.5f * r * r / sqrt(d);
    }

    return z;
}


TrackBall::TrackBall(float r)
{
    mCount = 0;
    mRadius = r;
    this->reset();
}

TrackBall::~TrackBall()
{

}

void TrackBall::reset()
{
    set_identity(mCurQuat);
}

void TrackBall::start(float x, float y)
{
    mLastX = x;
    mLastY = y;
    set_identity(mLastQuat);
}

void TrackBall::update(glm::quat const &q)
{
    mCurQuat = q * mCurQuat;
    if (++mCount > MAX_UPDATE_COUNT)
    {
        mCount = 0;
        glm::normalize(mCurQuat);
    }
}

void TrackBall::update(float x, float y)
{
    glm::vec3 a; /* Axis of rotation */
    glm::vec3 p1, p2;
    float phi;  /* how much to rotate about axis */
    float t;

    //  Might just be able to return here.
    if (mLastX == x && mLastY == y)
    {
        /* Zero rotation */
        set_identity(mLastQuat);
        return;
    }

    /*
     * First, figure out z-coordinates for projection of P1 and P2 to
     * deformed sphere
     */
    p1.x = mLastX;
    p1.y = mLastY;
    p1.z = project_to_sphere(mRadius, mLastX, mLastY);

    p2.x = x;
    p2.y = y;
    p2.z = project_to_sphere(mRadius, x, y);

    /*
    *  Now, we want the cross product of P1 and P2
    */
    a = glm::normalize(glm::cross(p1, p2));

    /*
    *  Figure out how much to rotate around that axis.
    */
    t = glm::distance(p1, p2) / (2.0f * mRadius);

    /*
    * Avoid problems with out-of-control values...
    */
    if (t > 1.0f) t = 1.0f;
    if (t < -1.0f) t = -1.0f;
    phi = 2.0f * asin(t);

    mLastQuat = glm::angleAxis(phi, a);

    mLastX = x;
    mLastY = y;

    this->update(mLastQuat);
}
