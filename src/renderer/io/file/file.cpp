#include "file.h"
#include "file_vrml.h"
#include <string_view>

bool File::LoadMesh(const std::string & fname, Mesh * mesh)
{
    if (CheckSuffix(fname, ".wrl"))
    {
        FileVRML fvrml;
        return fvrml.LoadMesh(fname, mesh);
    }
    return false;
}

std::string File::GetFileString(const std::string & fname)
{
    std::ifstream ifile(fname);
    if (ifile)
    {
        return std::move(GetFileString(ifile));
    }
    return std::string();
}

size_t File::GetFileSize(const std::string & fname)
{
    std::ifstream ifile(fname);
    if (ifile)
    {
        return GetFileSize(ifile);
    }
    return (size_t)~0;
}

size_t File::GetFileSize(std::ifstream & ifile)
{
    auto tell = ifile.tellg();
    ifile.seekg(0, std::ios::end);
    auto size = ifile.tellg();
    ifile.seekg(tell, std::ios::beg);
    return (size_t)size;
}

std::string File::GetFileString(std::ifstream & ifile)
{
    auto size = GetFileSize(ifile);
    std::string buffer(size, '\0');
    ifile.read(&buffer[0], size);
    return std::move(buffer);
}

bool File::CheckSuffix(const std::string & fname, const std::string & suffix)
{
    if (suffix.size() > fname.size())
    {
        return false;
    }
    auto pos = fname.size() - suffix.size();
    std::string_view sview(&fname[pos]);
    return sview == suffix;
}
