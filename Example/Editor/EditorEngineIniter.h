#pragma once

#include "include/Tiny.h"

namespace tezcat::Editor
{
	class EditorEngineIniter : public EngineIniter
	{
	public:
		EditorEngineIniter();

		void prepareEngine(Engine* engine) override;
		void prepareResource(Engine* engine) override;
		void prepareGame(Engine* engine) override;
		void initYourShaderParam() override;
	};
}
