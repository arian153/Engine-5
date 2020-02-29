#include "PrimitiveRenderer.hpp"
#include "../Renderer/RendererCommon.hpp"
#include "../Shader/ColorShaderCommon.hpp"

namespace Engine5
{
    PrimitiveRenderer::PrimitiveRenderer(RendererCommon* dx11_api)
        : m_view_matrix(), m_world_matrix(), m_proj_matrix(), m_renderer(dx11_api)
    {
    }

    PrimitiveRenderer::~PrimitiveRenderer()
    {
    }

    void PrimitiveRenderer::DrawPrimitive(Primitive* primitive, eRenderingMode mode, Color color)
    {
        primitive->DrawPrimitive(this, mode, color);
    }

    void PrimitiveRenderer::DrawPrimitive(const Primitive& primitive, eRenderingMode mode, Color color)
    {
        primitive.DrawPrimitive(this, mode, color);
    }

    void PrimitiveRenderer::DrawPoint(const Vector3& point, Color color)
    {
        I32 index = (I32)m_dot_vertices.size();
        I32 count = index + 1;
        m_dot_vertices.reserve(static_cast<size_t>(count));
        m_dot_vertices.emplace_back(point, color);
        m_dot_indices.push_back(index);
    }

    void PrimitiveRenderer::DrawSegment(const Vector3& start, const Vector3& end, Color color)
    {
        I32 index = (I32)m_line_vertices.size();
        I32 count = index + 2;
        m_line_vertices.reserve(static_cast<size_t>(count));
        m_line_vertices.emplace_back(start, color);
        m_line_vertices.emplace_back(end, color);
        PushLineIndices(index, index + 1);
    }

