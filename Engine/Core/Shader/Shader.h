#pragma once
#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Pipeline/Pipeline.h"
#include "../Head/ConfigHead.h"
#include "../Head/Context.h"
#include "ShaderParam.h"
#include "Uniform.h"


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
		void apply(const UniformID::USet& uniforms);

	protected:
		virtual void onApply(const UniformID::USet& uniforms) {}

	public:
		virtual void bind() = 0;
		virtual void resetState() = 0;

	public:
		inline bool isEnableLighting() const { return m_EnableLighting; }

	public:
		virtual void setGPUOptions() = 0;
		void setZWrite(bool val) { m_EnableZWrite = val; }
		void setLighting(bool val) { m_EnableLighting = val; }

		void setCullFace(const CullFace& value)
		{
			m_CullFace = ContextMap::CullFaceArray[(int)value];
		}

		void setCullFace(const CullFaceWrapper& value)
		{
			m_CullFace = value;
		}

		void setBlend(bool val) { m_EnableBlend = val; }

		void setBlendFunction(const Blend& source, const Blend& target)
		{
			m_BlendSource = ContextMap::BlendArray[(int)source];
			m_BlendTarget = ContextMap::BlendArray[(int)target];
		}

		void setBlendFunction(const BlendWrapper& source, const BlendWrapper& target)
		{
			m_BlendSource = source;
			m_BlendTarget = target;
		}

	public://特化传参功能
		virtual void setProjectionMatrix(const glm::mat4x4& matrix) = 0;

		virtual void setViewMatrix(const glm::mat4x4& matrix) = 0;
		virtual void setModelMatrix(const glm::mat4x4& matrix) = 0;
		virtual void setViewPosition(const glm::vec3& position) = 0;

		/// <summary>
		/// 传入参数为模型矩阵
		/// </summary>
		virtual void setNormalMatrix(const glm::mat4x4& matrix) = 0;
		virtual void setTextures(const UniformID::UMap<Texture*>& allTexture) = 0;

	public://慢速版通用传参功能
		virtual void setFloat1(const char* name, float* data) = 0;
		virtual void setFloat2(const char* name, float* data) = 0;
		virtual void setFloat3(const char* name, float* data) = 0;
		virtual void setFloat4(const char* name, float* data) = 0;

		virtual void setInt1(const char* name, int* data) = 0;
		virtual void setInt2(const char* name, int* data) = 0;
		virtual void setInt3(const char* name, int* data) = 0;
		virtual void setInt4(const char* name, int* data) = 0;

		virtual void setMat3(const char* name, float* data) = 0;
		virtual void setMat4(const char* name, float* data) = 0;

	public://快速版通用传参功能
		virtual void setFloat1(UniformID& uniform, float* data) = 0;
		virtual void setFloat2(UniformID& uniform, float* data) = 0;
		virtual void setFloat3(UniformID& uniform, float* data) = 0;
		virtual void setFloat4(UniformID& uniform, float* data) = 0;

		virtual void setInt1(UniformID& uniform, int* data) = 0;
		virtual void setInt2(UniformID& uniform, int* data) = 0;
		virtual void setInt3(UniformID& uniform, int* data) = 0;
		virtual void setInt4(UniformID& uniform, int* data) = 0;

		virtual void setMat3(UniformID& uniform, float* data) = 0;
		virtual void setMat4(UniformID& uniform, float* data) = 0;

		virtual void setTexture2D(UniformID& uniform, Texture2D* data) = 0;

	private:
		std::string m_Name;
		int m_OrderID;
		int m_UID;
		int m_Version;

		Pipeline::Queue m_RenderQueue;

	protected:
		int m_ProgramID;
		bool m_EnableZWrite;
		bool m_EnableBlend;
		bool m_EnableLighting;

		CullFaceWrapper m_CullFace;
		BlendWrapper m_BlendSource;
		BlendWrapper m_BlendTarget;

	protected:
		int m_ProjectionMatrixID;
		int m_ViewMatrixID;
		int m_ModelMatrixID;
		int m_NormalMatrixID;
		int m_ViewPositionID;

	protected:
		std::unordered_map<std::string, uint32_t> m_TextureID;
		std::unordered_map<std::string, uint32_t> m_UniformID;

		std::vector<int> m_TinyUniformList;
	};
}