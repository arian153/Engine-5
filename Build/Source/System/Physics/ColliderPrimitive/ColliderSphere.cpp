#include "ColliderSphere.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace Engine5
{
    ColliderSphere::ColliderSphere()
    {
        m_type = ColliderType::Sphere;
    }

    ColliderSphere::~ColliderSphere()
    {
    }

    void ColliderSphere::Initialize()
    {
        MakeUnitPrimitive();
    }

    void ColliderSphere::Shutdown()
    {
    }

    Vector3 ColliderSphere::Support(const Vector3& direction)
    {
        Vector3 local_dir = WorldToLocalVector(direction).Unit();
        Vector3 result    = Radius() * local_dir;
        return LocalToWorldPoint(result);
    }

    bool ColliderSphere::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real    radius = Radius();
        Vector3 l      = -local_ray.position;
        Real    tc     = l.DotProduct(local_ray.direction);
        if (tc < 0.0f)
        {
            return false;
        }
        Real d2             = tc * tc - l.DotProduct(l);
        Real radius_squared = radius * radius;
        if (d2 > radius_squared)
        {
            return false;
        }

        //solve for t1c
        Real t1c  = sqrtf(radius_squared - d2);
        minimum_t = tc - t1c;
        maximum_t = tc + t1c;
        return true;
    }

    Vector3 ColliderSphere::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider.Unit();
    }

    void ColliderSphere::CalculateMassData(Real density)
    {
        m_mass  = density * GetVolume();
        Real it = m_mass * 0.4f * Radius() * Radius();
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it, it, it);
        m_centroid.SetZero();
    }

    Real ColliderSphere::GetVolume()
    {
        if (m_rigid_body != nullptr)
        {
            return 4.0f / 3.0f * Math::PI * m_transformed_radius * m_transformed_radius * m_transformed_radius;
        }
        return 4.0f / 3.0f * Math::PI * m_radius * m_radius * m_radius;
    }

    void ColliderSphere::ScalePrimitiveData(const Vector3& scale)
    {
        m_transformed_radius = m_radius * scale.Length();
    }

    void ColliderSphere::MakeUnitPrimitive()
    {
        m_radius = 0.5f;
        //TODO - get scale from transform 
        Vector3 scale(1.0f, 1.0f, 1.0f);
        ScalePrimitiveData(scale);
    }

    void ColliderSphere::UpdateBoundingVolume()
    {
    }

    void ColliderSphere::DrawPrimitive(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color)
    {
        I32 index       = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count = renderer->SPHERICAL_STACK_COUNT;
        int slice_count = renderer->SPHERICAL_SLICE_COUNT;
        renderer->ReserveVertices(renderer->SPHERICAL_VERTICES_COUNT, mode);
        Vector3    axis_vector      = Vector3::AxisY();
        Vector3    body_position    = GetBodyPosition();
        Quaternion body_orientation = GetBodyOrientation();
        Real       radius           = Radius();

        //top vertex
        Vector3 top_vertex_local_pos = axis_vector;
        top_vertex_local_pos         = top_vertex_local_pos * radius;

        //modify rotation, translation
        top_vertex_local_pos = m_orientation.Rotate(top_vertex_local_pos);
        top_vertex_local_pos += m_position;
        top_vertex_local_pos = body_orientation.Rotate(top_vertex_local_pos);
        top_vertex_local_pos += body_position;

        //bottom vertex
        Vector3 bottom_vertex_local_pos = -axis_vector;
        bottom_vertex_local_pos         = bottom_vertex_local_pos * radius;

        //modify rotation, translation
        bottom_vertex_local_pos = m_orientation.Rotate(bottom_vertex_local_pos);
        bottom_vertex_local_pos += m_position;
        bottom_vertex_local_pos = body_orientation.Rotate(bottom_vertex_local_pos);
        bottom_vertex_local_pos += body_position;
        renderer->PushVertex(top_vertex_local_pos, mode, color);
        Real phi_step   = Math::PI / stack_count;
        Real theta_step = Math::TWO_PI / slice_count;

        // Compute vertices for each stack ring (do not count the poles as rings).
        for (I32 i = 1; i <= stack_count - 1; ++i)
        {
            Real phi = i * phi_step;
            // Vertices of ring.
            for (I32 j = 0; j <= slice_count; ++j)
            {
                Real theta = j * theta_step;

                // spherical to cartesian
                Vector3 vertex_local_pos;
                vertex_local_pos.x = sinf(phi) * cosf(theta);
                vertex_local_pos.y = cosf(phi);
                vertex_local_pos.z = sinf(phi) * sinf(theta);
                vertex_local_pos   = vertex_local_pos * radius;
                vertex_local_pos   = m_orientation.Rotate(vertex_local_pos);
                vertex_local_pos += m_position;
                vertex_local_pos = body_orientation.Rotate(vertex_local_pos);
                vertex_local_pos += body_position;
                renderer->PushVertex(vertex_local_pos, mode, color);
            }
        }
        renderer->PushVertex(bottom_vertex_local_pos, mode, color);
        if (mode == RenderingMode::Dot)
        {
            I32 count = renderer->SPHERICAL_VERTICES_COUNT;
            for (I32 i = 0; i < count; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == RenderingMode::Line)
        {
            for (I32 i = 1; i <= slice_count; ++i)
            {
                renderer->PushLineIndices(index, index + i + 1);
            }
            I32 base = 1 + index;
            I32 ring = slice_count + 1;
            for (I32 i = 0; i < stack_count - 2; ++i)
            {
                for (I32 j = 0; j < slice_count; ++j)
                {
                    renderer->PushLineIndices(base + i * ring + j, base + i * ring + j + 1);
                    renderer->PushLineIndices(base + i * ring + j, base + (i + 1) * ring + j);
                }
            }
            I32 south_pole_index = static_cast<I32>(renderer->VerticesSize(mode)) - 1;
            base                 = south_pole_index - ring;
            for (I32 i = 0; i < slice_count; ++i)
            {
                renderer->PushLineIndices(south_pole_index, base + i);
            }
        }
        else if (mode == RenderingMode::Face)
        {
            for (I32 i = 1; i <= slice_count; ++i)
            {
                renderer->PushFaceIndices(index, index + i + 1, index + i);
            }
            I32 base = 1 + index;
            I32 ring = slice_count + 1;
            for (I32 i = 0; i < stack_count - 2; ++i)
            {
                for (I32 j = 0; j < slice_count; ++j)
                {
                    renderer->PushFaceIndices(
                                              base + i * ring + j,
                                              base + i * ring + j + 1,
                                              base + (i + 1) * ring + j);
                    renderer->PushFaceIndices(
                                              base + (i + 1) * ring + j,
                                              base + i * ring + j + 1,
                                              base + (i + 1) * ring + j + 1);
                }
            }
            I32 south_pole_index = static_cast<I32>(renderer->VerticesSize(mode)) - 1;
            base                 = south_pole_index - ring;
            for (I32 i = 0; i < slice_count; ++i)
            {
                renderer->PushFaceIndices(south_pole_index, base + i, base + i + 1);
            }
        }
    }

    Real ColliderSphere::Radius() const
    {
        if (m_rigid_body != nullptr)
        {
            return m_transformed_radius;
        }
        return m_radius;
    }

    void ColliderSphere::Clone(ColliderPrimitive* cloned)
    {
    }
}
