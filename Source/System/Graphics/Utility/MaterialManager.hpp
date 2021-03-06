#pragma once
#include <string>
#include <vector>

#include "../DataType/MaterialData.hpp"

namespace Engine5
{
    class MaterialManager
    {
    public:
        MaterialManager();
        ~MaterialManager();

        void Initialize();
        void Shutdown();

        size_t       GetID(const MaterialTexture& material);
        MaterialTexture GetMaterial(size_t id);

    private:
        std::vector<MaterialTexture> m_data;
    };
}
