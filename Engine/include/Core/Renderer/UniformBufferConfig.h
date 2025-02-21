#pragma once

/*
	Copyright (C) 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "../Head/ConfigHead.h"
#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"

namespace tezcat::Tiny
{
#define TINY_TEMPLATE_SET_STRING(x) []() -> const char* { return x; }
#define TINY_UNIFORM_BUFFER_LAYOUT(layoutName, belong, index, type, name)\
	using layoutName = Layout<belong, index, type, TINY_TEMPLATE_SET_STRING(name)>

	struct TINY_API UniformBufferBinding
	{
		template<class Belong, int32_t I, class Hold, class True>
		struct Sizer
		{
			using HoldSize = typename Hold;
			using TrueSize = typename True;
			constexpr static int32_t Index{ I };
		};

		template<class Belong, int32_t I, class Type, auto LambdaString>
		struct Layout
		{
			using TrueSize = typename Type;
			constexpr static int32_t Index{ I };
			constexpr static std::string_view Name{ LambdaString() };
		};

		struct Camera
		{
			constexpr static int32_t Index{ 0 };
			constexpr static std::string_view Name{ "CameraUBO" };

			/*
			using MatrixP = Sizer<Camera, 0, float4x4, float4x4>;
			using MatrixV = Sizer<Camera, 1, float4x4, float4x4>;
			using MatrixVP = Sizer<Camera, 2, float4x4, float4x4>;
			using WorldPosition = Sizer<Camera, 3, float4, float3>;
			using NearFar = Sizer<Camera, 4, float2, float2>;
			*/

			TINY_UNIFORM_BUFFER_LAYOUT(MatrixP
				, Camera
				, 0
				, float4x4
				, "TINY_MatrixP");

			TINY_UNIFORM_BUFFER_LAYOUT(MatrixV
				, Camera
				, 1
				, float4x4
				, "TINY_MatrixV");

			TINY_UNIFORM_BUFFER_LAYOUT(MatrixVP
				, Camera
				, 2
				, float4x4
				, "TINY_MatrixVP");

			TINY_UNIFORM_BUFFER_LAYOUT(WorldPosition
				, Camera
				, 3
				, float3
				, "TINY_CameraWorldPosition");

			TINY_UNIFORM_BUFFER_LAYOUT(NearFar
				, Camera
				, 4
				, float2
				, "TINY_CameraNearFar");
		};

		struct SkyBox
		{
			constexpr static int32_t Index{ 1 };
			constexpr static std::string_view Name{ "SkyboxUBO" };

			//using MatrixP = Sizer<SkyBox, 0, float4x4, float4x4>;
			//using MatrixV6 = Sizer<SkyBox, 1, float4x4[6], float4x4[6]>;
			//using ViewIndex = Sizer<SkyBox, 2, int32_t, int32_t>;
			//using Resolution = Sizer<SkyBox, 3, int32_t, int32_t>;
			//using Roughness = Sizer<SkyBox, 4, float, float>;

			TINY_UNIFORM_BUFFER_LAYOUT(MatrixP
				, SkyBox
				, 0
				, float4x4
				, "TINY_SkyBox_MatrixP");

			TINY_UNIFORM_BUFFER_LAYOUT(MatrixV6
				, SkyBox
				, 1
				, float4x4[6]
				, "TINY_SkyBox_MatrixV");

			TINY_UNIFORM_BUFFER_LAYOUT(ViewIndex
				, SkyBox
				, 2
				, int32_t
				, "TINY_SkyBox_ViewIndex");

			TINY_UNIFORM_BUFFER_LAYOUT(Resolution
				, SkyBox
				, 3
				, int32_t
				, "TINY_SkyBox_Resolution");

			TINY_UNIFORM_BUFFER_LAYOUT(Roughness
				, SkyBox
				, 4
				, float
				, "TINY_SkyBox_Roughness");
		};
	};

	class UniformBuffer;
	class TINY_API UniformBufferLayout
	{
	public:
		struct Slot
		{
			std::string name;
			int32_t offset = -1;
			uint32_t size = 0;
		};

	public:
		void pushLayout(const std::string& name, const int32_t& offset, const uint32_t& size);

		void layoutCheckComplete() { mLayoutChecked = true; }
		bool isLayoutChecked() const { return mLayoutChecked; }

		template<class LayoutSlot>
		void pushLayoutWithConfig()
		{
			if (mLayoutChecked)
			{
				return;
			}

			if (mSlot.size() > LayoutSlot::Index)
			{
				auto& slot = mSlot[LayoutSlot::Index];
				TINY_BREAK(slot.name == LayoutSlot::Name);
				slot.size = sizeof(LayoutSlot::TrueSize);
				if (!mGPUChecked)
				{
					mSize += slot.size;
				}
			}
			else
			{
				while (mSlot.size() <= LayoutSlot::Index)
				{
					mSlot.emplace_back();
				}

				auto& slot = mSlot[LayoutSlot::Index];
				slot.name = LayoutSlot::Name;
				slot.offset = -1;
				slot.size = sizeof(LayoutSlot::TrueSize);
				mSize += slot.size;
			}
		}

		template<class BindingData>
		void setIndex()
		{
			mBindingIndex = BindingData::Index;
		}

		Slot* findSlot(const std::string_view& name);

		bool isValied();

		bool addHolder(UniformBuffer* buffer)
		{
			return mBufferSet.emplace(buffer).second;
		}

		void removeHolder(UniformBuffer* buffer)
		{
			auto r = mBufferSet.erase(buffer);
		}

		void notifyHolderUpdateData();

		bool isGPUChecked() const { return mGPUChecked; }
		void gpuCheckComplete() { mGPUChecked = true; }

	private:
		bool mLayoutChecked = false;
		bool mGPUChecked = false;
		std::unordered_set<UniformBuffer*> mBufferSet;

	public:
		std::vector<Slot> mSlot;
		std::string mName;
		int32_t mBindingIndex = -1;
		uint32_t mSize;
	};
}
