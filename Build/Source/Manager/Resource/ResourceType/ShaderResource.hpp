#pragma once
#include "../Resource.hpp"

namespace Engine5
{
    class ShaderResource : public Resource
    {
    public:
        explicit ShaderResource(const std::wstring& path);
        ~ShaderResource();

    private:
    };
}