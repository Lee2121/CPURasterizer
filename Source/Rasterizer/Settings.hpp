#pragma once

namespace Rasterizer
{
	enum class ECullMode
	{
		None,
		Clockwise,
		CounterClockWise,
	};

	enum class EDepthPassMode
	{
		Never,
		Always,
		Less,
		LessEqual,
		Greater,
		GreaterEqual,
		Equal,
		NotEqual,
	};

	struct FDepthPassSettings
	{
		bool bWrite = true;
		EDepthPassMode Mode = EDepthPassMode::Less;
	};
}