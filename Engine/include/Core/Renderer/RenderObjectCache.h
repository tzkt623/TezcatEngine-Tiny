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
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class BaseRenderObserver;
	class BaseRenderer;
	class PipelinePass;
	class ReplacedPipelinePass;

	/*
	* RenderLayer
	*
	* 
	* @brief 负责记录存在于当前Layer中的所有RenderObejct的Agent,以提供以下功能
	* @brief 1.相机剔除,相机可以通过查看对应的Layer,对可见的Object进行剔除
	* @brief 2.阴影剔除,阴影投射器可以通过查看对应的Layer,对可渲染阴影的Object进行剔除
	*          阴影只会查看非透明通道的单位进行剔除
	* 
	* @brief RenderLayer不会主动删除内部的Agent,而是等待Agent的Host销毁后,再下次遍历时自动删除
	* 
	* @brief Camera会在Layer内部进行剔除的时候,命令可见的单位生成RenderCommand并送到对应的管线.
	*/
	class TINY_API RenderObjectCache
	{
	public:
		RenderObjectCache();
		~RenderObjectCache();

	private:
		int mIndex;
		std::list<TinyWeakRef<BaseRenderer>> mRenderObjectList;
		std::list<TinyWeakRef<BaseRenderer>> mTransparentObjectList;

	public:
		static void init();

		static void culling(int layerIndex, BaseRenderObserver* renderObserver);
		static void culling(int layerIndex, ReplacedPipelinePass* pass);
		static RenderObjectCache* getRenderLayer(uint32_t index)
		{
			return sLayerAry[index];
		}

		static void addRenderAgent(uint32_t index, BaseRenderer* renderAgent);


	private:
		static std::array<RenderObjectCache*, 32> sLayerAry;
	};
}
