/**
* @file GLFWE.cpp
*/
#include "GLFWEW.h"
#include <iostream>

/// GLFW��GLEW�����b�v���邽�߂̖��O���.
namespace GLFWEW {

	/**
	* OpenGL����̃G���[�񍐂���������.
	*/
	void APIENTRY OutputGLDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam)
	{
		std::cerr << message << "\n";
	}

	/**
	* GLFW����̃G���[�񍐂���������.
	*
	* @param error �G���[�ԍ�.
	* @param desc  �G���[�̓��e.
	*/
	void ErrorCallback(int error, const char* desc)
	{
		std::cerr << "ERROR: " << desc << std::endl;
	}

	/**
	* �V���O���g���C���X�^���X���擾����.
	*
	* @return Window�̃V���O���g���C���X�^���X.
	*/
	Window& Window::Instance()
	{
		static Window instance;
		return instance;
	}

	/**
	* �f�X�g���N�^.
	*/
	Window::~Window()
	{
		if (isGLFWInitialized) {
			glfwTerminate();
		}
	}

	/**
	* GLFW/GLEW�̏�����.
	*
	* @param w �E�B���h�E�̕`��͈͂̕�(�s�N�Z��).
	* @param h �E�B���h�E�̕`��͈͂̍���(�s�N�Z��).
	* @param title �E�B���h�E�^�C�g��(UTF-8��0�I�[������).
	*
	* @retval true ����������.
	* @retval false ���������s.
	*/
	bool Window::Init(int w, int h, const char* title)
	{
		if (isInitialized) {
			std::cerr << "ERROR: GLFWEW�͊��ɏ���������Ă��܂�." << std::endl;
			return false;
		}
		if (!isGLFWInitialized) {
			glfwSetErrorCallback(ErrorCallback);
			if (glfwInit() != GL_TRUE) {
				return false;
			}
			isGLFWInitialized = true;
		}

		if (!window) {
			window = glfwCreateWindow(w, h, title, nullptr, nullptr);
			if (!window) {
				return false;
			}
			glfwMakeContextCurrent(window);
		}

		if (glewInit() != GLEW_OK) {
			std::cerr << "ERROR: GLEW�̏������Ɏ��s���܂���." << std::endl;
			return false;
		}

		for (auto& e : keyState) {
			e = KeyState::release;
		}

		width = w;
		height = h;
		glDebugMessageCallback(OutputGLDebugMessage, nullptr);

		const GLubyte* renderer = glGetString(GL_RENDERER);
		std::cout << "Renderer: " << renderer << "\n";
		const GLubyte* version = glGetString(GL_VERSION);
		std::cout << "Version: " << version << "\n";
		GLint offsetAlignment = 0;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &offsetAlignment);
		std::cout << "GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT: " << offsetAlignment << " Bytes\n";
		GLint maxVertexUniformComponents = 0;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniformComponents);
		std::cout << "GL_MAX_VERTEX_UNIFORM_COMPONENTS: " << maxVertexUniformComponents / 1024 << " KBytes\n";
		GLint maxFragmentUniformComponents = 0;
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniformComponents);
		std::cout << "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS: " << maxFragmentUniformComponents / 1024 << " KBytes\n";
		GLint maxUniformBlockSize = 0;
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
		std::cout << "GL_MAX_UNIFORM_BLOCK_SIZE: " << maxUniformBlockSize / 1024 / 1024 << " MBytes\n";

		const char* glfwVersion = glfwGetVersionString();
		std::cout << "GLFW Version: " << glfwVersion << "\n";
		const char* glewVersion = reinterpret_cast<const char*>(glewGetString(GLEW_VERSION));
		std::cout << "GLEW Version: " << glewVersion << "\n";

		ResetDeltaTime();

		isInitialized = true;
		return true;
	}

	/**
	*
	*/
	void Window::Update()
	{
		// �L�[��Ԃ̍X�V.
		for (size_t i = 0; i < keyState.size(); ++i) {
			const bool pressed = glfwGetKey(window, i) == GLFW_PRESS;
			if (pressed) {
				if (keyState[i] == KeyState::release) {
					keyState[i] = KeyState::startPress;
				}
				else if (keyState[i] == KeyState::startPress) {
					keyState[i] = KeyState::press;
				}
			}
			else if (keyState[i] != KeyState::release) {
				keyState[i] = KeyState::release;
			}
		}

		UpdateGamePad();

		const double t = glfwGetTime();
		deltaTime = t - prevTime;
		if (deltaTime >= 0.5) {
			deltaTime = 1.0 / 60.0;
		}
		prevTime = t;
	}

	/**
	*
	*/
	void Window::ResetDeltaTime()
	{
		prevTime = glfwGetTime();
		deltaTime = 0;
	}

	/**
	* �L�[�������ꂽ�u�Ԃ����ׂ�.
	*
	* @param key ���ׂ�L�[��ID(GLFW_KEY_A�Ȃ�).
	*
	* @retval true  �L�[�������ꂽ�u��.
	* @retval false �L�[�������ꂽ�u�Ԃł͂Ȃ�.
	*/
	bool Window::KeyDown(int key) const
	{
		if (key < 0 || key >= static_cast<int>(keyState.size())) {
			return false;
		}
		return keyState[key] == KeyState::startPress;
	}

	/**
	* �L�[��������Ă��邩���ׂ�.
	*
	* @param key ���ׂ�L�[��ID(GLFW_KEY_A�Ȃ�).
	*
	* @retval true  �L�[��������Ă���.
	* @retval false �L�[��������Ă��Ȃ�.
	*/
	bool Window::KeyPressed(int key) const
	{
		if (key < 0 || key >= static_cast<int>(keyState.size())) {
			return false;
		}
		return keyState[key] != KeyState::release;
	}

	/**
	* �E�B���h�E�����ׂ������ׂ�.
	*
	* @retval true ����.
	* @retval false ���Ȃ�.
	*/
	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(window) != 0;
	}

	/**
	* �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@��؂�ւ���.
	*/
	void Window::SwapBuffers() const
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	/**
	* �Q�[���p�b�h�̏�Ԃ��擾����.
	*
	* @return �Q�[���p�b�h�̏��.
	*/
	const GamePad& Window::GetGamePad() const
	{
		return gamepad;
	}



	/**
	* �}�E�X�̍��W���擾����.
	*
	* @return �E�B���h�E�̍������(0,0)�Ƃ����}�E�X�̍��W.
	*/
	glm::vec2 Window::MousePosition() const
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return glm::vec2(x, y);
	}

	/**
	* �}�E�X�J�[�\�����\���ɂ���.
	*
	* �J�[�\���ړ����́A���������擾�\.
	*/
	void Window::DisableMouseCursor()
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	/**
	* �}�E�X�J�[�\����\������.
	*/
	void Window::EnableMouseCursor()
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	/**
	* �Q�[���p�b�h�̃A�i���O���͑��uID.
	*
	* @note ������Xbox360�Q�[���p�b�h�.
	*/
	enum GAMEPAD_AXES
	{
		GAMEPAD_AXES_LEFT_X,	///< ���X�e�B�b�N��X��.
		GAMEPAD_AXES_LEFT_Y,	///< ���X�e�B�b�N��Y��.
		GAMEPAD_AXES_TRIGGER,	///< �A�i���O�g���K�[.
		GAMEPAD_AXES_RIGHT_Y,	///< �E�X�e�B�b�N��Y��.
		GAMEPAD_AXES_RIGHT_X,	///< �E�X�e�B�b�N��X��.
	};

	/**
	* �Q�[���p�b�h�̃f�W�^�����͑��uID.
	*
	* @note Xbox360�Q�[���p�b�h����.
	*/
	enum GAMEPAD_BUTTON {
		GAMEPAD_BUTTON_A, ///< A�{�^��.
		GAMEPAD_BUTTON_B, ///< B�{�^��.
		GAMEPAD_BUTTON_X, ///< X�{�^��.
		GAMEPAD_BUTTON_Y, ///< Y�{�^��.
		GAMEPAD_BUTTON_L, ///< L�{�^��.
		GAMEPAD_BUTTON_R, ///< R�{�^��.
		GAMEPAD_BUTTON_BACK, ///< Back�{�^��.
		GAMEPAD_BUTTON_START, ///< Start�{�^��.
		GAMEPAD_BUTTON_LTHUMB, ///< ���X�e�B�b�N�{�^��.
		GAMEPAD_BUTTON_RTHUMB, ///< �E�X�e�B�b�N�{�^��.
		GAMEPAD_BUTTON_UP, ///< ��L�[.
		GAMEPAD_BUTTON_RIGHT, ///< �E�L�[.
		GAMEPAD_BUTTON_DOWN, ///< ���L�[.
		GAMEPAD_BUTTON_LEFT, ///< ���L�[.
	};

	/**
	* �Q�[���p�b�h�̏�Ԃ��X�V����.
	*/
	void Window::UpdateGamePad()
	{
		// buttonDown�𐶐����邽�߂ɁA�X�V�O�̓��͏�Ԃ�ۑ����Ă���.
		const uint32_t prevButtons = gamepad.buttons;

		// �A�i���O���͂ƃ{�^�����͂��擾.
		int axesCount, buttonCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		const uint8_t* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

		// �����̔z��nullptr�łȂ��A�Œ���K�v�ȃf�[�^���𖞂����Ă���΁A�L���ȃQ�[���p�b�h��
		// �ڑ�����Ă���Ƃ݂Ȃ�.
		if (axes && buttons && axesCount >= 2 && buttonCount >= 8) {
			// �L���ȃQ�[���p�b�h���ڑ�����Ă���ꍇ

			// �����L�[�̓��͏�Ԃ��������āA���X�e�B�b�N�̓��͂Œu��������.
			gamepad.buttons &= 
				~(GamePad::DPAD_UP | GamePad::DPAD_DOWN | GamePad::DPAD_LEFT | GamePad::DPAD_RIGHT);

			static const float digitalThreshold = 0.3f; // �f�W�^�����͂Ƃ݂Ȃ��������l.
			if (axes[GAMEPAD_AXES_LEFT_Y] >= digitalThreshold) {
				gamepad.buttons |= GamePad::DPAD_UP;
			}
			else if (axes[GAMEPAD_AXES_LEFT_Y] <= -digitalThreshold) {
				gamepad.buttons |= GamePad::DPAD_DOWN;
			}
			if (axes[GAMEPAD_AXES_LEFT_X] >= digitalThreshold) {
				gamepad.buttons |= GamePad::DPAD_LEFT;
			}
			else if (axes[GAMEPAD_AXES_LEFT_X] <= -digitalThreshold) {
				gamepad.buttons |= GamePad::DPAD_RIGHT;
			}

			// �z��C���f�b�N�X��GamePad�L�[�̑Ή��\.
			static const struct {
				int dataIndex;
				uint32_t gamepadBit;
			} keyMap[] = {
				{ GAMEPAD_BUTTON_A, GamePad::A },
				{ GAMEPAD_BUTTON_B, GamePad::B },
				{ GAMEPAD_BUTTON_X, GamePad::X },
				{ GAMEPAD_BUTTON_Y, GamePad::Y },
				{ GAMEPAD_BUTTON_L, GamePad::L },
				{ GAMEPAD_BUTTON_R, GamePad::R },
				{ GAMEPAD_BUTTON_START, GamePad::START },
				{ GAMEPAD_BUTTON_UP, GamePad::DPAD_UP },
				{ GAMEPAD_BUTTON_DOWN, GamePad::DPAD_DOWN },
				{ GAMEPAD_BUTTON_LEFT, GamePad::DPAD_LEFT },
				{ GAMEPAD_BUTTON_RIGHT, GamePad::DPAD_RIGHT },
			};

			for (const auto& e : keyMap) {
				if (buttons[e.dataIndex] == GLFW_PRESS) {
					gamepad.buttons |= e.gamepadBit;
				}
				else if (buttons[e.dataIndex] == GLFW_RELEASE) {
					gamepad.buttons &= ~e.gamepadBit;
				}
			}
		}
		else {
			// �L���ȃQ�[���p�b�h���ڑ�����Ă��Ȃ��̂ŁA�L�[�{�[�h���͂ő�p.

			// �z��C���f�b�N�X��GamePad�L�[�̑Ή��\.
			static const struct {
				int keyCode;
				uint32_t gamepadBit;
			} keyMap[] = {
				{ GLFW_KEY_J, GamePad::A },
				{ GLFW_KEY_K,GamePad::B },
				{ GLFW_KEY_U,GamePad::X },
				{ GLFW_KEY_I,GamePad::Y },
				{ GLFW_KEY_O,GamePad::L },
				{ GLFW_KEY_L,GamePad::R },
				{ GLFW_KEY_ENTER,GamePad::START },
				{ GLFW_KEY_W,GamePad::DPAD_UP },
				{ GLFW_KEY_A,GamePad::DPAD_DOWN },
				{ GLFW_KEY_S,GamePad::DPAD_LEFT },
				{ GLFW_KEY_D,GamePad::DPAD_RIGHT },
			};

			for (const auto& e : keyMap) {
				const int key = glfwGetKey(window, e.keyCode);
				if (key == GLFW_PRESS) {
					gamepad.buttons |= e.gamepadBit;
				}
				else if (key == GLFW_RELEASE) {
					gamepad.buttons &= ~e.gamepadBit;
				}
			}
		}

		// �O��̍X�V�ŉ�����ĂȂ��āA���񉟂���Ă���L�[�̏���buttonDown�Ɋi�[.
		gamepad.buttonDown = gamepad.buttons & ~prevButtons;
	}

} // namespace GLFWEW