#include <iostream>

#include "Application.hpp"
#include "render/OpenGL/Renderer.hpp"
#include "render/DirectX10/Renderer.hpp"
#include "Log.hpp"
#include "ConfParser.hpp"

namespace Graphic
{
	Application::Application(HINSTANCE hInstance, PWSTR nCmdLine, int nCmdShow)
		:	m_hInstance(hInstance),
			m_nCmdShow(nCmdShow)
	{}

	Application::~Application()
	{
	}


	int Application::start(unsigned int width, unsigned int height)
	{
		ConfParser config{};
		config.parse("res/config.txt");

		std::string renderer = config.getRednerer();
		if (renderer == "OpenGL")
		{
			Renderer window = Renderer();
			int returnCode = window.init(width, height);
			return returnCode;
		}
		else if (renderer == "DirectX10")
		{
			return initWindow("NeuralEngineClass", "NeuralEngine");
		}
		else
		{
			return initWindow("NeuralEngineClass", "NeuralEngine");
		}
	}

	int Application::initWindow(std::string className, std::string wndName)
	{
		WNDCLASSEX wndClass;
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.lpfnWndProc = [](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
		{
			switch (msg)
			{
			case WM_DESTROY:
				PostQuitMessage(EXIT_SUCCESS);
				break;

			default:
				break;
			}
			return DefWindowProc(hWnd, msg, wParam, lParam);
		};
		wndClass.lpszClassName = className.c_str();
		wndClass.hInstance = m_hInstance;
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wndClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		wndClass.lpszMenuName = nullptr;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		if (FAILED(RegisterClassEx(&wndClass)))
		{
			MessageBox(NULL, "Can't register window class", "Error", MB_ICONERROR | MB_OK);
			PostQuitMessage(E_FAIL);
			return -5;
		}

		if (FAILED(m_hWnd = CreateWindowEx(
			WS_EX_ACCEPTFILES,
			className.c_str(),
			wndName.c_str(),
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			nullptr,
			nullptr,
			m_hInstance,
			nullptr
		)))
		{
			MessageBox(NULL, "Can't create window", "Error", MB_ICONERROR | MB_OK);
			PostQuitMessage(E_FAIL);
			return -6;
		}

		ShowWindow(m_hWnd, m_nCmdShow);
		UpdateWindow(m_hWnd);

		return run();
	}

	int Application::run()
	{
		DirectX10::Renderer renderer = DirectX10::Renderer(m_hWnd);
		renderer.init();

		while (m_msg.message != WM_QUIT)
		{
			if (PeekMessage(&m_msg, m_hWnd, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&m_msg);
				DispatchMessage(&m_msg);
			}
			else
			{
				renderer.on_update();
			}
		}

		return m_msg.wParam;
	}
}