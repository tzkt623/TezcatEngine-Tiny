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
#include "../Tool/Tool.h"

namespace tezcat::Tiny
{
	class EngineEvent
	{
		EngineEvent() = delete;
		~EngineEvent() = delete;

	public:
		static Event* getInstance() { return mEvent; }

	private:
		static Event* mEvent;
	};

	enum EngineEventID : EventID
	{
		EE_LoadScene = 0,
		EE_PopScene,
		EE_PushScene,
		EE_OnPopScene,
		EE_OnPushScene,

		EE_SetMainCamera,
		EE_ActiveSkybox,
		EE_ChangeEnvImage,

		EE_LoadModel,
		EE_MouseLeftButonUp,
		EE_MouseRightButonUp,

		EE_ReadObjectID,
		EE_SelectObject,

		EE_IDCount
	};
}
