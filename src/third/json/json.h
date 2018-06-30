#pragma once

extern "C" {
#include "cJSON.h"
}

#include <tuple>
#include <string>
#include <cassert>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <functional>

class Json {
public:
    enum ValueType {
        kBOOLEAN,
        kNUMBER,
        kSTRING,
        kOBJECT,
        kARRAY,
        kNULL,
    };

    class Value {
    public:
        Value(): _cjson(nullptr)
        { }

        Value(cJSON * cjson): _cjson(cjson)
        { }

        ValueType Type() const
        {
            switch (_cjson->type)
            {
            case cJSON_Number:
                return kNUMBER;
            case cJSON_String:
                return kSTRING;
            case cJSON_Object:
                return kOBJECT;
            case cJSON_Array:
                return kARRAY;
            case cJSON_True:
            case cJSON_False:
                return kBOOLEAN;
            }
            return kNULL;
        }

        int ToInt() const
        {
            return static_cast<int>(_cjson->valuedouble);
        }

        float ToFloat() const
        {
            return static_cast<float>(_cjson->valuedouble);
        }

        double ToDouble() const
        {
            return _cjson->valuedouble;
        }

        bool ToBool() const
        {
            return _cjson->valueint != 0;
        }

        std::string ToString() const
        {
            return _cjson->valuestring;
        }

        const char * Key() const
        {
            return _cjson->string;
        }

        void ForList(const std::function<void(const size_t idx, Value &)> & func)
        {
            for (auto i = 0; i != cJSON_GetArraySize(_cjson); ++i)
            {
                Value value(cJSON_GetArrayItem(_cjson, i));
                func(i, value);
            }
        }

        void ForHash(const std::function<void(const char * key, Value &)> & func)
        {
            for (auto i = 0; i != cJSON_GetArraySize(_cjson); ++i)
            {
                Value value(cJSON_GetArrayItem(_cjson, i));
                func(value.Key(), value);
            }
        }

    private:
        cJSON * _cjson;

        friend class Json;
    };

    template <class T>
    struct IsCharArr: public std::false_type {};
    template <>
    struct IsCharArr<char *>: public std::true_type {};
    template <int N>
    struct IsCharArr<char[N]>: public std::true_type {};

    template <class T>
    struct IsString: public std::false_type {};
    template <>
    struct IsString<std::string>: public std::true_type {};

    template <class T>
    struct IsValue: public std::false_type {};
    template <>
    struct IsValue<Value>: public std::true_type {};

    template <class T>
    struct IsInteger: public std::false_type { };
    template <>
    struct IsInteger<std::int8_t>: public std::true_type {};
    template <>
    struct IsInteger<std::int16_t>: public std::true_type {};
    template <>
    struct IsInteger<std::int32_t>: public std::true_type {};
    template <>
    struct IsInteger<std::int64_t>: public std::true_type {};
    template <>
    struct IsInteger<std::uint8_t>: public std::true_type {};
    template <>
    struct IsInteger<std::uint16_t>: public std::true_type {};
    template <>
    struct IsInteger<std::uint32_t>: public std::true_type {};
    template <>
    struct IsInteger<std::uint64_t>: public std::true_type {};

    static Json FromFile(const std::string & fname)
    {
        std::ifstream ifile(fname);
        std::string buffer;
        std::copy(std::istream_iterator<char>(ifile), 
                std::istream_iterator<char>(), 
                std::back_inserter(buffer));
        return std::move(FromString(buffer));
    }

    static Json FromString(const std::string & string)
    {
        Json json;
        json._cjson = cJSON_Parse(string.c_str());
        return std::move(json);
    }

    Json(const std::string & string)
    {
        _cjson = cJSON_Parse(string.c_str());
    }

    Json(): _cjson(nullptr)
    { }

    Json(const Json & json): _cjson(nullptr)
    {
        *this = json;
    }

    Json(Json && json): _cjson(nullptr)
    {
        *this = std::move(json);
    }

    ~Json()
    {
        RemoveThis();
    }

    Json & operator =(const Json & json)
    {
        RemoveThis();
        _cjson = CloneThis(json._cjson);
    }

    Json & operator =(Json && json)
    {
        RemoveThis();
        _cjson = json._cjson;
        json._cjson = nullptr;
        return *this;
    }

    std::string ToString()
    {
        return std::string(cJSON_Print(_cjson));
    }

    template <class ...Keys>
    Value Get(const Keys & ...keys)
    {
        return Value(GetImpl(_cjson, keys...));
    }

