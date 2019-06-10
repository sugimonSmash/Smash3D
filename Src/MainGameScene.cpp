/**
* @file MainGameScene.cpp
*/
#include "MainGameScene.h"
#include "StatusScene.h"
#include "GameOverScene.h"
#include <iostream>


/**
* シーンを初期化する.
*
* @retval true	初期化成功.
* @retval false 初期化失敗. ゲーム進行不可につき、プログラムを終了すること.
*/
bool MainGameScene::Initialize()
{
	// スプライトレンダラーを初期化
	spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
	sprites.reserve(100);
	Sprite spr(Texture::Image2D::Create("Res/Smash3D_MainGame.tga"));
	spr.Scale(glm::vec2(2));
	sprites.push_back(spr);
	meshBuffer.Init(1'000'000 * sizeof(Mesh::Vertex), 3'000'000 * sizeof(GLushort));

	// フォントレンダラーを初期化
	fontRenderer.Init(1000);
	fontRenderer.LoadFromFile("Res/font.fnt");

	return true;
}


/**
* プレイヤーの入力を処理する.
*/
void MainGameScene::ProcessInput()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (window.GetGamePad().buttonDown & GamePad::START) {
		if (!flag) {
			flag = true;
			SceneStack::Instance().Push(std::make_shared<StatusScene>());
		}
		else {
			SceneStack::Instance().Replace(std::make_shared<GameOverScene>());
		}
	}
}

/**
* シーンを更新する.
*
* @param deltaTime	前回の更新からの経過時間(秒).
*/
void MainGameScene::Update(float deltaTime)
{
	spriteRenderer.BeginUpdate();
	for (const Sprite& e : sprites) {
		spriteRenderer.AddVertices(e);
	}
	spriteRenderer.EndUpdate();

	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const float w = window.Width();
	const float h = window.Height();
	const float lineHeight = fontRenderer.LineHeight();
	fontRenderer.BeginUpdate();
	fontRenderer.Color(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	fontRenderer.AddString(glm::vec2(-w * 0.5f + 32, h * 0.5f - lineHeight), L"メインゲーム画面");
	fontRenderer.AddString(glm::vec2(-128, 0), L"メインゲーム");
	fontRenderer.EndUpdate();
}

/**
* シーンを描画する.
*/
void MainGameScene::Render()
{
	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	spriteRenderer.Draw(screenSize);
	fontRenderer.Draw(screenSize);


	const glm::vec3 cameraPos(-5, 3, 5);
	const glm::vec3 targetPos(0, 0, 0);
	const glm::mat4 matView = glm::lookAt(cameraPos, targetPos, glm::vec3(0, 1, 0));
	const float aspectRatio =
		static_cast<float>(window.Width()) / static_cast<float>(window.Height());
	const glm::mat4 matProj =
		glm::perspective(glm::radians(30.0f), aspectRatio, 1.0f, 1000.0f);
	const glm::mat4 matModel(1);
	Mesh::Draw(meshBuffer.GetFile("Cube"), matProj * matView, matModel);
}


/*
* 終了処理
*/
void MainGameScene::Finalize()
{
	std::cout << "Finalize MainGameScene" << std::endl;
}