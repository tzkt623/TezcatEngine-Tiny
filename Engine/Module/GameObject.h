#pragma once
#include "CommonHead.h"
#include <vector>
#include <string>
#include <functional>

namespace tezcat::Tiny::Core
{
	class RenderObject;
	class Mesh;
}

namespace tezcat::Tiny::Module
{
	class Transform;
	class Material;
	class Scene;
	class UserObject;
	class GameObject
	{
	public:
		GameObject();
		GameObject(const std::string& name);

		virtual ~GameObject();

	public:

		void apply();

		/// <summary>
		/// 开始每帧运行
		/// </summary>
		void startLogic();

		/// <summary>
		/// 停止每帧运行
		/// </summary>
		void stopLogic();

		virtual void update();

		std::string getName() const { return m_Name; }
		void setName(std::string val) { m_Name = val; }

		virtual void sceneEnter(Scene* scene);
		virtual void sceneExit(Scene* scene);

	protected:
		virtual void onApply();
		virtual void logic();

	public:
		void addChild(GameObject* gameObject);

		void setMaterial(Material* material);
		Material* getMaterial() const { return m_Material; }

		void setMesh(Core::Mesh* mesh);
		void setRenderObject(Core::RenderObject* renderObject);
		Core::RenderObject* getRenderObject() const { return m_RenderObject; }

		Transform* getTransform() const { return m_Transform; }

		void foreachChild(const std::function<void()>& begin,
			const std::function<void(GameObject*)>& get,
			const std::function<void()>& end,
			const std::function<void()>& nochildren);


	private:
		std::string m_Name;
		bool m_IsLogic;

		Transform* m_Transform;
		Material* m_Material;
		Core::RenderObject* m_RenderObject;
		std::vector<GameObject*> m_Children;

		Scene* m_Scene;

	private:
		UserObject* m_UserObject;
	};

	class UserObject
	{
	public:
		UserObject() = default;
		virtual ~UserObject() = 0;

	private:

	};
}