    //template <class ...Keys>
    //void Del(std::int8_t idx, const Keys & ...keys)
    //{
    //    DelImpl(idx, keys...);
    //}

    //template <class ...Keys>
    //void Del(std::int16_t idx, const Keys & ...keys)
    //{
    //    DelImpl(idx, keys...);
    //}

    //template <class ...Keys>
    //void Del(std::int32_t idx, const Keys & ...keys)
    //{
    //    DelImpl(idx, keys...);
    //}

    //template <class ...Keys>
    //void Del(std::int64_t idx, const Keys & ...keys)
    //{
    //    DelImpl(idx, keys...);
    //}

    //template <class ...Keys>
    //void Del(const char * key, const Keys & ...keys)
    //{
    //    DelImpl(key, keys...);
    //}

    //template <class ...Keys>
    //void Del(const std::string & key, const Keys & ...keys)
    //{
    //    DelImpl(key, keys...);
    //}

    template <class ...Keys>
    bool Set(std::int8_t val, const Keys & ...keys)
    {
        return SetImpl(_cjson, cJSON_CreateNumber(static_cast<double>(val)), keys...);
    }

    template <class ...Keys>
    bool Set(std::int16_t val, const Keys & ...keys)
    {
        return SetImpl(_cjson, cJSON_CreateNumber(static_cast<double>(val)), keys...);
    }

    template <class ...Keys>
    bool Set(std::int32_t val, const Keys & ...keys)
    {
        return SetImpl(_cjson, cJSON_CreateNumber(static_cast<double>(val)), keys...);
    }

    template <class ...Keys>
    bool Set(std::int64_t val, const Keys & ...keys)
    {
        return SetImpl(_cjson, cJSON_CreateNumber(static_cast<double>(val)), keys...);
    }

    template <class ...Keys>
    bool Set(float val, const Keys & ...keys)
    {
        return SetImpl(_cjson, cJSON_CreateNumber(static_cast<double>(val)), keys...);
    }

    template <class ...Keys>
    bool Set(double val, const Keys & ...keys)
    {
        return SetImpl(_cjson, cJSON_CreateNumber(static_cast<double>(val)), keys...);
    }

    template <class ...Keys>
    bool Set(const char * val, const Keys & ...keys)
    {
        return SetImpl(_cjson, cJSON_CreateString(val), keys...);
    }

    template <class ...Keys>
    bool Set(const std::string & val, const Keys & ...keys)
    {
        return SetImpl(_cjson, cJSON_CreateString(val.c_str()), keys...);
    }

    template <class ...Keys>
    bool SetList(const Keys & ...keys)
    {
        return SetImpl(_cjson, cJSON_CreateArray(), keys...);
    }

    template <class ...Keys>
    bool SetHash(const Keys & ...keys)
    {
        return SetImpl(_cjson, cJSON_CreateObject(), keys...);
    }

private:
    void RemoveThis()
    {
        if (_cjson != nullptr)
        {
            cJSON_Delete(_cjson);
            _cjson = nullptr;
        }
    }

    cJSON * CloneThis(cJSON * parent)
    {
        cJSON * cjson = nullptr;
        if (cJSON_Number == parent->type)
        {
            cjson = cJSON_CreateNumber(parent->valuedouble);
        }
        else if (cJSON_String == parent->type)
        {
            cjson = cJSON_CreateString(parent->valuestring);
        }
        else if (cJSON_False == parent->type)
        {
            cjson = cJSON_CreateFalse();
        }
        else if (cJSON_True == parent->type)
        {
            cjson = cJSON_CreateTrue();
        }
        else if (cJSON_Array == parent->type)
        {
            cjson = cJSON_CreateArray();
            for (auto ele = parent->child; ele != nullptr; ele = ele->next)
            {
                cJSON_AddItemToArray(cjson, CloneThis(ele));
            }
        }
        else if (cJSON_Object == parent->type)
        {
            cjson = cJSON_CreateObject();
            for (auto ele = parent->child; ele != nullptr; ele = ele->next)
            {
                cJSON_AddItemToObject(cjson, ele->string, CloneThis(ele));
            }
        }
        assert(nullptr != cjson);
        return cjson;
    }

    template <class Key, class ...Keys>
    cJSON * GetImpl(cJSON * cjson, const Key & key, const Keys & ...keys)
    {
        cjson = GetImpl(cjson, key);
        if (cjson != nullptr)
        {
            return GetImpl(cjson, keys...);
        }
        return nullptr;
    }

