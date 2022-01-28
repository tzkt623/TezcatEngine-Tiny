#pragma once
#include <string>

namespace tezcat::Tiny::Module
{
	class Material;
	class GameObject;
	class Camera;
}

namespace tezcat::Tiny::Core
{
	class Shader;
	class Mesh;
	class MeshData;
	class RenderAgent
	{
	public:
		RenderAgent();
		~RenderAgent();

		void apply();
		void render(Shader* shader);

	public:
		Module::GameObject* getMasterObject() const { return m_MasterObject; }
		void setMasterObject(Module::GameObject* val) { m_MasterObject = val; }

	public:
		void setMaterial(Module::Material* material) { m_Material = material; }
		Module::Material* getMaterial() const { return m_Material; }

	public:
		void setMeshData(MeshData* meshData);
		void setMesh(Mesh* mesh) { m_Mesh = mesh; }
		void setMesh(const std::string& name);
		Mesh* getMesh() const { return m_Mesh; }

	public:
		bool setEnable() const { return m_Enable; }
		void getEnable(bool val) { m_Enable = val; }

	private:
		bool m_Enable;
		Module::GameObject* m_MasterObject;

	protected:
		Mesh* m_Mesh;
		Module::Material* m_Material;
	};

}