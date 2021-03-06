#pragma once
#include "..//Component.hpp"
#include "../../../System/Graphics/DataType/MaterialData.hpp"

namespace Engine5
{
    class MeshResource;
    class Mesh2;
    class Quaternion;
    class Vector3;
    class Transform;

    class MeshComponent final : public Component
    {
    public:
        ~MeshComponent();
        MeshComponent()                                    = delete;
        MeshComponent(const MeshComponent& rhs)            = delete;
        MeshComponent& operator=(const MeshComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class MeshFactory;
        friend class Mesh2;

    private:
        explicit MeshComponent(Object* owner);
        void     Clone(MeshComponent* origin);

    private:
        Mesh2*     m_mesh      = nullptr;
        Transform* m_transform = nullptr;

        std::string     m_model_resource_path = "";
        MaterialTexture m_material_texture;
        MaterialColor   m_material_color;
    };
}
