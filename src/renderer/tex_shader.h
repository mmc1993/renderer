#pragma once

#include "shader.h"
#include "texture.h"

class TexShader : public Shader {
public:
    virtual Vec4 VertexFunc(Param & param) override
    {
        return Shader::VertexFunc(param);
    }

    virtual std::uint32_t FragmentFunc(Param & param) override
    {
        return Shader::FragmentFunc(param);
        //auto src = _texture[0]->GetColor(param.u, param.v);

        ////  金属度
        //auto metal = _texture[1]->GetColor(param.u, param.v);
        //src = src + metal;
        //src.Normal();

        ////  环境光
        //auto color = src * param.ambientLight;

        ////  漫反射
        //auto direct = (param.lightPoint - param.point).GetNormal();
        //auto dot = std::max(0.0f, direct.Dot(param.normal));
        //color.x += (src.x - color.x) * param.directLight.x * dot;
        //color.y += (src.y - color.y) * param.directLight.y * dot;
        //color.z += (src.z - color.z) * param.directLight.z * dot;

        //return RGB(static_cast<std::uint32_t>(color.x * 255.0f), 
        //           static_cast<std::uint32_t>(color.y * 255.0f), 
        //           static_cast<std::uint32_t>(color.z * 255.0f));
    }

    void BindTexture(size_t i, Texture * texture)
    {
        _texture[i] = texture;
    }
    
private:
    Texture *_texture[4];    //  纹理贴图, 法线贴图, 反射贴图, 高光贴图
};