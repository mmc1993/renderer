#include "file.h"
#include <string_view>

File::File()
{
}

File::~File()
{
}

bool File::LoadMesh(const std::string & fname, Mesh * mesh)
{
    if (!CheckSuffix(fname, ".json"))
    {
        return false;
    }

    return true;
}

std::string File::LoadFile(const std::string & fname) const
{
    std::ifstream ifile(fname);
    if (ifile)
    {
        return std::move(LoadFile(ifile));
    }
    return std::string();
}

size_t File::GetFileSize(const std::string & fname) const
{
    std::ifstream ifile(fname);
    if (ifile)
    {
        return GetFileSize(ifile);
    }
    return (size_t)~0;
}

size_t File::GetFileSize(std::ifstream & ifile) const
{
    auto tell = ifile.tellg();
    ifile.seekg(0, std::ios::end);
    auto size = ifile.tellg();
    ifile.seekg(tell, std::ios::beg);
    return (size_t)size;
}

std::string File::LoadFile(std::ifstream & ifile) const
{
    auto size = GetFileSize(ifile);
    std::string buffer('\0', size);
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
