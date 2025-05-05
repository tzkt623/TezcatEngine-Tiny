#pragma once
/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "ShaderConfig.h"

namespace tezcat::Tiny
{
#pragma region Attribute

	enum class TINY_API ShaderAttributeType
	{
		Register = 0,
		Binding,
		Editor,
		Count
	};

	struct TINY_API ShaderAttribute
	{
		virtual ShaderAttributeType getType() = 0;
	};

	struct TINY_API ShaderAttributeEditor : public ShaderAttribute
	{
		std::string editorName;
		std::unique_ptr<BaseRange> range;
		RangeType rangeType = RangeType::Number;

		ShaderAttributeEditor()
			: range(std::make_unique<RangeFloat>(0.0f, 0.0f))
		{

		}
		ShaderAttributeType getType() override final { return ShaderAttributeType::Editor; }
	};

	struct TINY_API ShaderAttributeRegister : public ShaderAttribute
	{
		int32_t globalIndex = -1;

		ShaderAttributeType getType() override final { return ShaderAttributeType::Register; }
	};

	struct TINY_API ShaderAttributeBinding : public ShaderAttribute
	{
		int32_t bindingIndex = -1;

		ShaderAttributeType getType() override final { return ShaderAttributeType::Binding; }
	};
#pragma endregion Attribute

#pragma region MetaData

	enum class TINY_API ShaderMetaDataType
	{
		Member = 0,
		Struct,
		LayoutStruct
	};

	struct TINY_API ShaderMetaData
	{
		std::string name;
		virtual ShaderMetaDataType getType() = 0;
		virtual std::size_t getHashID() const { return 0; }
	};

	struct TINY_API ShaderMetaDataHash
	{
		std::size_t operator()(ShaderMetaData* comparer) const
		{
			return comparer->getHashID();
		}
	};

	struct TINY_API ShaderMetaDataComparer
	{
		bool operator()(const ShaderMetaData* a, const ShaderMetaData* b) const
		{
			return a->getHashID() == b->getHashID();
		}
	};

	using ShaderMetaDataSet = std::unordered_set<ShaderMetaData*, ShaderMetaDataHash, ShaderMetaDataComparer>;
	using ShaderMemberMetaDataHandler = std::vector<std::function<void(ShaderMetaData*)>>;

	struct TINY_API ShaderMetaDataArgument : public ShaderMetaData
	{
		UniformType valueType = UniformType::Error;
		uint32_t arrayCount = 0;
	};

	struct TINY_API ShaderMetaDataMember : public ShaderMetaDataArgument
	{
		//成员占内存的大小
		std::unique_ptr<ShaderAttributeEditor> attributeEditor;
		std::unique_ptr<ShaderAttributeRegister> attributeRegister;

		ShaderMetaDataType getType() override { return ShaderMetaDataType::Member; }
		std::size_t getHashID() const override { return 1; }
	};

	struct TINY_API ShaderMetaDataStruct : public ShaderMetaDataArgument
	{
		std::string structType;
		//std::unordered_map<std::string_view, std::shared_ptr<ShaderMetaDataArgument>> memberMap;

		ShaderMetaDataStruct()
		{
			this->valueType = UniformType::Struct;
		}

		ShaderMetaDataType getType() override { return ShaderMetaDataType::Struct; }
		std::size_t getHashID() const override { return 2; }

		ShaderMetaDataMember* addMemeber(const std::string& name, const UniformType& uniformType);
		ShaderMetaDataStruct* addStruct(const std::string& name);
		void copyMembers(ShaderMetaDataStruct* other)
		{
			members.clear();
			members.insert(members.begin(), other->members.begin(), other->members.end());
		}

		const std::vector<std::shared_ptr<ShaderMetaDataArgument>>& getMembers() const { return members; }

	private:
		std::vector<std::shared_ptr<ShaderMetaDataArgument>> members;
	};

	struct TINY_API ShaderMetaDataLayoutStruct : public ShaderMetaDataStruct
	{
		std::unique_ptr<ShaderAttributeBinding> attributBinding;

		ShaderMetaDataType getType() override final { return ShaderMetaDataType::LayoutStruct; }
		std::size_t getHashID() const override { return 3; }
		void initBindingAttribute() { this->attributBinding = std::make_unique<ShaderAttributeBinding>(); }
	};

#pragma endregion MetaData

	struct TINY_API UniformValueConfig
	{
		//index in class Shader
		int32_t index;
		std::string name;
		UniformType type;
		//id in Graphic shader
		int32_t valueID;

		std::string_view editorName;

		ShaderAttributeEditor* attributeEditor;
		ShaderAttributeRegister* attributeRegister;

		~UniformValueConfig()
		{
			attributeRegister = nullptr;
			attributeEditor = nullptr;
		}
	};
}
