#pragma once
#include "ShaderParam.h"
#include "Uniform.h"

#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Head/ConfigHead.h"
#include "../Head/Context.h"
#include "../Pipeline/PipelineQueue.h"
#include "../Pipeline/Pipeline.h"


namespace tezcat::Tiny::Core
{
	class Texture;
	class TexttureCube;
	class PipelineQueue;
	class TINY_API Shader
	{
		//---------------------------------------------
		//
		//
	public:
		Shader();
		Shader(const std::string& name, int orderID);
		~Shader();

		virtual void create() {}
		virtual void clear() {}
		void apply();

		inline const std::string& getName() const { return mName; }
		inline PipelineQueue::Queue getRenderQueue() const { return mRenderQueue; }
		inline void setRenderQueue(PipelineQueue::Queue val) { mRenderQueue = val; }
		inline void setName(const std::string& name) { mName = name; }
		inline int getProgramID() const { return mProgramID; }
		inline int getOrderID() const { return mOrderID; }
		inline void setOrderID(int orderID) { mOrderID = orderID; }
		inline int getUID() const { return mUID; }
		inline int getVersion() const { return mVersion; }
		inline void setVersion(int val) { mVersion = val; }
		inline void setLightMode(LightMode lightMode) { mLightMode = lightMode; }
		inline LightMode getLightMode() { return mLightMode; }

	public:
		void apply(const UniformID::USet& uniforms);

	protected:
		virtual void onApply(const UniformID::USet& uniforms) {}

	public:
		void begin();
		void end();
		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual void resetState() = 0;

	public:
		inline bool isEnableLighting() const { return mEnableLighting; }

	public:
		/// <summary>
		/// 设置当前pass的状态参数
		/// </summary>
		virtual void setStateOptions() = 0;
		void setZWrite(bool val) { mEnableZWrite = val; }
		void setLighting(bool val) { mEnableLighting = val; }

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
		virtual void setMat4(UniformID& uniform, const glm::mat4& mat4) = 0;

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

		virtual void setTexture2D(UniformID& uniform, Texture2D* data) = 0;
		virtual void setTextureCube(UniformID& uniform, TextureCube* data) = 0;

	public:
		void cacheFilePath(const std::string& path)
		{
			mFilePath = path;
		}
		void rebuild(const UniformID::USet& uniforms);
	private:
		std::string mName;
		int mUID;
		int mOrderID;
		int mVersion;

		PipelineQueue::Queue mRenderQueue;

	protected:
		int mProgramID;
		bool mEnableLighting;

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
		int mProjectionMatrixID;
		int mViewMatrixID;
		int mModelMatrixID;
		int mNormalMatrixID;
		int mViewPositionID;

	protected:
		std::unordered_map<std::string, uint32_t> mTextureID;
		std::unordered_map<std::string, uint32_t> mUniformID;
		std::vector<int> mTinyUniformList;
		std::string mFilePath;
	};
}
