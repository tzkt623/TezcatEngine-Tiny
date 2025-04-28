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
		friend class ShaderParser;

		Shader(const std::string& filePath);
		TINY_OBJECT_H(Shader, TinyObject)

	public:

		virtual ~Shader();

		void generate();
		void rebuild();
		void apply(uint32_t pid);

		const std::string& getName() const { return mName; }
		const std::string& getFilePath() const { return mPath; }
		bool checkAndLoadContent();

		std::string getMemoryInfo() override;

	public:
		PipelineQueueType getRenderQueue() const { return mRenderQueue; }
		void setRenderQueue(PipelineQueueType val) { mRenderQueue = val; }

		void setName(const std::string& name)
		{
			mName = name;
			mEngineName = mName;
		}

		void setName(std::string& name)
		{
			mName.assign(name);
			mEngineName = mName;
		}

		uint32_t getProgramID() const { return mProgramID; }

		int16_t getSortingOffset() const { return mSortingOffset; }

		void setSortingOffset(int16_t offset) { mSortingOffset = offset; }

		int32_t getUID() const { return mUID; }

		int32_t getVersion() const { return mVersion; }

		void setVersion(int32_t val) { mVersion = val; }

		void setLightMode(LightMode lightMode) { mLightMode = lightMode; }

		LightMode getLightMode() const { return mLightMode; }

		bool isShadowReceiver() const { return mIsShadowReceiver; }

		int32_t getPropertyID(const std::string_view& name)
		{
			auto it = mUserUniformValueConfigMap.find(name);
			TINY_THROW_RUNTIME(it != mUserUniformValueConfigMap.end(), name);
			return it->second->index;
		}

		int32_t getUserUniformIndex(const std::string_view& name)
		{
			auto it = mUserUniformValueConfigMap.find(name);
			TINY_THROW_RUNTIME(it == mUserUniformValueConfigMap.end(), name);
			return it->second->index;
		}

		UniformValueConfig* getUserUniformValueConfig(const std::string_view& name) const
		{
			auto it = mUserUniformValueConfigMap.find(name);
			TINY_THROW_RUNTIME(it == mUserUniformValueConfigMap.end(), name);
			return it->second;
		}

		UniformValueConfig* getUserUniformValueConfig(const uint32_t& index) const
		{
			return mUserUniformValueConfigAry[index];
		}

	public:
		bool checkUniform(const UniformID& id)
		{
			TINY_THROW_RUNTIME(id.toUID() < 0, "UID must >= 0");
			return mTinyUniformList[id.toUID()]->valueID > -1;
		}

		bool checkTinyUniform(const UniformID& id, int& outValueID)
		{
			TINY_THROW_RUNTIME(id.toUID() < 0, "UID must >= 0");
			outValueID = mTinyUniformList[id.toUID()]->valueID;
			return outValueID > -1;
		}

		int32_t getTinyUniformShaderID(const UniformID& id)
		{
			return mTinyUniformList[id.toUID()]->valueID;
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

		auto getTinyUniformCount()
		{
			return mTinyUniformList.size();
		}

		const auto& getTinyUniforms() const
		{
			return mTinyUniformList;
		}

		const auto& getUserUniformMap() const
		{
			return mUserUniformValueConfigMap;
		}

		const auto& getUserUniformArray() const
		{
			return mUserUniformValueConfigAry;
		}

		void setTinyUniform(const std::string& name, const int32_t& shaderID);
		void setUserUniform(const std::string& name, const int32_t& shaderID);

		void resizeUserUniformArray(uint64_t size);

	public:
		void setZWrite(bool val) { mEnableZWrite = val; }

		void setCullFace(const CullFace& value)
		{
			mCullFace = GraphicsConfig::CullFaceArray[(uint32_t)value];
		}

		void setCullFace(const CullFaceWrapper& value)
		{
			mCullFace = value;
		}

		void setBlend(bool val) { mEnableBlend = val; }

		void setBlendFunction(const Blend& source, const Blend& target)
		{
			mBlendSource = GraphicsConfig::BlendArray[(uint32_t)source];
			mBlendTarget = GraphicsConfig::BlendArray[(uint32_t)target];
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
			mDepthTest = GraphicsConfig::DepthTestArray[(uint32_t)value];
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
		std::unique_ptr<std::string> mContent;
		//std::string mContent;

	private:
		void setupTinyUniform(ShaderUniformMember* metaData, const std::string& name, const uint32_t& index, const int& shaderID, const int& arrayIndex = -1);
		void setupUserUniformID(ShaderUniformMember* metaData, const std::string& name, const int& shaderID, const int& arrayIndex = -1);

		void registerTinyUniform(ShaderUniformMember* memberData);
		void registerUserUniform(ShaderUniformMember* memberData);

		void parseTinyUniform(ShaderUniformMember* memberData, const std::string& parentName);
		void parseUserUniform(ShaderUniformMember* memberData, const std::string& parentName);

	protected:
		std::string mName;
		std::string mPath;

	private:
		PipelineQueueType mRenderQueue;
		int32_t mUID;
		int32_t mVersion;
		uint32_t mProgramID;
		int16_t mSortingOffset;
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
		std::vector<UniformValueConfig*> mTinyUniformList;
		std::vector<UniformValueConfig*> mUserUniformValueConfigAry;
		std::unordered_map<std::string_view, UniformValueConfig*> mUserUniformValueConfigMap;
	};
}
