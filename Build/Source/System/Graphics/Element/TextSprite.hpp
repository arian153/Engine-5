#pragma once
#include <string>
#include "../../Math/Algebra/Matrix44.hpp"
#include "../Texture/TextureCommon.hpp"
#include "../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class Transform;
    class Color;
    class TextBufferCommon;

    class TextSprite
    {
    public:
        TextSprite();
        ~TextSprite();

        void Initialize(RendererCommon* renderer);
        void Render() const;
        void Shutdown();

        void SetText(const std::string& text) const;
        void SetText(const std::wstring& text) const;
        void SetFont(const std::wstring& font) const;
        void SetColor(const Color& color) const;
        void SetTransform(Transform* transform);

        Matrix44       GetModelMatrix() const;
        TextureCommon* GetTexture() const;
        Color          GetColor() const;
        U32            GetIndexCount() const;

    private:
        TextBufferCommon* m_text      = nullptr;
        TextureCommon*    m_texture   = nullptr;
        Transform*        m_transform = nullptr;
    };
}
