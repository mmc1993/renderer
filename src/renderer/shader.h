#pragma once

#include "../base.h"
#include "../math/vec4.h"
#include "../math/matrix.h"
#include "../math/number.h"

class Shader {
public:
    struct Param {
        Matrix4x4 vpMat;            //  ��ͼ * ͶӰ ����
        Vec4 lightPoint;            //  �ƹ�����
        Vec4 cameraPoint;           //  �������
        Vec4 ambientLight;          //  ������
        Vec4 directLight;           //  �����
        Vec4 color;                 //  ��ɫ
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
        return RGB( (int)(param.color.x * 255.0f), 
                    (int)(param.color.y * 255.0f), 
                    (int)(param.color.z * 255.0f) );
    }
};

