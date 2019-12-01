#pragma once
#include "../ColliderPrimitive.hpp"
#include "../ColliderEdge.hpp"

namespace Engine5
{
    class ColliderPolygon final : public ColliderPrimitive
    {
    public:
        ColliderPolygon();
        ~ColliderPolygon() override;
        ColliderPolygon(const ColliderPolygon& rhs)            = delete;
        ColliderPolygon& operator=(const ColliderPolygon& rhs) = delete;

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
        void SetScaleData(const Vector3& scale) override;
        void SetUnit() override;
        void UpdateBoundingVolume() override;
        void Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const override;

    protected:
        void Clone(ColliderPrimitive* cloned) override;

    private:
        std::vector<Vector2>*      m_vertices        = nullptr;
        std::vector<Vector2>*      m_scaled_vertices = nullptr;
        std::vector<ColliderEdge>* m_edges           = nullptr;

        Vector2 m_min_vertex;
        Vector2 m_max_vertex;
    };
}