#include "file_vrml.h"
#include "../../mesh.h"
#include "../../vertex.h"

#define CHECK_RET(ret)  { if (!(ret)) return false; }

bool FileVRML::LoadMesh(const std::string & fname, Mesh * mesh)
{
    VRML vrml;
    auto string = GetFileString(fname);
    if (Parse(string.c_str(), &vrml))
    {
        for (auto i = 0; i != vrml.coordIdxs.size() / 3; ++i)
        {
            Vertex v0, v1, v2;
            v0.pt = vrml.coords.at(vrml.coordIdxs.at(i * 3));
            v1.pt = vrml.coords.at(vrml.coordIdxs.at(i * 3 + 1));
            v2.pt = vrml.coords.at(vrml.coordIdxs.at(i * 3 + 2));

            v0.color = vrml.colors.at(vrml.colorIdxs.at(i));
            v1.color = vrml.colors.at(vrml.colorIdxs.at(i));
            v2.color = vrml.colors.at(vrml.colorIdxs.at(i));

            v0.normal = vrml.normals.at(vrml.normalIdxs.at(i));
            v1.normal = vrml.normals.at(vrml.normalIdxs.at(i));
            v2.normal = vrml.normals.at(vrml.normalIdxs.at(i));
            mesh->AddVertexs(v0, v1, v2);
        }
        return true;
    }
    return false;
}

bool FileVRML::Parse(const std::string & string, VRML * output)
{
    size_t pos = 0;
    std::vector<float> numbers;

    pos = string.find("point [", pos);
    CHECK_RET(pos != std::string::npos);
    pos = ParseList(string, pos + 7, &numbers);
    CHECK_RET(pos != string.size());
    for (auto i = 0; i != numbers.size(); i += 3)
    {
        //  x z y => x y z
        output->coords.emplace_back(numbers.at(i    ), 
                                    numbers.at(i + 1), 
                                    -numbers.at(i + 2));
    }

    numbers.clear();
    pos = string.find("color [", pos);
    CHECK_RET(pos != std::string::npos);
    pos = ParseList(string, pos + 7, &numbers);
    CHECK_RET(pos != string.size());
    for (auto i = 0; i != numbers.size(); i += 3)
    {
        output->colors.emplace_back(numbers.at(i    ),
                                    numbers.at(i + 1),
                                    numbers.at(i + 2));
    }

    numbers.clear();
    pos = string.find("colorIndex [", pos);
    CHECK_RET(pos != std::string::npos);
    pos = ParseList(string, pos + 12, &numbers);
    CHECK_RET(pos != string.size());
    for (auto i = 0; i != numbers.size(); ++i)
    {
        output->colorIdxs.push_back((size_t)numbers.at(i));
    }

    numbers.clear();
    pos = string.find("coordIndex [", pos);
    CHECK_RET(pos != std::string::npos);
    pos = ParseList(string, pos + 12, &numbers);
    CHECK_RET(pos != string.size());
    for (auto i = 0; i != numbers.size(); i += 4)
    {
        output->coordIdxs.push_back((size_t)numbers.at(i    ));
        output->coordIdxs.push_back((size_t)numbers.at(i + 1));
        output->coordIdxs.push_back((size_t)numbers.at(i + 2));
    }

    numbers.clear();
    pos = string.find("vector [", pos);
    CHECK_RET(pos != std::string::npos);
    pos = ParseList(string, pos + 8, &numbers);
    CHECK_RET(pos != string.size());
    for (auto i = 0; i != numbers.size(); i += 3)
    {
        output->normals.emplace_back(numbers.at(i),
                                     numbers.at(i + 1),
                                     -numbers.at(i + 2));
    }

    numbers.clear();
    pos = string.find("normalIndex [", pos);
    CHECK_RET(pos != std::string::npos);
    pos = ParseList(string, pos + 13, &numbers);
    CHECK_RET(pos != string.size());
    for (auto i = 0; i != numbers.size(); ++i)
    {
        output->normalIdxs.push_back((size_t)numbers.at(i));
    }

    return true;
}

size_t FileVRML::SkipSpace(const std::string & string, size_t pos)
{
    for (; string.size() != pos && string.at(pos) <= 32; ++pos)
        ;
    return pos;
}

size_t FileVRML::ParseList(const std::string & string, size_t pos, std::vector<float>* output)
{
    size_t index = 0;
    char number[32] = { 0 };
    pos = SkipSpace(string, pos);
    while (string.size() != pos && string.at(pos) != ']')
    {
        switch (string.at(pos))
        {
        case ',':
        case ' ':
            {
                output->push_back(std::strtof(number, nullptr));
                pos = SkipSpace(string, pos + 1);
                memset(number, 0, 32);
                index = 0;
            }
            break;
        case '\n':
            {
                pos = SkipSpace(string, pos + 1);
            }
            break;
        default:
            {
                number[index++] = string.at(pos++);
            }
            break;
        }
    }
    return pos;
}
