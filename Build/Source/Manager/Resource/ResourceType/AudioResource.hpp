#pragma once
#include "../Resource.hpp"

namespace Engine5
{
    class AudioResource : public Resource
    {
    public:
        explicit AudioResource(const std::wstring& path);
        ~AudioResource();

    private:
    };
}