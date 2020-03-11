#include "Application.hpp"
#include "OSCommon.hpp"

//systems
#include "../../Graphics/RenderSystem.hpp"
#include "../../Physics/PhysicsSystem.hpp"

//utilities
#include "../Utility/TimeUtility.hpp"
#include <string>
#include "../../../Manager/Object/ObjectFactory.hpp"
#include "../../../Manager/Component/ComponentRegistry.hpp"
#include "../../../Manager/Space/SpaceManager.hpp"
#include "../../../Manager/Level/LevelManager.hpp"
#include "../Utility/FrameUtility.hpp"
#include "../Input/InputCommon.hpp"

namespace Engine5
{
    Application::Application()
    {
        m_s_application = this;
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        Initialize();
        Update();
        Shutdown();
    }

    void Application::Initialize()
    {
        //create independent utility
        m_time_utility = new TimeUtility();
        m_time_utility->Reset();
        m_frame_utility = new FrameUtility();
        //create systems
        m_operating_system = new OSCommon(this);
        m_operating_system->Initialize();
        m_input = new InputCommon();
        m_input->Initialize();
        m_render_system  = new RenderSystem(m_operating_system);
        m_render_system->Initialize(1280, 720);
        m_physics_system = new PhysicsSystem();
        m_physics_system->Initialize();
        //create managers
        m_component_registry = new ComponentRegistry();
        m_component_registry->RegisterFactories();
        m_object_factory = new ObjectFactory();
        m_object_factory->Initialize(m_component_registry);
        m_space_manager = new SpaceManager(m_physics_system, m_render_system, m_object_factory, m_component_registry);
        m_space_manager->Initialize();
        m_level_manager = new LevelManager();
        m_level_manager->Initialize(this);
        m_level_manager->AddLevel("Test");
        m_level_manager->SetInitialLevel("Test");
        //setup missing system parameters
        m_operating_system->SetLevelManager(m_level_manager);
        m_operating_system->SetInput(m_input);
    }

    void Application::Update() const
    {
        while (m_operating_system->IsQuit() == false && m_operating_system->IsInit() == true)
        {
            m_level_manager->Update();
        }
    }

    void Application::Shutdown()
    {
        if (m_level_manager != nullptr)
        {
            m_level_manager->Shutdown();
            delete m_level_manager;
            m_level_manager = nullptr;
        }
        if (m_space_manager != nullptr)
        {
            m_space_manager->Shutdown();
            delete m_space_manager;
            m_space_manager = nullptr;
        }
        if (m_object_factory != nullptr)
        {
            m_object_factory->Shutdown();
            delete m_object_factory;
            m_object_factory = nullptr;
        }
        if (m_component_registry != nullptr)
        {
            m_component_registry->AbolishFactories();
            delete m_component_registry;
            m_component_registry = nullptr;
        }
        if (m_time_utility != nullptr)
        {
            delete m_time_utility;
            m_time_utility = nullptr;
        }
        if (m_frame_utility != nullptr)
        {
            delete m_frame_utility;
            m_frame_utility = nullptr;
        }
        if (m_physics_system != nullptr)
        {
            m_physics_system->Shutdown();
            delete m_physics_system;
            m_physics_system = nullptr;
        }
        if (m_render_system != nullptr)
        {
            m_render_system->Shutdown();
            delete m_render_system;
            m_render_system = nullptr;
        }
        if (m_input != nullptr)
        {
            m_input->Shutdown();
            delete m_input;
            m_input = nullptr;
        }

        if (m_operating_system != nullptr)
        {
            m_operating_system->Shutdown();
            delete m_operating_system;
            m_operating_system = nullptr;
        }
    }

    Application* Application::GetApplication()
    {
        return m_s_application;
    }

    OSCommon* Application::GetOperatingSystem() const
    {
        return m_operating_system;
    }

    RenderSystem* Application::GetRenderSystem() const
    {
        return m_render_system;
    }

    PhysicsSystem* Application::GetPhysicsSystem() const
    {
        return m_physics_system;
    }

    FrameUtility* Application::GetFrameUtility() const
    {
        return m_frame_utility;
    }

    TimeUtility* Application::GetApplicationTimer() const
    {
        return m_time_utility;
    }

    LevelManager* Application::GetLevelManager() const
    {
        return m_level_manager;
    }

    SpaceManager* Application::GetSpaceManager() const
    {
        return m_space_manager;
    }

    ObjectFactory* Application::GetObjectFactory() const
    {
        return m_object_factory;
    }

    ComponentRegistry* Application::GetComponentRegistry() const
    {
        return m_component_registry;
    }

    InputCommon* Application::GetInput() const
    {
        return m_input;
    }

    void Application::OnResize(int client_width, int client_height) const
    {
        if (m_render_system != nullptr)
        {
            m_render_system->OnResize(client_width, client_height);
        }
    }

    void Application::OnFullscreen()
    {
    }

    void Application::OnQuit()
    {
    }
}
