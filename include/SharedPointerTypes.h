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
#ifndef SHAREDPOINTERTYPES_H
#define SHAREDPOINTERTYPES_H

#include <memory>
#include <vector>

#define DEFINE_SHARED_PTR_TYPE(ClassName) \
    class ClassName; \
    typedef std::shared_ptr<ClassName> ClassName##Ptr; \
    typedef std::vector<ClassName##Ptr> ClassName##Array;

DEFINE_SHARED_PTR_TYPE(OpenGLMaterialEntity)
DEFINE_SHARED_PTR_TYPE(OpenGLRenderableEntity)

#endif // SHAREDPOINTERTYPES_H
