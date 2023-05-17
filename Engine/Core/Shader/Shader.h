#pragma once
#include "ShaderParam.h"
#include "Uniform.h"

#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Head/ConfigHead.h"
#include "../Head/RenderConfig.h"


namespace tezcat::Tiny
{
	class Texture;
	class TexttureCube;
	class PipelineQueue;

	/*
	* Shader
	* @brief 含有一个唯一UID,使用这个ID去创建对应的RenderPass,
	*/
	class TINY_API Shader
	{
	public:
		Shader();
		Shader(const std::string& name, int orderID);
		~Shader();

		virtual void createID() {}
		virtual void clear() {}
		void apply();

		const std::string& getName() const { return mName; }
		Queue getRenderQueue() const { return mRenderQueue; }
		void setRenderQueue(Queue val) { mRenderQueue = val; }
		void setName(const std::string& name) { mName = name; }
		int getProgramID() const { return mProgramID; }
		int getOrderID() const { return mOrderID; }
		void setOrderID(int orderID) { mOrderID = orderID; }
		int getUID() const { return mUID; }
		int getVersion() const { return mVersion; }
		void setVersion(int val) { mVersion = val; }
		void setLightMode(LightMode lightMode) { mLightMode = lightMode; }
		LightMode getLightMode() { return mLightMode; }
		bool isShadowReceiver() { return mIsShadowReceiver; }

	public:
		virtual void buildWithUniforms(const UniformID::USet& uniforms) = 0;

	public:
		void begin();
		void end();
		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual void resetGlobalState() = 0;
		virtual void resetLocalState() = 0;

	public:
		/// <summary>
		/// 设置当前pass的状态参数
		/// </summary>
		virtual void setStateOptions() = 0;
		void setZWrite(bool val) { mEnableZWrite = val; }

		void setCullFace(const CullFace& value)
		{
			mCullFace = ContextMap::CullFaceArray[(uint32_t)value];
		}

		void setCullFace(const CullFaceWrapper& value)
		{
			mCullFace = value;
		}

		void setBlend(bool val) { mEnableBlend = val; }

		void setBlendFunction(const Blend& source, const Blend& target)
		{
			mBlendSource = ContextMap::BlendArray[(uint32_t)source];
			mBlendTarget = ContextMap::BlendArray[(uint32_t)target];
		}

		void setBlendFunction(const BlendWrapper& source, const BlendWrapper& target)
		{
			mBlendSource = source;
			mBlendTarget = target;
		}

		void setDepthTest(const DepthTestWrapper& value)
		{
			mDepthTest = value;
		}

		void setDepthTest(const DepthTest& value)
		{
			mDepthTest = ContextMap::DepthTestArray[(uint32_t)value];
		}

	public://特化传参
		virtual void setProjectionMatrix(const glm::mat4x4& matrix) = 0;

		virtual void setViewMatrix(const glm::mat4x4& matrix) = 0;
		virtual void setModelMatrix(const glm::mat4x4& matrix) = 0;
		virtual void setViewPosition(const glm::vec3& position) = 0;

	public://慢速传参
		virtual void setFloat1(const char* name, float* data) = 0;
		virtual void setFloat2(const char* name, float* data) = 0;
		virtual void setFloat3(const char* name, float* data) = 0;
		virtual void setFloat4(const char* name, float* data) = 0;

		virtual void setInt1(const char* name, int* data) = 0;
		virtual void setInt2(const char* name, int* data) = 0;
		virtual void setInt3(const char* name, int* data) = 0;
		virtual void setInt4(const char* name, int* data) = 0;

		virtual void setMat3(const char* name, float* data) = 0;
		virtual void setMat4(const char* name, const float* data) = 0;

	public://快速传参
		virtual void setFloat1(UniformID& uniform, float* data) = 0;
		virtual void setFloat2(UniformID& uniform, float* data) = 0;
		virtual void setFloat2(UniformID& uniform, const glm::vec2& data) = 0;
		virtual void setFloat3(UniformID& uniform, float* data) = 0;
		virtual void setFloat4(UniformID& uniform, float* data) = 0;

		virtual void setInt1(UniformID& uniform, const int& data) = 0;
		virtual void setInt1(UniformID& uniform, int* data) = 0;
		virtual void setInt2(UniformID& uniform, int* data) = 0;
		virtual void setInt3(UniformID& uniform, int* data) = 0;
		virtual void setInt4(UniformID& uniform, int* data) = 0;

		virtual void setMat3(UniformID& uniform, float* data) = 0;
		virtual void setMat3(UniformID& uniform, const glm::mat3& mat3) = 0;
		virtual void setMat4(UniformID& uniform, const float* data) = 0;
		virtual void setMat4(UniformID& uniform, const glm::mat4& mat4) = 0;
		virtual void setMat4(UniformID& uniform, glm::mat4 data[], int count) = 0;

		virtual void setGlobalTexture2D(UniformID& uniform, Texture2D* data) = 0;
		virtual void setTexture2D(UniformID& uniform, Texture2D* data) = 0;

		virtual void setGlobalTextureCube(UniformID& uniform, TextureCube* data) = 0;
		virtual void setTextureCube(UniformID& uniform, TextureCube* data) = 0;

	protected:
		std::string mName;
		int mUID;
		int mOrderID;
		int mVersion;
		int mProgramID;
		Queue mRenderQueue;

	protected:

		bool mIsShadowReceiver;

		//LightMode
		LightMode mLightMode;

		//cull
		CullFaceWrapper mCullFace;

		//blend
		bool mEnableBlend;
		BlendWrapper mBlendSource;
		BlendWrapper mBlendTarget;

		//depth test
		bool mEnableZWrite;
		DepthTestWrapper mDepthTest;

	protected:
		std::unordered_map<std::string, uint32_t> mTextureID;
		std::unordered_map<std::string, uint32_t> mUniformID;
		std::vector<int> mTinyUniformList;
	};
}
