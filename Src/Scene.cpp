/*
* @file Scene.cpp
*
*/

#include "Scene.h"

/*
* �R���X�g���N�^
*
* @param name �V�[����
*/

Scene::Scene(std::string name) :name(name) {}

/*
* �f�X�g���N�^
*/
Scene::~Scene()
{
	Finalize();
}

/*
* �V�[����������Ԃɂ���.
*/
void Scene::Play()
{
	isActive = true;
}

/*
* �V�[�����~��Ԃɂ���.
*/
void Scene::Stop()
{
	isActive = false;
}
/*
* �V�[����\������
*/
void Scene::Show()
{
	isVisible = true;
}

/*
* �V�[�����\���ɂ���.
*/
void Scene::Hide()
{
	isVisible = false;

}

/*
* �V�[�������擾����
*/
const std::string& Scene::Name()const
{
	return name;
}


/*
* �V�[���̊�����Ԃ𒲂ׂ�
*
* return true  �������Ă���
* return false �������Ă��Ȃ�
*/
bool Scene::IsActive()const
{
	return isActive;
}


/*
* �V�[���̕\����Ԃ𒲂ׂ�.
*
* @return true  �\�����
* @return false ��\�����.
*
*/
bool Scene::IsVisible()const
{
	return isVisible;
}



/*
* �V�[���X�^�b�N���擾����
*
* @return �V�[���X�^�b�N.
*/
SceneStack& SceneStack::Instance()
{
	static SceneStack instance;
	return instance;
}

/*
* �R���X�g���N�^
*
*
*/
SceneStack::SceneStack()
{
	stack.reserve(16);
}

/*
* �V�[�����v�b�V������.
*
* @param p �V�����V�[��.
*
*
*/
void SceneStack::Push(ScenePtr p)
{
	if (!stack.empty()) {
		Current().Stop();
	}
	stack.push_back(p);
	std::cout << "[Push Scene]" << p->Name() << std::endl;
	Current().Initialize();
	Current().Play();
}

/*
* �V�[�����|�b�v����
*/
void SceneStack::Pop()
{
	if (stack.empty()) {
		std::cout << "[Pop Scene][Warning]Stack Empty" << std::endl;
		return;
	}
	Current().Stop();
	Current().Finalize();

	const std::string sceneName = Current().Name();
	stack.pop_back();
	std::cout << "[Pop Scene]" << sceneName << std::endl;
	if (!stack.empty()) {
		Current().Play();
	}
}

/*
* �V�[����u��������.
*
* @param p �V�����V�[��
*/
void SceneStack::Replace(ScenePtr p)
{
	std::string sceneName = "(Empty)";
	if (stack.empty()) {
		std::cout << "[Scene Replace][Warning]Stack Empty" << std::endl;
	}
	else
	{
		sceneName = Current().Name();
		Current().Stop();
		Current().Finalize();
		stack.pop_back();
	}
	stack.push_back(p);
	std::cout << "[Scene Replace]" << sceneName << "->" << p->Name() << std::endl;
	Current().Initialize();
	Current().Play();
}

/*
* ���݂̃V�[�����擾����
*
* return ���݂̃V�[��.
*/
Scene& SceneStack::Current()
{
	return *stack.back();
}
/*
* ���݂̃V�[�����擾����.
*
* @return ���݂̃V�[��.
*/
const Scene& SceneStack::Current()const
{
	return *stack.back();
}

/*
* �V�[���̐����擾����.
*
* @return �X�^�b�N�ɐς܂�Ă���V�[���̐�.
*/
size_t SceneStack::Size()const
{
	return stack.size();
}

/*
* �X�^�b�N���󂩂ǂ����𒲂ׂ�.
*
* @retval true  �X�^�b�N�͋�.
* @retval false �X�^�b�N�Ɉ�ȏ�̃V�[�����ς܂�Ă���
*/
bool SceneStack::Empty()const
{
	return stack.empty();
}



/*
* �V�[�����X�V����.
*
*
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b).
*/
void SceneStack::Update(float deltaTime)
{
	this->Current().ProcessInput();
	for (ScenePtr& e : stack) {
		e->Update(deltaTime);
	}
}
/*
* �V�[����`�悷��.
*/
void SceneStack::Render()
{
	for (ScenePtr& e : stack) {
		if (e->IsVisible()) {
			e->Render();
		}
	}
}