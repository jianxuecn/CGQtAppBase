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
#ifndef TRACKBALL_H
#define TRACKBALL_H

#include "glm/gtc/quaternion.hpp"

class TrackBall
{
public:
    TrackBall(float r = 1.0f);
    ~TrackBall();

    float radius() const { return mRadius; }
    void setRadius(float r) { mRadius = r; }

    glm::quat const & currentQuaternion() const { return mCurQuat; }
    glm::quat const & updateQuaternion() const { return mLastQuat; }

    void reset();
    void start(float x, float y);
    void update(glm::quat const &q);
    void update(float x, float y);

private:
    unsigned int mCount;
    float mRadius;
    float mLastX;
    float mLastY;

    glm::quat mCurQuat;
    glm::quat mLastQuat;

};

#endif // TRACKBALL_H
