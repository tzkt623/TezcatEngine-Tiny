#pragma once
#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Pipeline/Pipeline.h"
#include "../Head/ConfigHead.h"
#include "ShaderConfig.h"


namespace tezcat::Tiny::Core
{
	class Texture;
	class Pipeline;
	class TINY_API Shader
	{
		//---------------------------------------------
		//
		// shader中应该有控制GL渲染配置和流程的参数
		// 比如开关深度检测,模板,剪裁方式等等
		// 每一个Shader在诞生时都有一个对应的Pass
		// 这个Pass是永远不会变的
		//
	public:
		Shader();
		Shader(const std::string& name, int orderID);
		~Shader();

		inline const std::string& getName() const { return m_Name; }
		inline Pipeline::Queue getRenderQueue() const { return m_RenderQueue; }
		inline void setRenderQueue(Pipeline::Queue val) { m_RenderQueue = val; }
		inline void setName(const std::string& name) { m_Name = name; }
		inline int getProgramID() const { return m_ProgramID; }
		inline int getOrderID() const { return m_OrderID; }
		inline void setOrderID(int orderID) { m_OrderID = orderID; }
		inline int getUID() const { return m_UID; }
		inline int getVersion() const { return m_Version; }
		inline void setVersion(int val) { m_Version = val; }

	public://初始化生成
		void apply();

	protected:
		virtual void onApply() {}

	public:
		virtual void bind() = 0;
		virtual void registerTextureName(const std::string& textureName) = 0;
		virtual void registerUniformName(const std::string& uniformName) = 0;
		virtual void registerUniform(const std::string& uniformType, const std::string& uniformName) = 0;

	public:
		inline bool isEnableLighting() const { return m_EnableLighting; }

	public:
		virtual void setGPUOptions() = 0;
		void setZWrite(bool val) { m_EnableZWrite = val; }
		void setLighting(bool val) { m_EnableLighting = val; }
		virtual void setCullFace(int value) = 0;
		virtual void setBlend(bool val) { m_EnableBlend = val; }
		virtual void setBlendFunction(int source, int target) = 0;

	public://特化传参功能
		virtual void setProjectionMatrix(const glm::mat4x4& matrix) = 0;
		virtual void setViewMatrix(const glm::mat4x4& matrix) = 0;
		virtual void setModelMatrix(const glm::mat4x4& matrix) = 0;
		virtual void setViewPosition(const glm::vec3& position) = 0;

		/// <summary>
		/// 传入参数为模型矩阵
		/// </summary>
		virtual void setNormalMatrix(const glm::mat4x4& matrix) = 0;
		virtual void setTextures(const std::unordered_map<std::string, Texture*>& allTexture) = 0;

	public://通用传参功能
		virtual void setFloat1(const std::string& name, float* data) = 0;
		virtual void setFloat2(const std::string& name, float* data) = 0;
		virtual void setFloat3(const std::string& name, float* data) = 0;

	private:
		std::string m_Name;
		int m_OrderID;
		int m_UID;
		int m_Version;

		Pipeline::Queue m_RenderQueue;

	protected:
		int m_ProgramID;
		bool m_EnableZWrite;
		bool m_EnableCullFace;
		bool m_EnableBlend;
		bool m_EnableLighting;

	protected:
		int m_ProjectionMatrixID;
		int m_ViewMatrixID;
		int m_ModelMatrixID;
		int m_NormalMatrixID;
		int m_ViewPositionID;
	};
}