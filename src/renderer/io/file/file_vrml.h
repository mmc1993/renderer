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
    bool Parse(const char * string, VRML * output);
    const char * SkipSpace(const char * string);
    const char * SkipField(const char * string, const char * field);
    const char * ParseList(const char * string, std::vector<float> * output);
};