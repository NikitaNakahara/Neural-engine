#include "Renderer.hpp"

namespace Graphic
{
	namespace DirectX10
	{
		Renderer::Renderer(HWND hWnd)
			: m_hWnd(hWnd)
		{}

		Renderer::~Renderer() {}

		int Renderer::init()
		{
			RECT rect;
			GetClientRect(m_hWnd, &rect);
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;

			DXGI_SWAP_CHAIN_DESC dscd;
			ZeroMemory(&dscd, sizeof(dscd));
			dscd.BufferCount = 1;
			dscd.BufferDesc.Width = width;
			dscd.BufferDesc.Height = height;
			dscd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			dscd.BufferDesc.RefreshRate.Denominator = 1;
			dscd.BufferDesc.RefreshRate.Numerator = 60;
			dscd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			dscd.OutputWindow = m_hWnd;
			dscd.SampleDesc.Count = 1;
			dscd.SampleDesc.Quality = 0;
			dscd.Windowed = TRUE;

			if (FAILED(D3D10CreateDeviceAndSwapChain(
				NULL,
				D3D10_DRIVER_TYPE_REFERENCE,
				NULL,
				0,
				D3D10_SDK_VERSION,
				&dscd,
				&m_pSwapChain,
				&m_pD3Device
			)))
			{
				MessageBox(NULL, "Can't create device and swap chain", "Error", MB_ICONERROR | MB_OK);
				PostQuitMessage(E_FAIL);
				return -1;
			}

			ID3D10Texture2D* pBackBuffer;
			if (FAILED(m_pSwapChain->GetBuffer(
				0,
				__uuidof(ID3D10Texture2D),
				(LPVOID*)&pBackBuffer
			)))
			{
				MessageBox(NULL, "Can't get swap chain buffer", "Error", MB_ICONERROR | MB_OK);
				PostQuitMessage(E_FAIL);
				return -2;
			}

			if (FAILED(m_pD3Device->CreateRenderTargetView(
				pBackBuffer,
				NULL,
				&m_pRenderTarget
			)))
			{
				MessageBox(NULL, "Can't create render target view", "Error", MB_ICONERROR | MB_OK);
				PostQuitMessage(E_FAIL);
				return -3;
			}
			pBackBuffer->Release();

			m_pD3Device->OMSetRenderTargets(
				1,
				&m_pRenderTarget,
				NULL
			);

			D3D10_VIEWPORT viewport;
			viewport.Width = width;
			viewport.Height = height;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;

			m_pD3Device->RSSetViewports(1, &viewport);

			return 0;
		}

		std::array<bool, 3> color = { false, false, false };

		void Renderer::on_update()
		{
			for (int i = 0; i < 3; i++)
			{
				if (clearColor[i] < 0.1)
					color[i] = true;
				else if (clearColor[i] > 0.9)
					color[i] = false;

				if (color[i])
				{
					if (i == 0)
						clearColor[i] += 0.001;
					else if (i == 1)
						clearColor[i] += 0.01;
					else if (i == 2)
						clearColor[i] += 0.0001;
				}
				else
				{
					if (i == 0)
						clearColor[i] -= 0.001;
					else if (i == 1)
						clearColor[i] -= 0.01;
					else if (i == 2)
						clearColor[i] -= 0.0001;
				}
			}

			m_pD3Device->ClearRenderTargetView(m_pRenderTarget, clearColor.data());
			m_pSwapChain->Present(0, 0);
		}
	}
}