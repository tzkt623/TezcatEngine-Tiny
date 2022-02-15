#pragma once

#include <string>

namespace tezcat::Tiny::Core
{
	struct TinyParameter
	{
		//-----------------------------
		//����Shader����
		const static char* TINY_Default_Shader;

		//----------------------------
		//Shader���ò���
		const static char* MatP;
		const static char* MatV;
		const static char* MatM;
		const static char* MatN;
		const static char* VertexColor;
		const static char* TexColor;

		const static char* LightPosition;
		const static char* LightColor;
		const static char* ViewPosition;
		const static char* AmbientStrength;
	}; 

}

