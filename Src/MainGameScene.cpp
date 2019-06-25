/**
* @file MainGameScene.cpp
*/
#include "MainGameScene.h"
#include "StatusScene.h"
#include "GameOverScene.h"
#include <iostream>


/**
* �V�[��������������.
*
* @retval true	����������.
* @retval false ���������s. �Q�[���i�s�s�ɂ��A�v���O�������I�����邱��.
*/
bool MainGameScene::Initialize()
{
	// �X�v���C�g�����_���[��������
	spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
	sprites.reserve(100);
	Sprite spr(Texture::Image2D::Create("Res/Smash3D_MainGame.tga"));
	spr.Scale(glm::vec2(2));
	sprites.push_back(spr);
	meshBuffer.Init(1'000'000 * sizeof(Mesh::Vertex), 3'000'000 * sizeof(GLushort));

	// �n�C�g�}�b�v���쐬����.
	if (!heightMap.LoadFromFile("Res/Terrain.tga", 20.0f, 0.5f))
	{
		return false;
	}
	if (!heightMap.CreateMesh(meshBuffer, "Terrain"))
	{
		return false;
	}

	// �t�H���g�����_���[��������
	fontRenderer.Init(1000);
	fontRenderer.LoadFromFile("Res/font.fnt");

	return true;
}


/**
* �v���C���[�̓��͂���������.
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
	
	// �J��������.
	const GamePad gamepad = window.GetGamePad();
	glm::vec3 velocity(0);
	if (gamepad.buttons & GamePad::DPAD_LEFT) {
		velocity.x = -1;
	}
	else if (gamepad.buttons & GamePad::DPAD_RIGHT)
	{
		velocity.x = 1;
	}
	if (gamepad.buttons & GamePad::DPAD_DOWN) {
		velocity.z = 1;
	}
	else if (gamepad.buttons & GamePad::DPAD_UP) {
		velocity.z = -1;
	}
	if (velocity.x || velocity.z) {
		velocity = normalize(velocity) * 20.0f;
	}
	camera.velocity = velocity;
}

/**
* �V�[�����X�V����.
*
* @param deltaTime	�O��̍X�V����̌o�ߎ���(�b).
*/
void MainGameScene::Update(float deltaTime)
{
	spriteRenderer.BeginUpdate();
	for (const Sprite& e : sprites) {
		spriteRenderer.AddVertices(e);
	}
	spriteRenderer.EndUpdate();

	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	//const float deltaTime2 = window.DeltaTime();

	// �J�����̏�Ԃ��X�V.
	if (dot(camera.velocity, camera.velocity)) {
		camera.target += camera.velocity * deltaTime;
		camera.target.y = heightMap.Height(camera.target);
		camera.position = camera.target + glm::vec3(0, 100, 100);
	}
	

	const float w = window.Width();
	const float h = window.Height();
	const float lineHeight = fontRenderer.LineHeight();
	fontRenderer.BeginUpdate();
	fontRenderer.Color(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	fontRenderer.AddString(glm::vec2(-w * 0.5f + 32, h * 0.5f - lineHeight), L"���C���Q�[�����");
	fontRenderer.AddString(glm::vec2(-128, 0), L"���C���Q�[��");
	fontRenderer.EndUpdate();
}

/**
* �V�[����`�悷��.
*/
void MainGameScene::Render()
{
	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	spriteRenderer.Draw(screenSize);
	fontRenderer.Draw(screenSize);

	const glm::mat4 matView = glm::lookAt(camera.position, camera.target, camera.up);
	const float aspectRatio =
		static_cast<float>(window.Width()) / static_cast<float>(window.Height());
	const glm::mat4 matProj =
		glm::perspective(glm::radians(30.0f), aspectRatio, 1.0f, 1000.0f);
	//const glm::mat4 matModel(1);
	glm::vec3 cubePos(100, 200, 100);
	cubePos.y = heightMap.Height(cubePos);
	const glm::mat4 matModel = glm::translate(glm::mat4(1), cubePos);
	Mesh::Draw(meshBuffer.GetFile("Cube"), matProj * matView, matModel);
	Mesh::Draw(meshBuffer.GetFile("Terrain"), matProj * matView, glm::mat4(1));
}


/*
* �I������
*/
void MainGameScene::Finalize()
{
	std::cout << "Finalize MainGameScene" << std::endl;
}