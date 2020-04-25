#pragma once
#include "../Resource.hpp"

namespace Engine5
{
    class TextResource final : public Resource
    {
    public:
        explicit TextResource(const std::wstring& path);
        ~TextResource();

        void Initialize() override;
        void Shutdown() override;

        std::wstring* GetText() const;

    private:
        std::wstring* m_text = nullptr;
    };
}
