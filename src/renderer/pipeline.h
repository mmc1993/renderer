#pragma once

#include "vec4.h"
#include "math.h"
#include "color.h"
#include "vertex.h"
#include "matrix.h"

class Material;

struct PipelineParam {
    struct { float u, v; } uv;
    Vec4 caemraPos;     //  相机坐标
    Vec4 v;             //  顶点坐标
    Vec4 n;             //  顶点法线
    Color c;            //  顶点颜色
    Matrix4x4 mvp;      //  MVP
    Material *material; //  材质

    Vec4 outv;      //  输出坐标
    Color uvc;      //  贴图颜色
    Color outc;     //  输出颜色
};