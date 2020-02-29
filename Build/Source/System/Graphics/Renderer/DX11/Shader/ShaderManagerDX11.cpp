#include "ShaderManagerDX11.hpp"
#include "../../../Shader/ShaderManager.hpp"
#include "../../../Shader/ColorShaderCommon.hpp"
#include "../../RendererCommon.hpp"

namespace Engine5
{
    ShaderManagerDX11::ShaderManagerDX11()
    {
    }

    ShaderManagerDX11::~ShaderManagerDX11()
    {
    }

    void ShaderManagerDX11::SetHWnd(HWND hwnd)
    {
        m_hwnd = hwnd;
    }

    void ShaderManagerDX11::SetDevice(ID3D11Device* device)
    {
        m_device = device;
    }

    void ShaderManagerDX11::SetDeviceContext(ID3D11DeviceContext* device_context)
    {
        m_device_context = device_context;
    }

    ShaderManager::ShaderManager()
    {
    }

    ShaderManager::~ShaderManager()
    {
    }

    void ShaderManager::Initialize(RendererCommon* renderer)
    {
        SetHWnd(renderer->GetHwnd());
        SetDevice(renderer->GetDevice());
        SetDeviceContext(renderer->GetDeviceContext());

        m_color_shader = new ColorShaderCommon();
        m_color_shader->SetVertexShaderPath("../../Resource/Shader/Color.vs");
        m_color_shader->SetPixelShaderPath("../../Resource/Shader/Color.ps");
        m_color_shader->SetHWnd(m_hwnd);
        m_color_shader->SetDevice(m_device);
        m_color_shader->SetDeviceContext(m_device_context);
        m_color_shader->Initialize();
    }

    void ShaderManager::Update(Real dt)
    {
    }

    void ShaderManager::Shutdown()
    {
        if (m_color_shader != nullptr)
        {
            m_color_shader->Shutdown();
            delete m_color_shader;
            m_color_shader = nullptr;
        }
    }

    ColorShaderCommon* ShaderManager::GetColorShader() const
    {
        return m_color_shader;
    }
}
