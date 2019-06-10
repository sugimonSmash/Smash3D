/**
* @file GLFWEW.h
*/
#ifndef GLFWEW_H_INCLUDED
#define GLFWEW_H_INCLUDED

#include <GL/glew.h>
#include "GamePad.h"
#include <GLFW/glfw3.h>
#include <array>
#include <glm/vec2.hpp>

///**
//* �Q�[���p�b�h���.
//*/
//struct GamePad
//{
//	static const uint32_t DPAD_UP = 0x0001;
//	static const uint32_t DPAD_DOWN = 0x0002;
//	static const uint32_t DPAD_LEFT = 0x0004;
//	static const uint32_t DPAD_RIGHT = 0x0008;
//	static const uint32_t START = 0x0010;
//	static const uint32_t A = 0x0020;
//	static const uint32_t B = 0x0040;
//	static const uint32_t X = 0x0080;
//	static const uint32_t Y = 0x0100;
//	static const uint32_t L = 0x0200;
//	static const uint32_t R = 0x0400;
//
//	uint32_t buttons = 0; ///< ������Ă��邠�����t���O������.
//	uint32_t buttonDown = 0; ///< �����ꂽ�u�Ԃ����t���O������.
//};

namespace GLFWEW {

	/**
	* GLFW��GLEW�̃��b�p�[�N���X.
	*/
	class Window
	{
	public:
		static Window& Instance();
		bool Init(int w, int h, const char* title);
		void Update();
		bool ShouldClose() const;
		void SwapBuffers() const;
		float Width() const { return width *2; }
		float Height() const { return height *2; }
		void ResetDeltaTime();
		double DeltaTime() const { return deltaTime; }
		bool KeyDown(int key) const;
		bool KeyPressed(int key) const;
		const GamePad& GetGamePad() const;

		glm::vec2 MousePosition() const;
		void DisableMouseCursor();
		void EnableMouseCursor();

	private:
		Window() = default;
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		void UpdateGamePad();

		bool isGLFWInitialized = false;
		bool isInitialized = false;
		GLFWwindow* window = nullptr;
		float width = 0;
		float height = 0;
		double prevTime = 0;
		double deltaTime = 0;
		GamePad gamepad;

		enum class KeyState : char {
			release,
			startPress,
			press,
		};
		std::array<KeyState, GLFW_KEY_LAST + 1> keyState = { KeyState::release };

	};

} // namespace GLFWEW

#endif // GLFWEW_H_INCLUDED