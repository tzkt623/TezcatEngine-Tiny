#include "Transform.h"
#include "../Manager/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/GameObject.h"
#include "Utility/Tools.h"

namespace tezcat::Tiny::Core
{
	static glm::mat4 WORLD_MATRIX(1.0f);
	static glm::vec3 XAxis(1.0f, 0.0f, 0.0f);
	static glm::vec3 YAxis(0.0f, 1.0f, 0.0f);
	static glm::vec3 ZAxis(0.0f, 0.0f, 1.0f);

	Transform::Transform()
		: Transform(nullptr)
	{

	}

	Transform::Transform(Transform* parent)
		: m_IsDirty(true)
		, m_Position(0.0f)
		, m_Rotation(0.0f)
		, m_Scale(1.0f)
		, m_ModelMatrix(1.0f)
		, m_Parent(parent)
		, m_Index(-1)
	{
		this->setTransform(this);
		if (m_Parent != nullptr)
		{
			m_Parent->addChild(this);
		}
	}

	Transform::~Transform()
	{
		m_Parent = nullptr;
	}

	void Transform::update()
	{
		if (m_IsDirty)
		{
			m_IsDirty = false;

			if (m_Parent == nullptr)
			{
				m_ModelMatrix = WORLD_MATRIX;
			}
			else
			{
				m_ModelMatrix = m_Parent->getModelMatrix();
			}

			m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
			// 			m_ModelMatrix = glm::rotate(m_ModelMatrix, m_Rotation.x, XAxis);
			// 			m_ModelMatrix = glm::rotate(m_ModelMatrix, m_Rotation.y, YAxis);
			// 			m_ModelMatrix = glm::rotate(m_ModelMatrix, m_Rotation.z, ZAxis);

			m_ModelMatrix *= glm::toMat4(glm::quat(glm::radians(m_Rotation)));
			m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
		}

		if (!m_Children.empty())
		{
			auto it = m_Children.begin();
			while (it != m_Children.end())
			{
				auto go = (*it)->getGameObject();
				if (go->needDelete())
				{
					go->exitScene();
					it = m_Children.erase(it);
				}
				else
				{
					(*it++)->update();
				}
			}
		}
	}

	void Transform::enterScene(Scene* scene)
	{

	}

	void Transform::exitScene()
	{

	}

	void Transform::addChild(Transform* val)
	{
		val->m_Index = static_cast<unsigned int>(m_Children.size());
		m_Children.push_back(val);
	}

	bool Transform::removeChild(Transform* val)
	{
		auto index = val->m_Index;
		if (m_Children[index] != val)
		{
			return false;
		}

		m_Children.erase(m_Children.begin() + index);

		for (int i = index; i < m_Children.size(); i++)
		{
			m_Children[i]->m_Index = i;
		}

		return true;
	}

	void Transform::setParent(Transform* parent)
	{
		if (m_Parent != nullptr)
		{
			m_Parent->removeChild(this);
		}

		m_Parent = parent;

		if (m_Parent != nullptr)
		{
			m_Parent->addChild(this);
		}
		else
		{
			SG<SceneManager>::getInstance()->getCurrentScene()->addTransform(this);
		}
	}


	//-----------------------------------------------------
	TransformCollection::TransformCollection()
	{

	}

	TransformCollection::~TransformCollection()
	{

	}
	Transform* TransformCollection::giveTransform(int id)
	{
		if (id > m_TransformList.size())
		{
			m_TransformList.resize(id + 1);
		}

		return nullptr;
	}
	Transform* TransformCollection::get(int id)
	{
		return &m_TransformList[id];
	}
}