    template <class Key>
    cJSON * GetImpl(cJSON * cjson, const Key & key)
    {
        if (cjson != nullptr)
        {
            if constexpr (IsCharArr<Key>::value)
            {
                return cJSON_GetObjectItem(cjson, key);
            }
            if constexpr (IsInteger<Key>::value)
            {
                return cJSON_GetArrayItem(cjson, key);
            }
            if constexpr (IsString<Key>::value)
            {
                return cJSON_GetObjectItem(cjson, key.c_str());
            }
            if constexpr (IsValue<Key>::value)
            {
                return key._cjson;
            }
        }
        return nullptr;
    }

    template <class Key1, class Key2, class ...Keys>
    bool SetImpl(cJSON * root, cJSON * val, const Key1 & key1, const Key2 & key2, const Keys & ...keys)
    {
        if constexpr (IsValue<Key1>::value)
        {
            return SetImpl(key1._cjson, val, key2, keys...);
        }
        if constexpr (!IsValue<Key1>::value)
        {
            return SetImpl(GetImpl(root, key1), val, key2, keys...);
        }
    }

    template <class Key1, class Key2>
    bool SetImpl(cJSON * root, cJSON * val, const Key1 & key1, const Key2 & key2)
    {
        root = GetImpl(root, key1);
        if constexpr (IsInteger<Key2>::value)
        {
            if (!cJSON_IsArray(root)) { return false; }
            if (key2 >= cJSON_GetArraySize(root))
            {
                cJSON_AddItemToArray(root, val);
            }
            else
            {
                cJSON_DeleteItemFromArray(root, key2);
                cJSON_InsertItemInArray(root, key2, val);
            }
        }
        if constexpr (IsString<Key2>::value)
        {
            if (!cJSON_IsObject(root)) { return false; }
            cJSON_DeleteItemFromObject(root, key2.c_str());
            cJSON_AddItemToObject(root, key2.c_str(), val);
        }
        if constexpr (IsCharArr<Key2>::value)
        {
            if (!cJSON_IsObject(root)) { return false; }
            cJSON_DeleteItemFromObject(root, key2);
            cJSON_AddItemToObject(root, key2, val);
        }
        return true;
    }

    template <class Key>
    bool SetImpl(cJSON * root, cJSON * val, const Key & key)
    {
        assert(root == _cjson);
        if constexpr (IsInteger<Key>::value)
        {
            if (!cJSON_IsArray(_cjson))
            {
                RemoveThis();
                _cjson = cJSON_CreateArray();
            }
            if (key >= cJSON_GetArraySize(_cjson))
            {
                cJSON_AddItemToArray(_cjson, val);
            }
            else
            {
                cJSON_DeleteItemFromArray(_cjson, key);
                cJSON_InsertItemInArray(_cjson, key, val);
            }
        }
        if constexpr (IsString<Key>::value)
        {
            if (!cJSON_IsObject(_cjson))
            {
                RemoveThis();
                _cjson = cJSON_CreateObject();
            }
            cJSON_DeleteItemFromObject(_cjson, key.c_str());
            cJSON_AddItemToObject(_cjson, key.c_str(), val);
        }
        if constexpr (IsCharArr<Key>::value)
        {
            if (!cJSON_IsObject(_cjson))
            {
                RemoveThis();
                _cjson = cJSON_CreateObject();
            }
            cJSON_DeleteItemFromObject(_cjson, key);
            cJSON_AddItemToObject(_cjson, key, val);
        }
        return true;
    }
    
    //template <class Key, class ...Keys>
    //void DelImpl(Key && key, const Keys & ...keys)
    //{
    //    auto root = GetImpl(_cjson, keys...);
    //    if (root != nullptr)
    //    {
    //        if constexpr (IsString<Key>::value)
    //        {
    //            assert(cJSON_IsObject(cjson));
    //            cJSON_DeleteItemFromObject(root, key.c_str());
    //        }
    //        if constexpr (IsCharArr<Key>::value)
    //        {
    //            assert(cJSON_IsObject(cjson));
    //            cJSON_DeleteItemFromObject(root, key);
    //        }
    //        if constexpr (IsInteger<Key>::value)
    //        {
    //            assert(cJSON_IsArray(cjson));
    //            cJSON_DeleteItemFromArray(root, key);
    //        }
    //    }
    //}

private:
    cJSON * _cjson;
};
