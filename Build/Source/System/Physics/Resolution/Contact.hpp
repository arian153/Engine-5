#pragma once
#include "../../Math/Math.hpp"

namespace Engine5
{
    class ColliderPrimitive;

    class Contact
    {
    public:
        Contact();
        ~Contact();
        void     Clear();
        Contact& operator=(const Contact& rhs);
        bool     operator==(const Contact& rhs) const;

    public:
        //Two contact points in world space, each representing the deepest penetrating point of one collider.
        Vector3 global_position_a;
        Vector3 global_position_b;

        //Two contact points in local spaces of individual colliders, each corresponding to a contact point in world space. 
        //This information is crucial for maintaining persistent contacts. 
        Vector3 local_position_a;
        Vector3 local_position_b;

        //Contact normal representing the "best direction" to separate the two colliding colliders.
        Vector3 normal;

        //Two contact tangents perpendicular to each other and the contact normal. 
        //These vectors are used to resolve the frictional part of collision resolution.
        Vector3 tangent_a;
        Vector3 tangent_b;

        //Penetration depth, a scalar value that represents how deep the overlap of the two colliders.
        Real depth = 0.0f;

        //lagrangian multiplier sum.
        Real normal_impulse_sum    = 0.0f;
        Real tangent_a_impulse_sum = 0.0f;
        Real tangent_b_impulse_sum = 0.0f;

        ColliderPrimitive* collider_a = nullptr;
        ColliderPrimitive* collider_b = nullptr;

        bool is_valid      = true;
        bool is_collide    = false;
        bool is_persistent = false;
    };
}
