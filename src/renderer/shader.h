#pragma once

#include "../base.h"
#include "../math/vec4.h"
#include "../math/matrix.h"
#include "../math/number.h"

class Shader {
public:
    struct Param {
        Matrix4x4 vpMat;            //  视图 * 投影 矩阵
        Vec4 lightPoint;            //  灯光坐标
        Vec4 cameraPoint;           //  相机坐标
        Vec4 ambientLight;          //  环境光
        Vec4 directLight;           //  方向光
        Vec4 color;                 //  颜色
        Vec4 point;                 //  坐标
        Vec4 normal;                //  法线
        float u, v;                 //  uv

        //  输出
        Vec4 renderPoint;           //  渲染坐标
        Vec4 renderColor;           //  渲染颜色
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

