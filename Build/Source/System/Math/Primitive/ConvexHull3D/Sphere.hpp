#pragma once
#include "../Primitive.hpp"


namespace Engine5
{
    class Sphere final : public Primitive
    {
    public:
        Sphere();
        ~Sphere();

        void Initialize() override;
        void Shutdown() override;

        //Primitive Data
        void SetUnit() override;

        //Minkowski Support - gjk, epa
        Vector3 Support(const Vector3& direction) override;

        //Ray - Primitive Intersection
        bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const override;
        Vector3 GetNormal(const Vector3& local_point_on_primitive) override;

        //Draw
        void DrawPrimitive(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) override;
       
    public:
        Real radius = 1.0f;
    };
}
