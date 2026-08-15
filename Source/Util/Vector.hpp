#pragma once

struct FVector4f
{
	FVector4f() {};
	FVector4f(float InX, float InY, float InZ, float InW)
		: X(InX), Y(InY), Z(InZ), W(InW)
	{
	}

	float X = 0;
	float Y = 0;
	float Z = 0;
	float W = 0;

	inline float& operator[](size_t Index)
	{
		switch (Index)
		{
		case 0:	return X;
		case 1:	return Y;
		case 2:	return Z;
		case 3:	return W;
		default: throw;
		}
	}

	inline const float& operator[](size_t Index) const
	{
		switch (Index)
		{
		case 0:	return X;
		case 1:	return Y;
		case 2:	return Z;
		case 3:	return W;
		default: throw;
		}
	}

	inline FVector4f PerspectiveDivide()
	{
		FVector4f Return = *this;
		Return.X /= Return.W;
		Return.Y /= Return.W;
		Return.Z /= Return.W;
		return Return;
	}
};

inline FVector4f operator- (const FVector4f& v0, const FVector4f& v1)
{
	return { v0.X - v1.X, v0.Y - v1.Y, v0.Z - v1.Z, v0.W - v1.W };
}

inline FVector4f operator+ (const FVector4f& v0, const FVector4f& v1)
{
	return { v0.X + v1.X, v0.Y + v1.Y, v0.Z + v1.Z, v0.W + v1.W };
}

inline FVector4f operator* (const FVector4f& v0, const float& f)
{
	return { v0.X * f, v0.Y * f, v0.Z * f, v0.W * f };
}

inline float Determinant2D(const FVector4f& v0, const FVector4f& v1)
{
	return v0.X * v1.Y - v0.Y * v1.X;
}

struct FVector3f
{
	float X = 0;
	float Y = 0;
	float Z = 0;

	inline FVector4f AsVector4f(const float w = 0.f) const
	{
		return { X, Y, Z, w };
	}

	inline float& operator[](size_t Index)
	{
		switch (Index)
		{
		case 0:	return X;
		case 1:	return Y;
		case 2:	return Z;
		default: throw;
		}
	}

	inline const float& operator[](size_t Index) const
	{
		switch (Index)
		{
		case 0:	return X;
		case 1:	return Y;
		case 2:	return Z;
		default: throw;
		}
	}
};

inline FVector3f operator- (const FVector3f& v0, const FVector3f& v1)
{
	return { v0.X - v1.X, v0.Y - v1.Y, v0.Z - v1.Z };
}