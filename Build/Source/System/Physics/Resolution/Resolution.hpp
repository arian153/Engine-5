#pragma once
#include "../../Math/Math.hpp"
#include "../NarrowPhase/ManifoldTable.hpp"
#include "../Dynamics/RigidBody.hpp"
#include "../Utility/FrictionUtility.hpp"
#include <vector>
#include "Constraints/ContactConstraints.hpp"

namespace Engine5
{
    class Constraints;
    class ContactPoint;
    class ContactManifold;

    class Resolution
    {
    public:
        Resolution();
        ~Resolution();
        void Initialize();
        void Shutdown();

        void Solve(ManifoldTable* manifold_table, std::vector<RigidBody*>* rigid_bodies, Real dt);

        void SolveConstraints(Constraints* constraints, Real dt) const;


    private:
        bool m_b_warm_starting = true;

        size_t m_velocity_iteration = 8;
        size_t m_position_iteration = 3;

        Physics::FrictionUtility m_friction;
        std::vector<ContactConstraints> m_contacts;

    };
}
