#pragma once
#include "CommonHead.h"
#include "RenderObject.h"

namespace tezcat::Tiny::Core
{
	class Shader;
	class Light : public RenderObject
	{

	public:
		Light();
		~Light();

		void render(Shader* shader) override;
		void apply() override;

		glm::vec3 getColor() const { return m_Color; }
		void setColor(const glm::vec3& val) { m_Color = val; }

	private:
		std::string m_Name;
		glm::vec3 m_Color;
	};
}