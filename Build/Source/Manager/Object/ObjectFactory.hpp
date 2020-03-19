#pragma once
#include <vector>
#include <string>
#include <unordered_map>

namespace Engine5
{
    class JsonResource;
    class ComponentRegistry;
    class ComponentManager;
    class Object;
    class ObjectManager;

    class ObjectFactory
    {
    public:
        ObjectFactory();
        ~ObjectFactory();

        void Initialize(ComponentRegistry* component_registry);
        void Shutdown();

        Object* CreateRawObject(const std::string& name);
        Object* CreateRawObject(const std::string& name, ObjectManager* object_manager);
        Object* CreateArchetypeObject(const std::string& name, size_t archetype_id, ObjectManager* object_manager, ComponentManager* component_manager);

        void AddArchetype(Object* object);
        void AddArchetype(JsonResource* resource);

        size_t GetArchetypeID(JsonResource* resource);
    private:
        void ClearArchetypes();

    private:
        friend class Object;

    private:
        std::unordered_map<JsonResource*, size_t> m_resource_id;
        //others
        std::vector<Object*> m_archetypes;
        ComponentManager*    m_archetype_component_manager = nullptr;
    };
}
