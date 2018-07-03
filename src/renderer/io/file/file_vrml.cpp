#include "file_vrml.h"
#include "../../mesh.h"
#include "../../vertex.h"

#define CHECK_RET(ret)  { if (!ret) return false; }

bool FileVRML::LoadMesh(const std::string & fname, Mesh * mesh)
{
    VRML vrml;
    auto string = LoadFile(fname);
    if (Parse(string.c_str(), &vrml))
    {
        //  TODO ผฬะ๘ะด
        //vrml.
    }
    return false;
}

bool FileVRML::Parse(const char * string, VRML * output)
{
    std::vector<float> numbers;
    string = SkipField(string, "point [");
    string = ParseList(string, &numbers);
    CHECK_RET(*string != '\0');
    //  x z y => x y z
    for (auto i = 0; i != numbers.size() / 3; i += 3)
    {
        output->coords.emplace_back(numbers.at(i    ), 
                                    numbers.at(i + 2), 
                                    numbers.at(i + 1));
    }

    numbers.clear();
    string = SkipField(string, "color [");
    string = ParseList(string, &numbers);
    CHECK_RET(*string != '\0');
    for (auto i = 0; i != numbers.size() / 3; i += 3)
    {
        output->colors.emplace_back(numbers.at(i    ),
                                    numbers.at(i + 1),
                                    numbers.at(i + 2));
    }

    numbers.clear();
    string = SkipField(string, "colorIndex [");
    string = ParseList(string, &numbers);
    CHECK_RET(*string != '\0');
    for (auto i = 0; i != numbers.size(); ++i)
    {
        output->colorIdxs.push_back((size_t)numbers.at(i));
    }

    numbers.clear();
    string = SkipField(string, "coordIndex [");
    string = ParseList(string, &numbers);
    CHECK_RET(*string != '\0');
    for (auto i = 0; i != numbers.size(); ++i)
    {
        output->coordIdxs.push_back((size_t)numbers.at(i));
    }

    numbers.clear();
    string = SkipField(string, "vector [");
    string = ParseList(string, &numbers);
    CHECK_RET(*string != '\0');
    for (auto i = 0; i != numbers.size() / 3; i += 3)
    {
        output->normals.emplace_back(numbers.at(i),
                                     numbers.at(i + 1),
                                     numbers.at(i + 2));
    }

    numbers.clear();
    string = SkipField(string, "normalIndex [");
    string = ParseList(string, &numbers);
    CHECK_RET(*string != '\0');
    for (auto i = 0; i != numbers.size(); ++i)
    {
        output->normalIdxs.push_back((size_t)numbers.at(i));
    }

    return true;
}

const char * FileVRML::SkipSpace(const char * string)
{
    for (; *string != '\0' && *string <= 32; ++string)
        ;
    return string;
}

const char * FileVRML::SkipField(const char * string, const char * field)
{
    assert(field != nullptr);
    for (; *string != '\0' && std::strcmp(string, field) != 0; ++string)
        ;
    return string;
}

const char * FileVRML::ParseList(const char * string, std::vector<float>* output)
{
    size_t index = 0;
    char number[32] = { 0 };
    string = SkipSpace(string);
    while (*string != ']')
    {
        number[index++] = *string;
        if (*string == ',')
        {
            output->push_back(std::strtof(number, nullptr));
            string = SkipSpace(string + 1);
            memset(number, 0, 32);
            index = 0;
        }
    }
    return string;
}
