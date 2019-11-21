#pragma once
#include "ColliderPrimitive.hpp"

namespace Engine5
{
    class ColliderBox final : public ColliderPrimitive
    {
    public:
        ColliderBox();
        ~ColliderBox() override;
        ColliderBox(const ColliderBox& rhs)            = delete;
        ColliderBox& operator=(const ColliderBox& rhs) = delete;

        void Initialize() override;
        void Shutdown() override;

        //minkowski support - gjk, epa
        Vector3 Support(const Vector3& direction) override;

        //ray - collider intersection
        bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const override;
        Vector3 GetNormal(const Vector3& local_point_on_collider) override;

        //physics
        void SetMassData(Real density) override;
        Real GetVolume() override;

        //collider 
        void SetScale(const Vector3& scale) override;
        void SetUnit() override;
        void UpdateBoundingVolume() override;
        void Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const override;

        Vector3 Vertex(size_t i) const;
        void    SetBox(Real width, Real height, Real depth);

    protected:
        void Clone(ColliderPrimitive* cloned) override;

    private:
        Vector3 m_vertices[8];
        Vector3 m_transformed_vertices[8];
    };
}
