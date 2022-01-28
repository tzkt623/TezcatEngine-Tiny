#pragma once
#include "VAO.h"

namespace tezcat::Tiny::Core
{
	class Shader;

	/// <summary>
	/// 渲染对象
	/// 此类型代表需要被渲染的东西
	/// </summary>
	class RenderObject
	{
	public:
		RenderObject();
		virtual ~RenderObject();

		virtual void render(Shader* shader) = 0;
		virtual void apply() = 0;

		bool setEnable() const { return m_Enable; }
		void getEnable(bool val) { m_Enable = val; }

	private:
		bool m_Enable;
	};
}