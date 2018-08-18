#pragma once

#include "vec4.h"
#include "math.h"
#include "color.h"
#include "vertex.h"
#include "matrix.h"

class Material;

struct PipelineParam {
    struct { float u, v; } uv;
    Vec4 caemraPos;     //  �������
    Vec4 v;             //  ��������
    Vec4 n;             //  ���㷨��
    Color c;            //  ������ɫ
    Matrix4x4 mvp;      //  MVP
    Material *material; //  ����

    Vec4 outv;      //  �������
    Color uvc;      //  ��ͼ��ɫ
    Color outc;     //  �����ɫ
};