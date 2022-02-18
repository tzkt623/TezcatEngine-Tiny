#pragma once
#include "Component.h"
#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"

namespace tezcat::Tiny::Core
{
	class Scene;
	class Transform : public ComponentT<Transform>
	{
	public:
		Transform();
		Transform(Transform* parent);
		~Transform();

	public:
		glm::vec3& getPosition() { return m_Position; }
		void setPosition(const glm::vec3& val)
		{
			m_IsDirty = true;
			m_Position = val;
		}

		void setPosition(float val[])
		{
			m_IsDirty = true;
			m_Position.x = val[0];
			m_Position.y = val[1];
			m_Position.z = val[2];
		}

		glm::vec3& getRotation() { return m_Rotation; }
		void setRotation(const glm::vec3& val)
		{
			m_IsDirty = true;
			m_Rotation = val;
		}

		void setRotation(float x, float y, float z)
		{
			m_IsDirty = true;
			m_Rotation.x = x;
			m_Rotation.y = y;
			m_Rotation.z = z;
		}

		glm::vec3& getScale() { return m_Scale; }
		void setScale(const glm::vec3& val)
		{
			m_IsDirty = true;
			m_Scale = val;
		}

		inline glm::mat4x4& getModelMatrix() { return m_ModelMatrix; }

		Transform* getParent() const { return m_Parent; }
		void setParent(Transform* parent);

		void addChild(Transform* val);
		bool removeChild(Transform* val);
		size_t getChildCount() { return m_Children.size(); }
		std::vector<Transform*>& getChildren() { return m_Children; }

	public:
		void update();

	public:
		void enterScene(Scene* scene);
		void exitScene();

	public:
		void translate(const glm::vec3& offset)
		{
			m_IsDirty = true;
			m_Position += offset;
		}

	private:
		unsigned int m_Index;
		Transform* m_Parent;
		std::vector<Transform*> m_Children;

	private:
		bool m_IsDirty;
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		glm::mat4x4 m_ModelMatrix;

	private:
		friend class Scene;
	};


	class TransformCollection
	{
	public:
		TransformCollection();
		~TransformCollection();

		Transform* giveTransform(int id);
		Transform* get(int id);

	private:
		std::vector<Transform> m_TransformList;
	};
}