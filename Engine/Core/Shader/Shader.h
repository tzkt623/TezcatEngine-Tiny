#pragma once
#include "ShaderParam.h"
#include "Uniform.h"

#include "../Head/GLMHead.h"
#include "../Head/ConfigHead.h"
#include "../Head/RenderConfig.h"
#include "../Base/TinyObject.h"


namespace tezcat::Tiny
{
	/*
	* Shader
	* @brief 含有一个唯一UID,使用这个ID去创建对应的RenderPass,
	*/
	class TINY_API Shader : public TinyObject
	{
		friend class BaseGraphics;
		Shader(const std::string& filePath);

		TINY_RTTI_H(Shader);
		TINY_Factory(Shader);
	public:

		virtual ~Shader();

		void generate();
		void apply(uint32_t pid);

		const std::string& getName() const { return mName; }
		const std::string& getFilePath() const { return mPath; }

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
		bool checkUniform(const UniformID& id)
		{
			return mTinyUniformList[id.getUID()] > -1;
		}

		int getUniformID(const UniformID& id)
		{
			return mTinyUniformList[id.getUID()];
		}

		uint32_t getTextureIndex()
		{
			return mGlobalTexture + mLocalTexure;
		}

		void addGlobalTextureIndex()
		{
			++mGlobalTexture;
		}

		void addLocalTextureIndex()
		{
			++mLocalTexure;
		}

		void resetGlobalState()
		{
			mGlobalTexture = 0;
			mLocalTexure = 0;
		}

		void resetLocalState()
		{
			mLocalTexure = 0;
		}

		void resizeUniformList(uint64_t size, int value);

		auto getUniformListSize()
		{
			return mTinyUniformList.size();
		}

		void setUniformID(uint32_t index, uint32_t value)
		{
			mTinyUniformList[index] = value;
		}

	public:
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

	public:
		CullFaceWrapper& getCullFaceWrapper() { return mCullFace; }

		bool isEnableBlend() { return mEnableBlend; }
		BlendWrapper& getBlendSourceWrapper() { return mBlendSource; }
		BlendWrapper& getBlendTargetWrapper() { return mBlendTarget; }

		bool isEnableZWrite() { return mEnableZWrite; }
		DepthTestWrapper& getDepthTestWrapper() { return mDepthTest; }

	protected:
		std::string mName;
		std::string mPath;
		int mUID;
		int mOrderID;
		int mVersion;
		int mProgramID;
		Queue mRenderQueue;
		uint8_t mLocalTexure;
		uint8_t mGlobalTexture;
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
