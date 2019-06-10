/**
* @file GameOverScene.cpp
*/
#include "GameOverScene.h"
#include "TitleScene.h"

/**
* シーンを初期化する.
*
* @retval true	初期化成功.
* @retval false 初期化失敗. ゲーム進行不可につき、プログラムを終了すること.
*/
bool GameOverScene::Initialize()
{
	// スプライトレンダラーを初期化
	spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
	sprites.reserve(100);
	Sprite spr(Texture::Image2D::Create("Res/Smash3D_Result.tga"));
	spr.Scale(glm::vec2(2));
	sprites.push_back(spr);

	// フォントレンダラーを初期化
	fontRenderer.Init(1000);
	fontRenderer.LoadFromFile("Res/font.fnt");

	return true;
}


/**
* プレイヤーの入力を処理する.
*/
void GameOverScene::ProcessInput() 
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (window.GetGamePad().buttonDown & GamePad::START) {
		SceneStack::Instance().Replace(std::make_shared<TitleScene>());
	}
}

/**
* シーンを更新する.
*
* @param deltaTime	前回の更新からの経過時間(秒).
*/
void GameOverScene::Update(float deltaTime)
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
	fontRenderer.AddString(glm::vec2(-w * 0.5f + 32, h * 0.5f - lineHeight), L"ゲームオーバー画面");
	fontRenderer.AddString(glm::vec2(-128, 0), L"ゲームオーバー");
	fontRenderer.EndUpdate();
}

/**
* シーンを描画する.
*/
void GameOverScene::Render()
{
	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	spriteRenderer.Draw(screenSize);
	fontRenderer.Draw(screenSize);
}

/*
* 終了処理
*/
void GameOverScene::Finalize()
{
	std::cout << "Finalize GameOverScene" << std::endl;
}