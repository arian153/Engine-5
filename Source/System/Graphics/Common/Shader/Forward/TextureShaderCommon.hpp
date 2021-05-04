#pragma once
#include "../../../GraphicsAPI.hpp"
#include "../../../../Core/Utility/CoreDef.hpp"

#include IncludeTextureShaderAPI

namespace Engine5
{
    class MatrixData;
    class TextureCommon;
    class Color;
    class ShaderManagerCommon;
    class ShaderResource;
    class Matrix44;

    class TextureShaderCommon : public TextureShaderAPI
    {
    public:
        explicit TextureShaderCommon(ShaderManagerCommon* shader_manager);
        ~TextureShaderCommon();

        void SetShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const;
        void Shutdown();

    private:
        ShaderManagerCommon*  m_shader_manager  = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };
}
