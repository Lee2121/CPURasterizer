#pragma once

namespace Rasterizer
{
	template <typename T>
	struct TAttribute
	{
		const void* Pointer = nullptr;
		uint32_t Stride = sizeof(T);

		// Get attribute by vertex index
		const T& operator[](uint32_t i) const
		{
			return *(const T*)((const char*)(Pointer) + Stride * i);
		}
	};
}