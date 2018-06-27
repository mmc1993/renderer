#pragma once

#include "../base.h"
#include "matrix.h"
#include "color.h"
#include "vec4.h"
#include "math.h"

class Shader {
public:
    struct Param {
        Matrix4x4 vpMat;            //  ��ͼ * ͶӰ ����
        Vec4 lightPoint;            //  �ƹ�����
        Vec4 cameraPoint;           //  �������
        Vec4 ambientLight;          //  ������
        Vec4 directLight;           //  �����
        Color color;                //  ��ɫ
        Vec4 point;                 //  ����
        Vec4 normal;                //  ����
        float u, v;                 //  uv

        //  ���
        Vec4 renderPoint;           //  ��Ⱦ����
        Vec4 renderColor;           //  ��Ⱦ��ɫ
    };

public:
    Shader() { }
    virtual ~Shader() { }
    virtual Vec4 VertexFunc(Param & param)
    {
        param.renderPoint = param.point * param.vpMat;
        return param.renderPoint;
    }

    virtual std::uint32_t FragmentFunc(Param & param)
    {
        return param.color.ToRGBA();
    }
};

