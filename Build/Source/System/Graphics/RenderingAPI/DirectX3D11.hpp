#pragma once
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d2d1_2.h>
#include <dwrite_2.h>
#include <directxmath.h>
#include <string>
#include "../../Math/Utility/MathDef.hpp"
#include "../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class MatrixGenerator;
    class Color;

    class DirectX3D11
    {
    public:
        explicit DirectX3D11(HWND hwnd, MatrixGenerator* matrix_gen);
        ~DirectX3D11();
        void Initialize(int client_width, int client_height, bool fullscreen_flag, Real far_plane, Real near_plane, Real field_of_view);
        void Update(Real dt);
        void Shutdown();
        void OnResize(int client_width, int client_height, bool fullscreen_flag, Real far_plane, Real near_plane, Real field_of_view);
        void OnFullscreen(bool fullscreen_flag);

        void BeginScene(Color color) const;
        void EndScene() const;
        void SetVSync(bool flag);
        void SetAlphaBlending(bool flag) const;
        void SetZBuffering(bool flag) const;


        ID3D11Device*           GetDevice() const;
        ID3D11DeviceContext*    GetDeviceContext() const;
        ID3D11DepthStencilView* GetDepthStencilView() const;
        //RenderSystem* GetRenderSystem();

        String            GetVideoCardInfo(size_t& memory) const;
        DirectX::XMMATRIX GetProjectionMatrix() const;

        void SetBackBufferRenderTarget() const;
        void SetRasterStateWireFrame(bool flag) const;

    private:
        void SetUpAdapterDescription(int client_width, int client_height);
        void SetUpDevice();
        void SetUpSwapChain(int client_width, int client_height, HWND hwnd, bool fullscreen_flag);
        void SetUpBackBuffer();
        void SetUpDepthBufferDescription(int client_width, int client_height);
        void SetUpStencilStateDescription();
        void SetUpDepthStencilViewDescription();
        void SetUpRasterDescription();
        void SetUpViewport(int client_width, int client_height) const;
        void SetUpMatrices(int client_width, int client_height, Real far_plane, Real near_plane, Real field_of_view);

        void SetUpMultiSamplingLevel();
        void SetUpBlendState();
        void SetUpDWDevice();
        void SetUpDWRenderTarget();

        DirectX::XMMATRIX OrthoGraphicMatrix(size_t client_width, size_t client_height, Real far_plane, Real near_plane) const;

    private:
        IDXGISwapChain*          m_swap_chain                    = nullptr;
        ID3D11Device*            m_device                        = nullptr;
        ID3D11DeviceContext*     m_device_context                = nullptr;
        ID3D11RenderTargetView*  m_render_target_view            = nullptr;
        ID3D11Texture2D*         m_depth_stencil_buffer          = nullptr;
        ID3D11DepthStencilState* m_depth_stencil_state           = nullptr;
        ID3D11DepthStencilView*  m_depth_stencil_view            = nullptr;
        ID3D11RasterizerState*   m_raster_state                  = nullptr;
        ID3D11RasterizerState*   m_wire_frame_raster_state       = nullptr;
        ID3D11DepthStencilState* m_depth_disabled_stencil_state  = nullptr;
        ID3D11BlendState*        m_alpha_enabled_blending_state  = nullptr;
        ID3D11BlendState*        m_alpha_disabled_blending_state = nullptr;
        ID2D1Device1*            m_d2d_device                    = nullptr;
        ID2D1DeviceContext1*     m_d2d_device_context            = nullptr;
        ID2D1Factory2*           m_d2d_factory                   = nullptr;
        IDWriteFactory2*         m_write_factory                 = nullptr;
        ID2D1Bitmap1*            m_target_bitmap                 = nullptr;
        HWND                     m_hwnd                          = nullptr;
        MatrixGenerator*         m_matrix_generator              = nullptr;
        D3D_FEATURE_LEVEL        m_d3d_feature_level;
        DXGI_FORMAT              m_dxgi_color_format = DXGI_FORMAT_B8G8R8A8_UNORM;


        DirectX::XMMATRIX m_projection_matrix;
        DirectX::XMMATRIX m_world_matrix;
        DirectX::XMMATRIX m_ortho_matrix;
        DirectX::XMMATRIX m_oblique_matrix;
        DirectX::XMMATRIX m_cavalier_matrix;
        DirectX::XMMATRIX m_cabinet_matrix;
        DirectX::XMMATRIX m_dimetric_matrix;
        DirectX::XMMATRIX m_trimetric_matrix;
        DirectX::XMMATRIX m_isometric_matrix;
        DirectX::XMMATRIX m_ndc_to_screen_matrix;

        UINT   m_back_buffer_count = 1;
        bool   m_enable_msaa       = false;
        UINT   m_msaa_quality      = 0;
        bool   m_vsync_enabled     = true;
        UINT   m_numerator;
        UINT   m_denominator;
        size_t m_video_card_memory;
        String m_video_card_description;
        size_t m_client_width;
        size_t m_client_height;
        Real   m_dw_dpi = 96.0f;

    private:
    };
}
