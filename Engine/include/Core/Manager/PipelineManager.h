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

#include "../Head/TinyCpp.h"

namespace tezcat::Tiny
{
	class Shader;
	class ObserverPipelinePass;
	class PipelineQueue;
	class Pipeline;
	class PipelinePass;
	class BaseRenderObserver;
	class ReplacedPipelinePass;


	/*
	* Pass的创建和查找流程
	* 结构
	* Pass本身只和Shader强相关,每一个Pass渲染的都是相同Shader的Object,也就是同一个Material
	* 但是存在一个问题,生成RenderCMD的过程,是靠相机进行剔除时进行的,顺便对没有生成的Pass进行生成
	* 也就是Pass本身需要知道是哪个观察者对自己进行渲染
	* 如果把所有CMD都放到一个Pass中,那么每个相机进入渲染的时候,还需要再做筛选,完全没有必要
	* 所以解决方法,应该是给每个相机,都生成一套完整的Pass系统
	* 但是在删除相机的时候,不能牵连到生成的Pass,因为相机和可渲染物体没有强相关性
	* 所以解决方案是在PipelineManager中,集中管理Pass系统,并且给每个相机一个此系统的引用
	*
	* 比如相机1可以看到layer1,2,3的物体
	* 所以相机在遍历时,会把layer1,2,3中同一个材质的物体生成渲染命令到同一个Pass中
	*
	* 所以cmd在生成时,需要查询当前object在哪个layer中,然后在此layer中生成对应的pass,然后将cmd添加到pass中
	* 这个时候的问题是需要确定这个pass属于哪个observer
	* 还有一个问题,ReplacedPass可以不属于任何一个layer
	* 
	*/

	class PipelineManager
	{
	public:
		static void init();
		static void initPipeline(Pipeline* pipeline);

		static void addToPipeline(PipelineQueue* queue);
		static void sortQueue();

	private:
		static Pipeline* sCurrentPipeline;
		static std::vector<PipelineQueue*> sObserverQueueTable;
		static std::array<PipelineQueue*, 32> sTable;
	};
}
