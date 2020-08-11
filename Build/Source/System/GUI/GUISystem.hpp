#pragma once
#include "../../External/imgui/imgui.h"
#include "Editor/GameEditor.hpp"

namespace Engine5
{
    class RendererCommon;
    class Application;

    class GUISystem
    {
    public:
        explicit GUISystem(ImGuiIO& io);
        ~GUISystem();

        void Initialize(Application* application);
        void Shutdown();

        void BeginUpdate() const;
        void EndUpdate();

        void BeginRender() const;
        void EndRender() const;

        static bool IsFocusGUI();
        static void CreateGUIContext();

        void GenerateGUIKeyMap();

    private:
        ImGuiIO&        m_im_gui_io;
        RendererCommon* m_renderer = nullptr;

        GameEditor m_game_editor;

        bool show_demo_window    = true;
        bool show_another_window = false;
        bool show_dock_space     = true;
    };
}
