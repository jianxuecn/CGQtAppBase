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
#ifndef AssimpHelper_h
#define AssimpHelper_h

#include <vector>

#include "assimp/scene.h"          // Output data structure
#include "assimp/postprocess.h"    // Post processing flags

#include "glm/mat4x4.hpp"

/**
 * @brief 从assimp获取网格模型相关信息的辅助数据结构
 */
struct aiRetMeshItem
{
    aiMesh const *mesh;    ///< 网格对象指针
    aiMatrix4x4 transform; ///< 该网格对象相对于整个场景的几何变换矩阵
};

typedef std::vector<aiRetMeshItem> aiRetMeshArray;

/**
 * @brief aiMatrix4x4 到 glm::mat4x4 的转换函数
 * @param[in] aiMat 输入的 aiMatrix4x4 类型矩阵
 * @param[out] glmMat 输出 glm::mat4x4 类型的转换结果
 */
inline void aiMatrix4x4_to_glm_mat4x4(aiMatrix4x4 const &aiMat, glm::mat4x4 &glmMat) {
    glmMat = glm::mat4x4(aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
                         aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
                         aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
                         aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4);
}

/**
 * @brief aiMatrix4x4 到 glm::mat4x4 的转换函数
 * @param aiMat 输入的 aiMatrix4x4 类型矩阵
 * @return glm::mat4x4 类型的转换结果
 */
inline glm::mat4 get_glm_mat4x4(aiMatrix4x4 const &aiMat) {
    return glm::mat4x4(aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
                       aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
                       aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
                       aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4);
}

/**
 * @brief 从读取的场景图中获取第1个网格对象
 * @param node 遍历起始结点（场景图根结点）
 * @param scene 场景图
 * @return 场景图遍历过程中遇到的第1个网格对象
 */
aiMesh const * get_first_mesh(aiNode *node, aiScene const *scene);

/**
 * @brief 从读取的场景图中获取给定名称的网格对象
 * @param node 遍历起始结点（场景图根结点）
 * @param transMat 遍历起始结点相对于根结点的变换矩阵
 * @param scene 场景图
 * @param name 要获取的网格对象名称
 * @return 所需的网格对象及其相对于根结点的变换矩阵
 */
aiRetMeshItem get_mesh_by_name(aiNode *node, aiMatrix4x4 transMat, aiScene const *scene, aiString const &name);

/**
 * @brief 从读取的场景图中获取所有网格对象
 * @param node 遍历起始结点（场景图根结点）
 * @param parentTrans 遍历起始结点的父结点相对于根结点的变换矩阵
 * @param scene 场景图
 * @param meshes 返回存储了所有网格对象及其变换矩阵的数组
 */
void get_all_meshes(aiNode *node, aiMatrix4x4 parentTrans, aiScene const *scene, aiRetMeshArray &meshes);

#endif /* AssimpHelper_h */
