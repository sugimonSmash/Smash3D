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
		1280, 720, u8"アクションゲーム")) {
		std::cerr << "ERORR>>Main.cpp:Failed to Initialize GLFWEW" << std::endl;
		return 1;
	}

	SceneStack& sceneStack = SceneStack::Instance();
	sceneStack.Push(std::make_shared<TitleScene>());
	while (!window.ShouldClose()) {
		window.Update();

		// ESCキーが押されたら終了ウィンドウを表示.
		if (window.KeyDown(GLFW_KEY_ESCAPE)) {
			if (MessageBox(nullptr, "ゲームを終了しますか？", "終了", MB_OKCANCEL) == IDOK)
			{
				break;
			}
		}
		const float deltaTime = window.DeltaTime();
		sceneStack.Update(deltaTime);

		// バックバッファを消去する.
		glClearColor(0.8f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// GLコンテキストのパラメータを設定.
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		sceneStack.Render();
		window.SwapBuffers();
	}
}



































//
////宣言
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
////定義
///*
//* スタックに値をプッシュする.
//*/
//void push(int data) {
//	if (N < sizeof(stack) / sizeof(stack[0])) {
//		stack[N] = data;
//		N++;
//	}
//}
//
///*
//* スタックから値をポップする.
//*/
//int pop() {
//	int tmp = -1;
//	if (N > 0) {
//		tmp = stack[N - 1];
//		// なくてよい。ポインタなら必要　stack[N - 1] = 0; 
//		N--;
//	}
//	return tmp;
//}