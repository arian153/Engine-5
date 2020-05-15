#include "ParticleEmitter.hpp"
#include "../DataType/Particle.hpp"
#include "../../Math/Utility/Random.hpp"
#include "../Buffer/InstanceBufferCommon.hpp"
#include "../../Math/Algebra/Matrix44.hpp"
#include "../../../Manager/Component/EngineComponent/ParticleEmitterComponent.hpp"
#include "../../Math/Structure/Transform.hpp"

namespace Engine5
{
    ParticleEmitter::ParticleEmitter()
    {
    }

    ParticleEmitter::~ParticleEmitter()
    {
    }

    void ParticleEmitter::Initialize()
    {
        m_base_particle.life     = 100.0f;
        m_base_particle.position = Vector3(0.0f, 10.0f, 0.0f);
        m_base_particle.velocity = Vector3(0.0f, 0.0f, 0.0f);
        m_base_particle.scale    = 0.25f;
        m_base_particle.color    = ColorDef::Pure::Black;
        m_direction_variance     = Vector3(1.0f, 1.0f, 1.0f);
        m_speed_variance         = 0.20f;
        m_color_variance         = Color(0.7f, 0.0f, 0.0f, 1.0f);
        //m_position_variance = Vector3(1.0f, 1.0f, 1.0f);
        m_emission_amount  = 1;
        m_scale_decay_rate = 0.01f;
        m_emission_rate    = 0.05f;
        m_life_decay_rate  = 0.0f;
    }

    void ParticleEmitter::Update(Real dt)
    {
        m_elapsed_time += dt;
        if (m_elapsed_time > m_emission_rate)
        {
            m_elapsed_time = 0.0f;
            EmitParticles();
        }
        
        m_instances.clear();
        m_instances.reserve(m_max_amount);
        m_active_amount = 0;
        for (size_t i = 0; i < m_max_amount; ++i)
        {
            if (m_particles[i].IsActive())
            {
                m_particles[i].position += m_particles[i].velocity * dt;
                m_particles[i].life -= m_base_particle.life * m_life_decay_rate * dt;
                m_particles[i].scale -= m_base_particle.scale * m_scale_decay_rate * dt;
                auto world = ParticleToWorldMatrix(m_particles[i]);
                m_instances.emplace_back(world, m_particles[i].color);
                m_active_amount++;
            }
        }
        m_buffer->UpdateInstanceBuffer(m_instances);
    }

    void ParticleEmitter::Render() const
    {
        if (m_buffer != nullptr)
        {
            m_buffer->Render(sizeof(TextureVertexCommon), 0);
        }
    }

    void ParticleEmitter::Shutdown()
    {
        if (m_component != nullptr)
        {
            m_component->m_emitter = nullptr;
        }
        m_instances.clear();
        if (m_particles != nullptr)
        {
            delete[] m_particles;
            m_particles = nullptr;
        }
        if (m_buffer != nullptr)
        {
            m_buffer->Shutdown();
            delete m_buffer;
            m_buffer = nullptr;
        }
    }

    void ParticleEmitter::AddParticle(const Particle& particle)
    {
        size_t index       = GetFreeIndex();
        m_particles[index] = particle;
    }

    void ParticleEmitter::AddParticle(const Vector3& pos, const Vector3& vel, const Color& color, Real life, Real scale)
    {
        size_t    index   = GetFreeIndex();
        Particle& created = m_particles[index];
        created.position  = pos;
        created.velocity  = vel;
        created.color     = color;
        created.life      = life;
        created.scale     = scale;
    }

    void ParticleEmitter::SetRenderer(RendererCommon* renderer)
    {
        m_renderer = renderer;
    }

    void ParticleEmitter::SetBuffer()
    {
        if (m_buffer == nullptr)
        {
            m_buffer = new InstanceBufferCommon();
            //vertices
            std::vector<TextureVertexCommon> vertices;
            vertices.emplace_back(-0.5f, 0.5f, 0.0f, 0.0f, 0.0f);
            vertices.emplace_back(0.5f, -0.5f, 0.0f, 1.0f, 1.0f);
            vertices.emplace_back(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f);
            vertices.emplace_back(0.5f, 0.5f, 0.0f, 1.0f, 0.0f);
            //indices
            std::vector<U32> indices;
            indices.push_back(0);
            indices.push_back(1);
            indices.push_back(2);
            indices.push_back(0);
            indices.push_back(3);
            indices.push_back(1);
            m_buffer->BuildBuffer(m_renderer, vertices, indices);
            m_index_count = 6;
            vertices.clear();
            indices.clear();
        }
    }

    void ParticleEmitter::SetTexture(TextureCommon* texture)
    {
        m_texture = texture;
    }

    void ParticleEmitter::SetTransform(Transform* transform)
    {
        m_transform = transform;
    }

    void ParticleEmitter::SetParticleAmount(size_t amount)
    {
        if (m_particles != nullptr)
        {
            delete[] m_particles;
        }
        m_max_amount = amount;
        m_particles  = new Particle[ m_max_amount ];
        m_instances.resize(m_max_amount);
        m_buffer->BuildInstanceBuffer(m_instances);
    }

