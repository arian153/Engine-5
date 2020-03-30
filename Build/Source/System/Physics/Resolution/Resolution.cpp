#include "Resolution.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"
#include "Constraints/ContactConstraints.hpp"
#include <vector>
// ReSharper disable once CppUnusedIncludeDirective
#include "ContactManifold.hpp"
#include "../../Graphics/DataType/Color.hpp"

namespace Engine5
{
    Resolution::Resolution()
    {
    }

    Resolution::~Resolution()
    {
    }

    void Resolution::Initialize()
    {
        m_friction.Initialize();
    }

    void Resolution::Shutdown()
    {
        m_friction.Shutdown();
    }

    void Resolution::Solve(ManifoldTable* manifold_table, std::vector<RigidBody*>* rigid_bodies, Real dt)
    {
        //resolution phase
        m_contacts.clear();
        //solve contact manifold
        for (auto& manifold : manifold_table->m_manifold_table)
        {
            auto contact = m_contacts.emplace_back(&manifold.second, &m_friction);
            contact.Initialize();
        }
        if (m_b_warm_starting == true)
        {
            for (auto& contact : m_contacts)
            {
                contact.WarmStart();
            }
        }
        for (size_t i = 0; i < m_velocity_iteration; ++i)
        {
            for (auto& contact : m_contacts)
            {
                contact.Solve(dt);
            }
        }
        //apply
        for (auto& contact : m_contacts)
        {
            contact.Apply();
        }
        //integration phase
        for (auto& body : *rigid_bodies)
        {
            body->Integrate(dt);
        }
        //solve position constraints.
        for (size_t i = 0; i < m_position_iteration; ++i)
        {
            for (auto& contact : m_contacts)
            {
                contact.SolvePositionConstraints();
            }
        }
       
    }

    void Resolution::SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer)
    {
        m_primitive_renderer = primitive_renderer;
    }

    void Resolution::Draw(const ColorFlag& draw_contact_flag)
    {
        if (draw_contact_flag.b_flag)
        {
            for (auto& contact : m_contacts)
            {
                contact.Draw(m_primitive_renderer, draw_contact_flag.color);
            }
        }
    }
}
