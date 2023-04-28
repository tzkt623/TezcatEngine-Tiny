#include "RenderHelper.h"
#include "RenderCommand.h"
#include "BaseGraphics.h"
#include "Vertex.h"

namespace tezcat::Tiny
{
	std::list<RenderCommand*> RenderHelper::sPreList;


	void RenderHelper::renderPreData()
	{
		auto it = sPreList.begin();
		auto end = sPreList.end();
		while (it != end)
		{
			auto cmd = *it;
			switch (cmd->getLife())
			{
			case CMDLife::Once:
			{
				cmd->submit();
				Graphics::getInstance()->draw(cmd->getVertex());
				it = sPreList.erase(it);
				break;
			}
			case CMDLife::Countor:
			{
				Graphics::getInstance()->draw(cmd->getVertex());
				break;
			}
			case CMDLife::Continue:
			{
				Graphics::getInstance()->draw(cmd->getVertex());
				++it;
				break;
			}
			case CMDLife::Death:
			{
				it = sPreList.erase(it);
				break;
			}
			default:
				break;
			}
		}
	}

	void RenderHelper::render2D()
	{

	}

	void RenderHelper::renderPostData()
	{

	}

}

