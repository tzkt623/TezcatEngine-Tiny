#pragma once
#include "ShaderBuilder.h"
#include <vector>
#include <string>
#include "Matrix.h"

namespace tezcat::Tiny::Core
{
	class Shader
	{
	public:
		Shader(const std::string& name, int orderID);
		~Shader();

		Shader* create();
		void bind();

		Shader* attachShader(ShaderBuilder *shader);
		Shader* attachShader(const std::string& filePath, GLenum shaderType);

		void apply();

	public:
		const std::string& getName() const { return m_Name; }
		int getOrderID() const { return m_OrderID; }

	public:
		void setProjectionMatrix(const glm::mat4x4& matrix);
		void setViewMatrix(const glm::mat4x4& matrix);
		void setModelMatrix(const glm::mat4x4& matrix);

	private:
		int m_ProgramID;
		std::string m_Name;

		int m_OrderID;

	private:
		int m_ProjectionMatrixID;
		int m_ViewMatrixID;
		int m_ModelMatrixID;
	};
}