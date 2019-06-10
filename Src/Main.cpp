/**
* @file Main.cpp
*/
#include "TitleScene.h"
#include "GLFWEW.h"
#include <Windows.h>


int main()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (!window.Init(
		1280, 720, u8"�A�N�V�����Q�[��")) {
		std::cerr << "ERORR>>Main.cpp:Failed to Initialize GLFWEW" << std::endl;
		return 1;
	}

	SceneStack& sceneStack = SceneStack::Instance();
	sceneStack.Push(std::make_shared<TitleScene>());
	while (!window.ShouldClose()) {
		window.Update();

		// ESC�L�[�������ꂽ��I���E�B���h�E��\��.
		if (window.KeyDown(GLFW_KEY_ESCAPE)) {
			if (MessageBox(nullptr, "�Q�[�����I�����܂����H", "�I��", MB_OKCANCEL) == IDOK)
			{
				break;
			}
		}
		const float deltaTime = window.DeltaTime();
		sceneStack.Update(deltaTime);

		// �o�b�N�o�b�t�@����������.
		glClearColor(0.8f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// GL�R���e�L�X�g�̃p�����[�^��ݒ�.
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		sceneStack.Render();
		window.SwapBuffers();
	}
}



































//
////�錾
//int stack[10] = {};
//int N = 0;
//
//void push(int);
//int pop();
//
//int main() {
//	push(10);
//	push(30);
//	std::cout << pop() << "\n";
//	push(20);
//	push(100);
//	pop();
//	std::cout << pop() << "\n";
//	std::cout << pop() << "\n";
//
//	int a;
//	std::cin >> a;
//
//	return 0;
//} 
//
////��`
///*
//* �X�^�b�N�ɒl���v�b�V������.
//*/
//void push(int data) {
//	if (N < sizeof(stack) / sizeof(stack[0])) {
//		stack[N] = data;
//		N++;
//	}
//}
//
///*
//* �X�^�b�N����l���|�b�v����.
//*/
//int pop() {
//	int tmp = -1;
//	if (N > 0) {
//		tmp = stack[N - 1];
//		// �Ȃ��Ă悢�B�|�C���^�Ȃ�K�v�@stack[N - 1] = 0; 
//		N--;
//	}
//	return tmp;
//}