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
#include "Core/Head/CppHead.h"

namespace tezcat::Tiny
{
	struct TINY_API GaussianMatrix
	{
		static float gaussian(float x, float mu, float sigma)
		{
			const float a = (x - mu) / sigma;
			return (float)std::exp(-0.5f * a * a);
		}

		static std::vector<float> calculate(int kernelRadius, int& length)
		{
			const float PI = 3.14159265357f;
			length = (2 * kernelRadius + 1);
			std::vector<float> kernel_matrix(length * length);

			float sigma = kernelRadius / 2.0f;
			float sum = 0.0; // For accumulating the kernel values
			for (int y = 0; y < length; ++y)
			{
				for (int x = 0; x < length; ++x)
				{
					kernel_matrix[x + y * length] = gaussian((float)x, (float)kernelRadius, sigma) * gaussian((float)y, (float)kernelRadius, sigma);
					//kernel_matrix[x + y * kernelRadius] = exp(-0.5 * (pow((x - mean) / sigma, 2.0) + pow((y - mean) / sigma, 2.0)))/ (2 * PI * sigma * sigma);

					sum += kernel_matrix[x + y * length];
				}
			}

			// Normalize the kernel
			for (auto& v : kernel_matrix)
			{
				v /= sum;
			}

			return kernel_matrix;
		}

		static std::vector<float> calculateHalf(int kernelRadius, int& length)
		{
			const float PI = 3.14159265357f;
			length = 2 * kernelRadius + 1;
			std::vector<float> kernel_matrix(length);

			float sigma = kernelRadius / 2.0f;
			float sum = 0.0; // For accumulating the kernel values

			float x_sum = 0;

			for (int y = 0; y < length; ++y)
			{
				for (int x = 0; x < length; ++x)
				{
					float value = gaussian((float)x, (float)kernelRadius, sigma) * gaussian((float)y, (float)kernelRadius, sigma);
					x_sum += value;

					sum += value;
				}

				kernel_matrix[y] = x_sum;
				x_sum = 0;
			}

			// Normalize the kernel
			for (auto& v : kernel_matrix)
			{
				v /= sum;
			}

			return kernel_matrix;
		}
	};
}
