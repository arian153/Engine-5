#include "Component.hpp"

namespace Engine5
{
    Component::Component()
    {
    }

    Component::~Component()
    {
    }

    String Component::Type() const
    {
        return m_type;
    }

    bool Component::IsLoaded() const
    {
        return m_b_loaded;
    }

    bool Component::IsUnloaded() const
    {
        return m_b_unloaded;
    }
}