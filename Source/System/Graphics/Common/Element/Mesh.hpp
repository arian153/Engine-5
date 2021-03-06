#pragma once
#include "../../../Math/Algebra/Matrix44.hpp"
#include "../Shader/ShaderType.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../DataType/Color.hpp"
#include "../Texture/TextureArrayCommon.hpp"
#include "../Light/LightDef.hpp"

namespace Engine5
{
    class Color;
    class TextureArrayCommon;
    class Transform;
    class TextureCommon;
    class ShaderManagerCommon;
    class RendererCommon;
    class MeshData;
    class MeshBufferCommon;

    class Mesh
    {
    public:
        Mesh();
        ~Mesh();
        void Initialize();
        void Shutdown();

        void RenderBuffer() const;
        void BuildBuffer();
        void SetMeshData(MeshData* mesh_data);
        void SetTexture(TextureCommon* texture);
        void AddTexture(TextureCommon* texture);
        void ClearTexture();
        void RemoveTexture(TextureCommon* texture);
        void SetTransform(Transform* transform);
        void SetShaderType(eShaderType type);
        void SetColor(const Color& color);
        void SetRenderer(RendererCommon* renderer);

        Matrix44            GetModelMatrix() const;
        TextureCommon*      GetTexture() const;
        TextureArrayCommon* GetTextureArray();
        eShaderType         GetShaderType() const;
        U32                 GetIndexCount() const;
        Color               GetColor() const;
        eLightingMethod     GetLightingMethod() const;

    private:
        friend class MeshComponent;

    private:
        ShaderManagerCommon* m_shader_manager = nullptr;
        RendererCommon*      m_renderer       = nullptr;
        MeshBufferCommon*    m_buffer         = nullptr;
        MeshData*            m_mesh_data      = nullptr;
        TextureArrayCommon   m_texture_array;
        Transform*           m_transform = nullptr;
        MeshComponent*       m_component = nullptr;
        Color                m_color;
        eShaderType          m_type     = eShaderType::Invalid;
        U32                  m_stride   = 0;
        eLightingMethod      m_lighting = eLightingMethod::None;
    };
}
