#include "ColliderFactory.hpp"
#include "../EngineComponent/ColliderComponent.hpp"

namespace Engine5
{
    ColliderFactory::ColliderFactory()
    {
        type = typeid(ColliderComponent).name();
    }

    ColliderFactory::~ColliderFactory()
    {
    }

    Component* ColliderFactory::Create(Object* owner, Space* space)
    {
        return new ColliderComponent(owner);
    }

    Component* ColliderFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<ColliderComponent*>(origin);
        auto cloned = static_cast<ColliderComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
