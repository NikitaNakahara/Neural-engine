#pragma once

#include <Windows.h>
#include <d3d10.h>
#include <array>

namespace Graphic
{
	namespace DirectX10
	{
		class Renderer
		{
		public:
			Renderer(HWND hWnd);
			~Renderer();

			int init();
			void on_update();

		private:
			HWND m_hWnd;
			MSG m_msg;
			IDXGISwapChain* m_pSwapChain;
			ID3D10Device* m_pD3Device;
			ID3D10RenderTargetView* m_pRenderTarget;

			std::array<float, 4> clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		};
	}
}