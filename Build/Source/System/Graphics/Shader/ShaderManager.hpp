#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../API/GraphicsAPI.hpp"
#include IncludeShaderManagerAPI

namespace Engine5
{
    class LightShaderCommon;
    class DirectionalLight;
    class Camera;
    class TextureCommon;
    class TextureShaderCommon;
    class MatrixData;
    class ResourceManager;
    class RendererCommon;
    class ColorShaderCommon;
    class DeferredShaderCommon;
    class Color;

    class ShaderManager : public ShaderManagerAPI
    {
    public:
        ShaderManager();
        ~ShaderManager();
        void Initialize(RendererCommon* renderer, ResourceManager* resource_manager);
        void Shutdown();

        void RenderColorShader(U32 indices_count, const MatrixData& mvp_data) const;
        void RenderTextureShader(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const;
        void RenderLightShader(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, Camera* camera, const Color& color, const DirectionalLight& light) const;
        void RenderDeferredShader(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const;

        ColorShaderCommon*    GetColorShader() const;
        TextureShaderCommon*  GetTextureShader() const;
        LightShaderCommon*    GetLightShader() const;
        DeferredShaderCommon* GetDeferredShader() const;

    private:
        //shader list
        ColorShaderCommon*    m_color_shader     = nullptr;
        TextureShaderCommon*  m_texture_shader   = nullptr;
        LightShaderCommon*    m_light_shader     = nullptr;
        DeferredShaderCommon* m_deferred_shader  = nullptr;
        ResourceManager*      m_resource_manager = nullptr;
    };
}