    void ParticleEmitter::SetEmissionAmount(size_t amount)
    {
        m_emission_amount = amount;
    }

    void ParticleEmitter::SetEmissionRate(Real rate)
    {
        m_emission_rate = rate;
    }

    void ParticleEmitter::SetLifeDecayRate(Real rate)
    {
        if (rate >= 0.0f)
        {
            m_life_decay_rate = rate;
        }
    }

    void ParticleEmitter::SetScaleDecayRate(Real rate)
    {
        if (rate >= 0.0f)
        {
            m_scale_decay_rate = rate;
        }
    }

    void ParticleEmitter::SetBaseParticle(const Particle& particle)
    {
        m_base_particle = particle;
    }

    void ParticleEmitter::SetBasePosition(const Vector3& position)
    {
        m_base_particle.position = position;
    }

    void ParticleEmitter::SetBaseVelocity(const Vector3& velocity)
    {
        m_base_particle.velocity = velocity;
    }

    void ParticleEmitter::SetBaseColor(const Color& color)
    {
        m_base_particle.color = color;
    }

    void ParticleEmitter::SetBaseLife(Real life_time)
    {
        m_base_particle.life = life_time;
    }

    void ParticleEmitter::SetBaseScale(Real scale)
    {
        m_base_particle.scale = scale;
    }

    void ParticleEmitter::SetPositionVariance(const Vector3& variance)
    {
        m_position_variance = variance;
    }

    void ParticleEmitter::SetDirectionVariance(const Vector3& variance)
    {
        m_direction_variance = variance;
    }

    void ParticleEmitter::SetColorVariance(const Color& variance)
    {
        m_color_variance = variance;
    }

    void ParticleEmitter::SetSpeedVariance(Real variance)
    {
        m_speed_variance = variance;
    }

    void ParticleEmitter::SetScaleVariance(Real variance)
    {
        m_scale_variance = variance;
    }

    void ParticleEmitter::SetLifeVariance(Real variance)
    {
        m_life_variance = variance;
    }

    size_t ParticleEmitter::GetIndexCount() const
    {
        return m_index_count;
    }

    size_t ParticleEmitter::GetInstanceCount() const
    {
        return m_active_amount;
    }

    TextureCommon* ParticleEmitter::GetTexture() const
    {
        return m_texture;
    }

    Matrix44 ParticleEmitter::GetModelMatrix() const
    {
        if (m_transform != nullptr)
        {
            return m_transform->LocalToWorldMatrix();
        }
        return Matrix44();
    }

    size_t ParticleEmitter::GetFreeIndex()
    {
        for (size_t i = m_free_particle; i < m_max_amount; ++i)
        {
            if (m_particles[i].IsActive() == false)
            {
                m_free_particle = i;
                return m_free_particle;
            }
        }
        for (size_t i = 0; i < m_free_particle; ++i)
        {
            if (m_particles[i].IsActive() == false)
            {
                m_free_particle = i;
                return m_free_particle;
            }
        }
        m_free_particle++;
        if (m_free_particle >= m_max_amount)
        {
            m_free_particle = 0;
        }
        return m_free_particle;
    }

    void ParticleEmitter::EmitParticles()
    {
        Random random;
        for (size_t i = 0; i < m_emission_amount; ++i)
        {
            Particle particle;
            //position
            particle.position.x = random.GetRangedRandomReal(-m_position_variance.x, m_position_variance.x);
            particle.position.y = random.GetRangedRandomReal(-m_position_variance.y, m_position_variance.y);
            particle.position.z = random.GetRangedRandomReal(-m_position_variance.z, m_position_variance.z);
            //velocity
            Vector3 random_dir;
            random_dir.x      = random.GetRangedRandomReal(-m_direction_variance.x, m_direction_variance.x);
            random_dir.y      = random.GetRangedRandomReal(-m_direction_variance.y, m_direction_variance.y);
            random_dir.z      = random.GetRangedRandomReal(-m_direction_variance.z, m_direction_variance.z);
            Real random_speed = random.GetRangedRandomReal(0.0f, m_speed_variance);
            particle.velocity = random_speed * random_dir.Normalize();
            //color
            particle.color.r = random.GetRangedRandomReal(0.0f, m_color_variance.r);
            particle.color.g = random.GetRangedRandomReal(0.0f, m_color_variance.g);
            particle.color.b = random.GetRangedRandomReal(0.0f, m_color_variance.b);
            particle.color.a = random.GetRangedRandomReal(0.0f, m_color_variance.a);
            //life
            particle.life = random.GetRangedRandomReal(0.0f, m_life_variance);
            //scale
            particle.scale = random.GetRangedRandomReal(0.0f, m_scale_variance);
            //apply default data
            particle.Merge(m_base_particle);
            AddParticle(particle);
        }
    }

    Matrix44 ParticleEmitter::ParticleToWorldMatrix(const Particle& particle) const
    {
        Matrix44 result;
        result.SetDiagonal(particle.scale, particle.scale, particle.scale, 1.0f);
        result.AddVectorColumn(3, particle.position);
        return result;
    }
}
