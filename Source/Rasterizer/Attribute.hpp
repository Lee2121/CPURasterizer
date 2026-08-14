#pragma once

namespace Rasterizer
{
	template <typename T>
	struct TAttribute
	{
		const void* Pointer = nullptr;
		size_t Stride = sizeof(T);

		// Get attribute by vertex index
		const T& operator[](size_t i) const
		{
			return *(const T*)((const char*)(Pointer) + Stride * i);
		}
	};
}