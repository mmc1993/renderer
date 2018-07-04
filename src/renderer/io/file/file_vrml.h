#pragma once

#include "file.h"
#include "../../mesh.h"
#include "../../vec4.h"
#include "../../color.h"

class FileVRML: public File {
public:
    struct VRML {
        std::vector<Vec4> coords;
        std::vector<Vec4> normals;
        std::vector<Color> colors;
        std::vector<size_t> coordIdxs;
        std::vector<size_t> colorIdxs;
        std::vector<size_t> normalIdxs;
    };

    bool LoadMesh(const std::string & fname, Mesh * mesh);

private:
    bool Parse(const std::string & string, VRML * output);
    size_t SkipSpace(const std::string & string, size_t pos);
    size_t ParseList(const std::string & string, size_t pos, std::vector<float> * output);
};