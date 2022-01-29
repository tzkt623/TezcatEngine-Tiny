#pragma once
#include "ShaderBuilder.h"
#include "ShaderConfig.h"
#include "CommonHead.h"
#include <vector>
#include <string>
#include <unordered_map>


namespace tezcat::Tiny::Core
{
	class Texture;
	class Shader
	{
		//---------------------------------------------
		//
		// shader中应该有控制GL渲染配置和流程的参数
		// 比如开关深度检测,模板,剪裁方式等等
		//
	public:
		Shader();
		Shader(const std::string& name, int orderID);
		~Shader();

	public://初始化生成
		void attachShader(unsigned int id);

		void registerTextureName(const std::string& textureTypeName);
		void registerUniformName(const std::string& uniformName);

		void apply();

	public:
		const std::string& getName() const { return m_Name; }
		void setName(const std::string& name) { m_Name = name; }
		int getOrderID() const { return m_OrderID; }
		void setOrderID(int orderID) { m_OrderID = orderID; }
		void bind();

	public://特化传参功能
		void setProjectionMatrix(const glm::mat4x4& matrix);
		void setViewMatrix(const glm::mat4x4& matrix);
		void setModelMatrix(const glm::mat4x4& matrix);
		void setTextures(const std::unordered_map<std::string, Texture*>& allTexture);



	public://通用传参功能
		void setFloat1(const std::string& name, float* data);
		void setFloat2(const std::string& name, float* data);
		void setFloat3(const std::string& name, float* data);

	private:
		std::string m_Name;
		int m_ProgramID;
		int m_OrderID;

	private:
		int m_ProjectionMatrixID;
		int m_ViewMatrixID;
		int m_ModelMatrixID;
		std::unordered_map<std::string, unsigned int> m_TextureID;
		std::unordered_map<std::string, unsigned int> m_UniformID;
	};
}