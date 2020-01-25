#include "FillteringPhase.hpp"
#include "../Resolution/ContactManifold.hpp"
#include "../Dynamics/ColliderSet.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"

namespace Engine5
{
    FillteringPhase::FillteringPhase()
    {
    }

    FillteringPhase::~FillteringPhase()
    {
    }

    void FillteringPhase::Initialize(std::unordered_multimap<size_t, ContactManifold>* data_table)
    {
        m_manifold_table = data_table;
    }

    void FillteringPhase::Shutdown()
    {
        m_key_table.clear();
    }

    void FillteringPhase::SendHasCollision(ColliderSet* a, ColliderSet* b, bool was_collision)
    {
        size_t key = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
        auto   key_range = m_state_table.equal_range(key);
        if (key_range.first != key_range.second)
        {
            bool is_duplicated = false;
            for (auto it = key_range.first; it != key_range.second; ++it)
            {
                if (((*it).second.set_a == a && (*it).second.set_b == b) ||
                    ((*it).second.set_b == a && (*it).second.set_a == b))
                {
                    // in this case, keep previous contact data for collision event.
                    is_duplicated = true;
                    if (was_collision == true)
                    {
                        (*it).second.state = CollisionState::Persist;
                    }
                    else
                    {
                        (*it).second.state = CollisionState::Start;
                    }
                    break;
                }
            }

            if (is_duplicated == false)
            {
                //found same key of pair. 
                //but this data is new data, so add a new collision data.
                CollisionStateData collision_data;
                collision_data.set_a = a;
                collision_data.set_b = b;

                if (was_collision == true)
                {
                    collision_data.state = CollisionState::Persist;
                }
                else
                {
                    collision_data.state = CollisionState::Start;
                }
                m_state_table.emplace(key, collision_data);
            }
        }
        else
        {
            //found new pair. add a new collision data.
            CollisionStateData collision_data;
            collision_data.set_a = a;
            collision_data.set_b = b;

            if (was_collision == true)
            {
                collision_data.state = CollisionState::Persist;
            }
            else
            {
                collision_data.state = CollisionState::Start;
            }

            m_state_table.emplace(key, collision_data);
        }
    }

    void FillteringPhase::SendNotCollision(ColliderSet* a, ColliderSet* b, bool was_collision)
    {
        size_t key = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
        auto   key_range = m_state_table.equal_range(key);
        if (key_range.first != key_range.second)
        {
            bool is_duplicated = false;
            for (auto it = key_range.first; it != key_range.second; ++it)
            {
                if (((*it).second.set_a == a && (*it).second.set_b == b) ||
                    ((*it).second.set_b == a && (*it).second.set_a == b))
                {
                    // in this case, keep previous contact data for collision event.
                    is_duplicated = true;
                    if (was_collision == true)
                    {
                        (*it).second.state = CollisionState::End;
                    }
                    else
                    {
                        (*it).second.state = CollisionState::None;
                    }
                    break;
                }
            }
            if (is_duplicated == false)
            {
                //found same key of pair. 
                //but this data is new data, so add a new collision data.
                CollisionStateData collision_data;
                collision_data.set_a = a;
                collision_data.set_b = b;

                if (was_collision == true)
                {
                    collision_data.state = CollisionState::End;
                }
                else
                {
                    collision_data.state = CollisionState::None;
                }
                m_state_table.emplace(key, collision_data);
            }
        }
        else
        {
            //found new pair. add a new collision data.
            CollisionStateData collision_data;
            collision_data.set_a = a;
            collision_data.set_b = b;

            if (was_collision == true)
            {
                collision_data.state = CollisionState::End;
            }
            else
            {
                collision_data.state = CollisionState::None;
            }
            m_state_table.emplace(key, collision_data);
        }
    }

    void FillteringPhase::SendInvalidCollision(ColliderSet* a, ColliderSet* b)
    {
        size_t key = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
        auto   key_range = m_state_table.equal_range(key);
        if (key_range.first != key_range.second)
        {
            bool is_duplicated = false;
            for (auto it = key_range.first; it != key_range.second; ++it)
            {
                if (((*it).second.set_a == a && (*it).second.set_b == b) ||
                    ((*it).second.set_b == a && (*it).second.set_a == b))
                {
                    // in this case, keep previous contact data for collision event.
                    is_duplicated = true;
                    (*it).second.state = CollisionState::Invalid;
                    break;
                }
            }
            if (is_duplicated == false)
            {
                //found same key of pair. 
                //but this data is new data, so add a new collision data.
                CollisionStateData collision_data;
                collision_data.set_a = a;
                collision_data.set_b = b;

                collision_data.state = CollisionState::Invalid;
                m_state_table.emplace(key, collision_data);
            }
        }
        else
        {
            //found new pair. add a new collision data.
            CollisionStateData collision_data;
            collision_data.set_a = a;
            collision_data.set_b = b;

            collision_data.state = CollisionState::Invalid;
            m_state_table.emplace(key, collision_data);
        }
    }

    size_t FillteringPhase::GenerateKey(ColliderSet* a, ColliderSet* b)
    {
        return reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
    }

    size_t FillteringPhase::GenerateKey(ColliderPrimitive* a, ColliderPrimitive* b)
    {
        return reinterpret_cast<size_t>(a->m_collider_set) + reinterpret_cast<size_t>(b->m_collider_set);
    }

    size_t FillteringPhase::GenerateKey(ContactManifold* manifold)
    {
        return reinterpret_cast<size_t>(manifold->m_set_a) + reinterpret_cast<size_t>(manifold->m_set_b);
    }

