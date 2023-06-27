#pragma once

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
			const float PI = 3.14159265357;
			length = (2 * kernelRadius + 1);
			std::vector<float> kernel_matrix(length * length);

			float sigma = kernelRadius / 2.0f;
			float sum = 0.0; // For accumulating the kernel values
			for (int y = 0; y < length; ++y)
			{
				for (int x = 0; x < length; ++x)
				{
					kernel_matrix[x + y * length] = gaussian(x, kernelRadius, sigma) * gaussian(y, kernelRadius, sigma);
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
	};
}
