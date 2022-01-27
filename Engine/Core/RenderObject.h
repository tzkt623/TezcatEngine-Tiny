#pragma once
#include "RCVAO.h"

namespace tezcat::Tiny::Module
{
	class GameObject;
	class Material;
}

namespace tezcat::Tiny::Core
{
	class Shader;
	class RenderObject
	{
	public:
		RenderObject();
		virtual ~RenderObject();

		virtual void draw(Shader* shader) = 0;
		virtual void apply() = 0;

		Module::GameObject* getMasterObject() const { return m_MasterObject; }
		void setMasterObject(Module::GameObject* val) { m_MasterObject = val; }
		void setMaterial(Module::Material* material)
		{
			m_Material = material;
		}


		bool setEnable() const { return m_Enable; }
		void getEnable(bool val) { m_Enable = val; }

	private:
		bool m_Enable;

	protected:
		Module::GameObject* m_MasterObject;
		Module::Material* m_Material;
	};


	class RCVAO;
	class ROMesh : public RenderObject
	{
	public:
		ROMesh();
		ROMesh(Mesh* mesh);

		virtual ~ROMesh();

		void draw(Shader* shader) override;
		void apply() override;

	private:
		RCVAO* m_VAO;
		Mesh* m_Mesh;
	};
}


