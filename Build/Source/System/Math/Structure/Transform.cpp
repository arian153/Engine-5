﻿#include "Transform.hpp"
#include "../Utility/MatrixUtility.hpp"

namespace Engine5
{
    Transform::Transform()
    {
    }

    Transform::~Transform()
    {
    }

    Transform& Transform::operator=(const Transform& rhs)
    {
        if (this != &rhs)
        {
            origin      = rhs.origin;
            position    = rhs.position;
            scale       = rhs.scale;
            orientation = rhs.orientation;
        }
        return *this;
    }

    Matrix44 Transform::LocalToWorldMatrix() const
    {
        Matrix44 result = Math::Matrix44::Scale(scale, 1.0f);
        result.AddVectorColumn(3, -origin);
        result = Math::Matrix44::Rotation(orientation) * result;
        result.AddVectorColumn(3, origin);
        result.AddVectorColumn(3, position);
        result.SetTranspose();
        return result;
    }

    Vector3 Transform::LocalToWorldPoint(const Vector3& local_point) const
    {
        return orientation.Rotate(local_point - origin) + origin + position;
    }

    Vector3 Transform::WorldToLocalPoint(const Vector3& world_point) const
    {
        return orientation.Inverse().Rotate(world_point - origin - position) + origin;
    }

    Vector3 Transform::LocalToWorldVector(const Vector3& local_vector) const
    {
        return orientation.Rotate(local_vector);
    }

    Vector3 Transform::WorldToLocalVector(const Vector3& world_vector) const
    {
        return orientation.Inverse().Rotate(world_vector);
    }

    Vector3 Transform::LocalToWorldVectorOrigin(const Vector3& local_vector) const
    {
        return orientation.Rotate(local_vector - origin) + origin;
    }

    Vector3 Transform::WorldToLocalVectorOrigin(const Vector3& world_vector) const
    {
        return orientation.Inverse().Rotate(world_vector - origin) + origin;
    }
}
