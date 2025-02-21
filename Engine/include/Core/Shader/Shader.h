#pragma once

/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "ShaderParam.h"
#include "Uniform.h"
#include "ShaderConfig.h"

#include "../Head/GLMHead.h"
#include "../Base/TinyObject.h"


namespace tezcat::Tiny
{
	class ShaderParser;
	/*
	* Shader
	* @brief 含有一个唯一UID,使用这个ID去创建对应的RenderPass
	* @brief 包含所有uniform的位置ID
	*/
	class TINY_API Shader : public TinyObject
	{
		friend class BaseGraphics;
		Shader(const std::string& filePath);
		TINY_OBJECT_H(Shader, TinyObject)

	public:

		virtual ~Shader();

		void generate();
		void rebuild();
		void apply(uint32 pid);

		const std::string& getName() const { return mName; }
		const std::string& getFilePath() const { return mPath; }
		bool checkAndLoadContent();

	public:

		Queue getRenderQueue() const { return mRenderQueue; }
		void setRenderQueue(Queue val) { mRenderQueue = val; }
		void setName(const std::string& name) { mName = name; }
		void setName(std::string& name) { mName.assign(name); }

		int32 getProgramID() const { return mProgramID; }
		int32 getOrderID() const { return mOrderID; }
		void setOrderID(int32 orderID) { mOrderID = orderID; }
		int32 getUID() const { return mUID; }
		int32 getVersion() const { return mVersion; }
		void setVersion(int32 val) { mVersion = val; }
		void setLightMode(LightMode lightMode) { mLightMode = lightMode; }
		LightMode getLightMode() const { return mLightMode; }
		bool isShadowReceiver() const { return mIsShadowReceiver; }

		int32 getPropertyID(const std::string_view& name)
		{
			auto it = mUserUniformValueConfigMap.find(name);
			TINY_THROW_RUNTIME(it != mUserUniformValueConfigMap.end(), name);
			return it->second->index;
		}

		int getUniformIndex(const std::string_view& name)
		{
			auto it = mUserUniformValueConfigMap.find(name);
			TINY_THROW_RUNTIME(it == mUserUniformValueConfigMap.end(), name);
			return it->second->index;
		}

		UniformValueConfig* getUniformValueConfig(const std::string_view& name) const
		{
			auto it = mUserUniformValueConfigMap.find(name);
			TINY_THROW_RUNTIME(it == mUserUniformValueConfigMap.end(), name);
			return it->second;
		}

		UniformValueConfig* getUniformValueConfig(const uint32& index) const
		{
			return mUserUniformValueConfigAry[index];
		}

	public:
		bool checkUniform(const UniformID& id)
		{
			return mTinyUniformList[id.getUID()]->valueID > -1;
		}

		bool checkTinyUniform(const UniformID& id, int& outValueID)
		{
			outValueID = mTinyUniformList[id.getUID()]->valueID;
			return outValueID > -1;
		}

		int32 getTinyUniformShaderID(const UniformID& id)
		{
			return mTinyUniformList[id.getUID()]->valueID;
		}

		uint32 getTextureIndex()
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

		auto getTinyUniformCount()
		{
			return mTinyUniformList.size();
		}

		const auto& getTinyUniforms() const
		{
			return mTinyUniformList;
		}

		const auto& getUniformMap() const
		{
			return mUserUniformValueConfigMap;
		}

		const auto& getUniformAry() const
		{
			return mUserUniformValueConfigAry;
		}

		void setupTinyUniform(ArgMetaData* metaData, const std::string& name, const uint32& index, const int& shaderID, const int& arrayIndex = -1);
		void resizeTinyUniformAry(uint64 size);

		void setupUserUniformID(ArgMetaData* metaData, const std::string& name, const int& shaderID, const int& arrayIndex = -1);
		void resizeUserUniformAry(uint64 size);

	public:
		void setZWrite(bool val) { mEnableZWrite = val; }

		void setCullFace(const CullFace& value)
		{
			mCullFace = ContextMap::CullFaceArray[(uint32)value];
		}

		void setCullFace(const CullFaceWrapper& value)
		{
			mCullFace = value;
		}

		void setBlend(bool val) { mEnableBlend = val; }

		void setBlendFunction(const Blend& source, const Blend& target)
		{
			mBlendSource = ContextMap::BlendArray[(uint32)source];
			mBlendTarget = ContextMap::BlendArray[(uint32)target];
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
			mDepthTest = ContextMap::DepthTestArray[(uint32)value];
		}

	public:
		CullFaceWrapper& getCullFaceWrapper() { return mCullFace; }

		bool isEnableBlend() const { return mEnableBlend; }
		const BlendWrapper& getBlendSourceWrapper() const { return mBlendSource; }
		const BlendWrapper& getBlendTargetWrapper() const { return mBlendTarget; }

		bool isEnableZWrite() const { return mEnableZWrite; }
		const DepthTestWrapper& getDepthTestWrapper() const { return mDepthTest; }



	public:
		std::unique_ptr<ShaderParser> mParser;
		std::string mContent;

	protected:
		std::string mName;
		std::string mPath;

	private:
		int32 mUID;
		int32 mOrderID;
		int32 mVersion;
		int32 mProgramID;
		Queue mRenderQueue;
		uint8 mLocalTexure;
		uint8 mGlobalTexture;
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
		std::vector<UniformValueConfig*> mTinyUniformList;
		std::vector<UniformValueConfig*> mUserUniformValueConfigAry;
		std::unordered_map<std::string_view, UniformValueConfig*> mUserUniformValueConfigMap;
	};
}
