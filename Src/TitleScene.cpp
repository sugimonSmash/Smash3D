/**
* @file TitleScene.cpp
*/
#include "TitleScene.h"
#include "MainGameScene.h"


/**
* �V�[��������������.
*
* @retval true	����������.
* @retval false ���������s. �Q�[���i�s�s�ɂ��A�v���O�������I�����邱��.
*/
bool TitleScene::Initialize()
{
	// �X�v���C�g�����_���[��������
	spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
	sprites.reserve(100);
	Sprite spr(Texture::Image2D::Create("Res/Smash3D_Title.tga"));
	spr.Scale(glm::vec2(2));
	sprites.push_back(spr);

	// �t�H���g�����_���[��������
	fontRenderer.Init(1000);
	fontRenderer.LoadFromFile("Res/font.fnt");

	return true;
}

/**
* �v���C���[�̓��͂���������.
*/
void TitleScene::ProcessInput() {
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (window.GetGamePad().buttonDown & GamePad::START) {
		SceneStack::Instance().Replace(std::make_shared<MainGameScene>());
	}
}

/**
* �V�[�����X�V����.
*
* @param deltaTime	�O��̍X�V����̌o�ߎ���(�b).
*/
void TitleScene::Update(float deltaTime)
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
	fontRenderer.Scale(glm::vec2(2, 2));
	fontRenderer.AddString(glm::vec2(-w * 0.5f + 32, h * 0.5f - lineHeight), L"�^�C�g�����");
	fontRenderer.AddString(glm::vec2(-128, 0), L"�^�C�g��");
	fontRenderer.EndUpdate();
}

/**
* �V�[����`�悷��.
*/
void TitleScene::Render()
{
	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	spriteRenderer.Draw(screenSize);
	fontRenderer.Draw(screenSize);

}

/*
* �I������
*/
void TitleScene::Finalize()
{
	std::cout << "Finalize TitleScene" << std::endl;
}