    auto FillteringPhase::FindAssociatedPairs(ColliderSet* key)
    {
        return m_key_table.equal_range(key);
    }

    auto FillteringPhase::FindCollisionDatas(ColliderSet* a, ColliderSet* b, size_t at)
    {
        size_t key = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);

        Vector3Pair result;

        auto   data_range = m_manifold_table->equal_range(key);
        if (data_range.first != data_range.second)
        {
            for (auto it = data_range.first; it != data_range.second; ++it)
            {
                if (it->second.m_set_a == a && it->second.m_set_b == b)
                {
                    size_t count = it->second.ContactsCount();
                    size_t index = at < count ? at : 0;
                    result.a = it->second.contacts.at(index).local_position_a;
                    result.b = it->second.contacts.at(index).local_position_b;
                    return result;
                }
                else if (it->second.m_set_b == a && it->second.m_set_a == b)
                {
                    size_t count = it->second.ContactsCount();
                    size_t index = at < count ? at : 0;
                    result.b = it->second.contacts.at(index).local_position_a;
                    result.a = it->second.contacts.at(index).local_position_b;
                    return result;
                }
            }
        }

        //result.a = Math::Vector3::INVALID;
        //result.b = Math::Vector3::INVALID;
        return result;
    }

    auto FillteringPhase::FindColisionState(ColliderSet* a, ColliderSet* b)
    {
        size_t key = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
        auto   data_range = m_state_table.equal_range(key);
        if (data_range.first != data_range.second)
        {
            for (auto it = data_range.first; it != data_range.second; ++it)
            {
                if ((it->second.set_a == a && it->second.set_b == b) ||
                    (it->second.set_b == a && it->second.set_a == b))
                {
                    return it->second.state;
                }
            }
        }
        return CollisionState::None;
    }

    ContactManifold* FillteringPhase::FindManifold(ColliderSet* a, ColliderSet* b)
    {
        size_t key = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
        auto   data_range = m_manifold_table->equal_range(key);
        if (data_range.first != data_range.second)
        {
            for (auto it = data_range.first; it != data_range.second; ++it)
            {
                if (it->second.m_set_a == a && it->second.m_set_b == b ||
                    it->second.m_set_b == a && it->second.m_set_a == b)
                {
                    return &it->second;
                }
            }
        }

        return nullptr;
    }

    ContactManifold* FillteringPhase::CreateManifold(ColliderSet* a, ColliderSet* b)
    {
        size_t key = RegisterKey(a, b);
        auto result = m_manifold_table->emplace(key, ContactManifold(a, b));
        return &result->second;
    }

    bool FillteringPhase::HasManifold(ColliderSet* a, ColliderSet* b)
    {
        size_t key = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
        auto   data_range = m_manifold_table->equal_range(key);
        if (data_range.first != data_range.second)
        {
            for (auto it = data_range.first; it != data_range.second; ++it)
            {
                if (it->second.m_set_a == a && it->second.m_set_b == b ||
                    it->second.m_set_b == a && it->second.m_set_a == b)
                {
                    return true;
                }
            }
        }

        return false;
    }

    void FillteringPhase::FilteringManifolds()
    {
        for (auto it = m_manifold_table->begin(); it != m_manifold_table->end();)
        {
            //if sets don't intersect each other, remove previous frame's manifolds.
            if (it->second.m_set_a->m_bounding_volume.Intersect(it->second.m_set_b->m_bounding_volume) == false)
            {
                DeregisterKey(it->second.m_set_a, it->second.m_set_b);
                m_manifold_table->erase(it++);
            }
            else
            {
                //else their is a potential contacts in manifold. and then update invalid contact in manifolds
                it->second.UpdateInvalidContact();
                ++it;
            }
        }
    }

    size_t FillteringPhase::RegisterKey(ColliderSet* a, ColliderSet* b)
    {
        auto key_range_a = m_key_table.equal_range(a);
        if (key_range_a.first != key_range_a.second)
        {
            bool is_duplicated = false;
            for (auto it = key_range_a.first; it != key_range_a.second; ++it)
            {
                if ((*it).second == b)
                {
                    is_duplicated = true;
                    break;
                }
            }
            if (is_duplicated == false)
            {
                m_key_table.emplace(a, b);
            }
        }
        else
        {
            m_key_table.emplace(a, b);
        }

        auto key_range_b = m_key_table.equal_range(b);
        if (key_range_b.first != key_range_b.second)
        {
            bool is_duplicated = false;
            for (auto it = key_range_b.first; it != key_range_b.second; ++it)
            {
                if ((*it).second == a)
                {
                    is_duplicated = true;
                    break;
                }
            }
            if (is_duplicated == false)
            {
                m_key_table.emplace(b, a);
            }
        }
        else
        {
            m_key_table.emplace(b, a);
        }

        return reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
    }

    void FillteringPhase::DeregisterKey(ColliderSet* a, ColliderSet* b)
    {
        auto key_range_a = m_key_table.equal_range(a);
        if (key_range_a.first != key_range_a.second)
        {
            for (auto it = key_range_a.first; it != key_range_a.second; )
            {
                if ((*it).second == b)
                {
                    m_key_table.erase(it++);
                    break;
                }
                else
                {
                    ++it;
                }
            }
        }

        auto key_range_b = m_key_table.equal_range(b);
        if (key_range_b.first != key_range_b.second)
        {
            for (auto it = key_range_b.first; it != key_range_b.second;)
            {
                if ((*it).second == a)
                {
                    m_key_table.erase(it++);
                    break;
                }
                else
                {
                    ++it;
                }
            }
        }

    }

}