    void PrimitiveRenderer::DrawTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, eRenderingMode mode, Color color)
    {
        I32 index = static_cast<I32>(VerticesSize(mode));
        I32 count = 3;
        ReserveVertices(count, mode);
        PushVertex(p0, mode, color);
        PushVertex(p1, mode, color);
        PushVertex(p2, mode, color);
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < count; ++i)
            {
                PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            for (int i = 0; i < count; ++i)
            {
                I32 j = i + 1 < count ? i + 1 : 0;
                PushLineIndices(index + i, index + j);
            }
        }
        else if (mode == eRenderingMode::Face)
        {
            PushFaceIndices(index + 0, index + 1, index + 2);
        }
    }

    void PrimitiveRenderer::DrawTetrahedron(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, eRenderingMode mode, Color color)
    {
        I32 index = static_cast<I32>(VerticesSize(mode));
        I32 count = 4;
        ReserveVertices(count, mode);
        PushVertex(p0, mode, color);
        PushVertex(p1, mode, color);
        PushVertex(p2, mode, color);
        PushVertex(p3, mode, color);
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < count; ++i)
            {
                PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            PushLineIndices(index, index + 1);
            PushLineIndices(index, index + 2);
            PushLineIndices(index, index + 3);
            PushLineIndices(index + 1, index + 2);
            PushLineIndices(index + 2, index + 3);
            PushLineIndices(index + 3, index + 1);
        }
        else if (mode == eRenderingMode::Face)
        {
            PushFaceIndices(index, index + 1, index + 2);
            PushFaceIndices(index, index + 2, index + 3);
            PushFaceIndices(index, index + 3, index + 1);
            PushFaceIndices(index + 1, index + 2, index + 3);
        }
    }

    void PrimitiveRenderer::Initialize(ColorShaderCommon* color_shader)
    {
        m_color_shader = color_shader;
        m_position     = Vector3(0.0f, 0.0f, -60.0f);
        m_rotation     = Quaternion();
        UpdatePrimitiveRendererCamera();
        UpdateProjectionMatrix();
        m_device_context = m_renderer->GetDeviceContext();
        m_dot_buffer     = new BufferCommon();
        m_line_buffer    = new BufferCommon();
        m_face_buffer    = new BufferCommon();
    }

    void PrimitiveRenderer::Update(Real dt)
    {
        UpdatePrimitiveRendererCamera();
        //dot
        m_dot_buffer->BuildBuffer(m_renderer, m_dot_vertices, m_dot_indices);
        UpdateDotBuffer(dt);
        //line
        m_line_buffer->BuildBuffer(m_renderer, m_line_vertices, m_line_indices);
        UpdateLineBuffer(dt);
        //face
        m_face_buffer->BuildBuffer(m_renderer, m_face_vertices, m_face_indices);
        UpdateTriangleBuffer(dt);
        Clear();
    }

    void PrimitiveRenderer::Shutdown()
    {
        Clear();
        if (m_dot_buffer != nullptr)
        {
            m_dot_buffer->Shutdown();
            delete m_dot_buffer;
            m_dot_buffer = nullptr;
        }
        if (m_line_buffer != nullptr)
        {
            m_line_buffer->Shutdown();
            delete m_line_buffer;
            m_line_buffer = nullptr;
        }
        if (m_face_buffer != nullptr)
        {
            m_face_buffer->Shutdown();
            delete m_face_buffer;
            m_face_buffer = nullptr;
        }
    }

    void PrimitiveRenderer::UpdatePrimitiveRendererCamera()
    {
        DirectX::XMVECTOR up_vector       = Converter::ToXMVector(Math::Vector3::Y_AXIS);
        DirectX::XMVECTOR pos_vector      = Converter::ToXMVector(m_position);
        DirectX::XMVECTOR look_vector     = Converter::ToXMVector(Math::Vector3::Z_AXIS);
        DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationQuaternion(Converter::ToXMVector(m_rotation));
        // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
        look_vector = DirectX::XMVector3TransformCoord(look_vector, rotation_matrix);
        up_vector   = DirectX::XMVector3TransformCoord(up_vector, rotation_matrix);
        // Translate the rotated camera position to the location of the viewer.
        look_vector = DirectX::XMVectorAdd(pos_vector, look_vector);
        // Finally create the view matrix from the three updated vectors.
        m_view_matrix = Converter::ToMatrix44(DirectX::XMMatrixLookAtLH(pos_vector, look_vector, up_vector));
    }

    void PrimitiveRenderer::Clear()
    {
        m_dot_vertices.clear();
        m_dot_indices.clear();
        m_line_vertices.clear();
        m_line_indices.clear();
        m_face_vertices.clear();
        m_face_indices.clear();
    }

    void PrimitiveRenderer::UpdateDotBuffer(Real dt) const
    {
        if (m_dot_vertices.empty() == false)
        {
            // Set vertex buffer stride and offset.
            unsigned int stride = sizeof(ColorVertex);
            unsigned int offset = 0;
            // Set the vertex buffer to active in the input assembler so it can be rendered.
            m_device_context->IASetVertexBuffers(0, 1, &m_dot_buffer->m_vertex_buffer, &stride, &offset);
            // Set the index buffer to active in the input assembler so it can be rendered.
            m_device_context->IASetIndexBuffer(m_dot_buffer->m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
            // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
            m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
            m_color_shader->Render(static_cast<U32>(m_dot_indices.size()), m_world_matrix, m_view_matrix, m_proj_matrix);
        }
    }

    void PrimitiveRenderer::UpdateLineBuffer(Real dt) const
    {
        if (m_line_vertices.empty() == false)
        {
            // Set vertex buffer stride and offset.
            unsigned int stride = sizeof(ColorVertex);
            unsigned int offset = 0;
            // Set the vertex buffer to active in the input assembler so it can be rendered.
            m_device_context->IASetVertexBuffers(0, 1, &m_line_buffer->m_vertex_buffer, &stride, &offset);
            // Set the index buffer to active in the input assembler so it can be rendered.
            m_device_context->IASetIndexBuffer(m_line_buffer->m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
            // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
            m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
            m_color_shader->Render(static_cast<U32>(m_line_indices.size()), m_world_matrix, m_view_matrix, m_proj_matrix);
        }
    }

    void PrimitiveRenderer::UpdateTriangleBuffer(Real dt) const
    {
        if (m_face_vertices.empty() == false)
        {
            // Set vertex buffer stride and offset.
            unsigned int stride = sizeof(ColorVertex);
            unsigned int offset = 0;
            // Set the vertex buffer to active in the input assembler so it can be rendered.
            m_device_context->IASetVertexBuffers(0, 1, &m_face_buffer->m_vertex_buffer, &stride, &offset);
            // Set the index buffer to active in the input assembler so it can be rendered.
            m_device_context->IASetIndexBuffer(m_face_buffer->m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
            // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
            m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            m_color_shader->Render(static_cast<U32>(m_face_indices.size()), m_world_matrix, m_view_matrix, m_proj_matrix);
        }
    }

    void PrimitiveRenderer::UpdateProjectionMatrix()
    {
        m_proj_matrix = Converter::ToMatrix44(m_renderer->GetProjectionMatrix());
    }

    void PrimitiveRenderer::SetRendererCameraPosition(const Vector3& pos)
    {
        m_position = pos;
        this->UpdatePrimitiveRendererCamera();
    }

    void PrimitiveRenderer::SetRendererCameraRotation(const Quaternion& rot)
    {
        m_rotation = rot;
        this->UpdatePrimitiveRendererCamera();
    }

    void PrimitiveRenderer::PushVertex(const Vector3& pos, eRenderingMode mode, const Color& color)
    {
        switch (mode)
        {
        case Engine5::eRenderingMode::Dot:
            m_dot_vertices.emplace_back(pos, color);
            break;
        case Engine5::eRenderingMode::Line:
            m_line_vertices.emplace_back(pos, color);
            break;
        case Engine5::eRenderingMode::Face:
            m_face_vertices.emplace_back(pos, color);
            break;
        default:
            break;
        }
    }

    void PrimitiveRenderer::PushIndex(I32 index, eRenderingMode mode)
    {
        switch (mode)
        {
        case Engine5::eRenderingMode::Dot:
            m_dot_indices.push_back(index);
            break;
        case Engine5::eRenderingMode::Line:
            m_line_indices.push_back(index);
            break;
        case Engine5::eRenderingMode::Face:
            m_face_indices.push_back(index);
            break;
        default:
            break;
        }
    }

    void PrimitiveRenderer::PushLineIndices(I32 a, I32 b)
    {
        m_line_indices.push_back(a);
        m_line_indices.push_back(b);
    }

    void PrimitiveRenderer::PushFaceIndices(I32 a, I32 b, I32 c)
    {
        m_face_indices.push_back(a);
        m_face_indices.push_back(b);
        m_face_indices.push_back(c);
    }

    void PrimitiveRenderer::ReserveVertices(size_t adding_count, eRenderingMode mode)
    {
        switch (mode)
        {
        case Engine5::eRenderingMode::Dot:
            m_dot_vertices.reserve(m_dot_vertices.size() + adding_count);
            break;
        case Engine5::eRenderingMode::Line:
            m_line_vertices.reserve(m_line_vertices.size() + adding_count);
            break;
        case Engine5::eRenderingMode::Face:
            m_face_vertices.reserve(m_face_vertices.size() + adding_count);
            break;
        default:
            break;
        }
    }

    void PrimitiveRenderer::ReserveIndices(size_t adding_count, eRenderingMode mode)
    {
        switch (mode)
        {
        case Engine5::eRenderingMode::Dot:
            m_dot_indices.reserve(m_dot_indices.size() + adding_count);
            break;
        case Engine5::eRenderingMode::Line:
            m_line_indices.reserve(m_line_indices.size() + adding_count);
            break;
        case Engine5::eRenderingMode::Face:
            m_face_indices.reserve(m_face_indices.size() + adding_count);
            break;
        default:
            break;
        }
    }

    size_t PrimitiveRenderer::VerticesSize(eRenderingMode mode) const
    {
        switch (mode)
        {
        case Engine5::eRenderingMode::Dot:
            return m_dot_vertices.size();
        case Engine5::eRenderingMode::Line:
            return m_line_vertices.size();
        case Engine5::eRenderingMode::Face:
            return m_face_vertices.size();
        default:
            break;
        }
        return 0;
    }

    size_t PrimitiveRenderer::IndicesSize(eRenderingMode mode) const
    {
        switch (mode)
        {
        case Engine5::eRenderingMode::Dot:
            return m_dot_indices.size();
        case Engine5::eRenderingMode::Line:
            return m_line_indices.size();
        case Engine5::eRenderingMode::Face:
            return m_face_indices.size();
        default:
            break;
        }
        return 0;
    }
}
