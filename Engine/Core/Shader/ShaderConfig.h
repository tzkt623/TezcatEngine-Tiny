#pragma once

#include "../Head/RenderConfig.h"

namespace tezcat::Tiny
{
	struct ArgMetaData;

	enum class TINY_API ShaderConstraint : int8_t
	{
		Error = -1,
		Color,
		Range,
		Count
	};

	struct TINY_API BaseRange
	{
		enum class Type
		{
			Float,
			Int,
			UInt,
			Double
		};

		virtual Type getType() = 0;
	};

	template<typename T>
	struct TINY_API RangeT : BaseRange
	{
		T min;
		T max;

		RangeT(T min, T max)
			: min(min)
			, max(max)
		{}

		virtual ~RangeT() {}
	};

	struct TINY_API RangeFloat : public RangeT<float>
	{
		using RangeT::RangeT;
		Type getType() final { return BaseRange::Type::Float; }
	};

	struct TINY_API RangeInt : public RangeT<int>
	{
		using RangeT::RangeT;
		Type getType() final { return BaseRange::Type::Int; }
	};

	struct TINY_API RangeUInt : public RangeT<uint32_t>
	{
		using RangeT::RangeT;
		Type getType() final { return BaseRange::Type::UInt; }
	};

	struct TINY_API BaseArgInfo
	{

	};

	struct TINY_API ArgMemberInfo : public BaseArgInfo
	{
		//编辑器中的名称
		std::string editorName;
		//约束
		ShaderConstraint constraint;
		//范围
		std::shared_ptr<BaseRange> range;
	};

	struct TINY_API ArgStructInfo : public BaseArgInfo
	{
		//类类型名
		std::string structName;
		//类成员变量
		std::vector<std::shared_ptr<ArgMetaData>> members;
	};

	struct TINY_API ArgMetaData
	{
		//值名称
		std::string valueName;
		//值类型
		UniformType valueType;
		//值数量
		int valueCount;

		std::shared_ptr<BaseArgInfo> info;

		ArgMetaData()
			: valueCount(0)
			, valueType(UniformType::Error)
		{

		}

		~ArgMetaData()
		{

		}

		template<typename Info>
		auto createInfo(const UniformType& uType)
		{
			info.reset(new Info());
			this->valueType = uType;
			return (Info*)info.get();
		}

		template<typename Info>
		auto getInfo()
		{
			return (Info*)info.get();
		}
	};


	struct TINY_API UniformInfo
	{
		int index;
		std::string name;
		UniformType type;
		int shaderID;
		ShaderConstraint constraint;
		std::string editorName;

		std::shared_ptr<BaseRange> range;
	};
}