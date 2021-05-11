#pragma once
#include "../../../GraphicsAPI.hpp"
#include "../../../../Core/Utility/CoreDef.hpp"

#include IncludeColorShaderAPI

namespace Engine5
{
    class MatrixData;
    class ShaderManagerCommon;
    class ShaderResource;
    class Matrix44;

    class ColorShaderCommon : public ColorShaderAPI
    {
    public:
        explicit ColorShaderCommon(ShaderManagerCommon* shader_manager);
        ~ColorShaderCommon();

        void SetShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 indices_count, const MatrixData& mvp_data) const;
        void Shutdown();

    private:
        ShaderManagerCommon*  m_shader_manager  = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };
}