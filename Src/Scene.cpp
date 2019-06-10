/*
* @file Scene.cpp
*
*/

#include "Scene.h"

/*
* コンストラクタ
*
* @param name シーン名
*/

Scene::Scene(std::string name) :name(name) {}

/*
* デストラクタ
*/
Scene::~Scene()
{
	Finalize();
}

/*
* シーンを活動状態にする.
*/
void Scene::Play()
{
	isActive = true;
}

/*
* シーンを停止状態にする.
*/
void Scene::Stop()
{
	isActive = false;
}
/*
* シーンを表示する
*/
void Scene::Show()
{
	isVisible = true;
}

/*
* シーンを非表示にする.
*/
void Scene::Hide()
{
	isVisible = false;

}

/*
* シーン名を取得する
*/
const std::string& Scene::Name()const
{
	return name;
}


/*
* シーンの活動状態を調べる
*
* return true  活動している
* return false 活動していない
*/
bool Scene::IsActive()const
{
	return isActive;
}


/*
* シーンの表示状態を調べる.
*
* @return true  表示状態
* @return false 非表示状態.
*
*/
bool Scene::IsVisible()const
{
	return isVisible;
}



/*
* シーンスタックを取得する
*
* @return シーンスタック.
*/
SceneStack& SceneStack::Instance()
{
	static SceneStack instance;
	return instance;
}

/*
* コンストラクタ
*
*
*/
SceneStack::SceneStack()
{
	stack.reserve(16);
}

/*
* シーンをプッシュする.
*
* @param p 新しいシーン.
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
* シーンをポップする
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
* シーンを置き換える.
*
* @param p 新しいシーン
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
* 現在のシーンを取得する
*
* return 現在のシーン.
*/
Scene& SceneStack::Current()
{
	return *stack.back();
}
/*
* 現在のシーンを取得する.
*
* @return 現在のシーン.
*/
const Scene& SceneStack::Current()const
{
	return *stack.back();
}

/*
* シーンの数を取得する.
*
* @return スタックに積まれているシーンの数.
*/
size_t SceneStack::Size()const
{
	return stack.size();
}

/*
* スタックが空かどうかを調べる.
*
* @retval true  スタックは空.
* @retval false スタックに一つ以上のシーンが積まれている
*/
bool SceneStack::Empty()const
{
	return stack.empty();
}



/*
* シーンを更新する.
*
*
* @param deltaTime 前回の更新からの経過時間(秒).
*/
void SceneStack::Update(float deltaTime)
{
	this->Current().ProcessInput();
	for (ScenePtr& e : stack) {
		e->Update(deltaTime);
	}
}
/*
* シーンを描画する.
*/
void SceneStack::Render()
{
	for (ScenePtr& e : stack) {
		if (e->IsVisible()) {
			e->Render();
		}
	}
}