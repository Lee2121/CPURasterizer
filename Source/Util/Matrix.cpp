#pragma once

#include "Matrix.hpp"
#include <cmath>

FMatrix4x4f FMatrix4x4f::Identity()
{
	return FMatrix4x4f
	{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};
}

FMatrix4x4f FMatrix4x4f::Zero()
{
	return FMatrix4x4f
	{
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f
	};
}


FMatrix4x4f FMatrix4x4f::Scale(const FVector3f& S)
{
	return FMatrix4x4f{
		S.X, 0.f, 0.f, 0.f,
		0.f, S.Y, 0.f, 0.f,
		0.f, 0.f, S.Z, 0.f,
		0.f, 0.f, 0.f, 1.f,
	};
}

FMatrix4x4f FMatrix4x4f::Scale(const float& S)
{
	return Scale({S, S, S});
}

FMatrix4x4f FMatrix4x4f::Translate(const FVector3f& T)
{
	return FMatrix4x4f{
		1.f, 0.f, 0.f, T.X,
		0.f, 1.f, 0.f, T.Y,
		0.f, 0.f, 1.f, T.Z,
		0.f, 0.f, 0.f, 1.f,
	};
}

FMatrix4x4f FMatrix4x4f::RotateXY(float Angle)
{
	float Cos = std::cos(Angle);
	float Sin = std::sin(Angle);

	return FMatrix4x4f{
		Cos, -Sin, 0.f, 0.f,
		Sin,  Cos, 0.f, 0.f,
		0.f,  0.f, 1.f, 0.f,
		0.f,  0.f, 0.f, 1.f,
	};
}

FMatrix4x4f FMatrix4x4f::RotateYZ(float Angle)
{
	float Cos = std::cos(Angle);
	float Sin = std::sin(Angle);

	return FMatrix4x4f{
		1.f, 0.f,  0.f, 0.f,
		0.f, Cos, -Sin, 0.f,
		0.f, Sin,  Cos, 0.f,
		0.f, 0.f,  0.f, 1.f,
	};
}

FMatrix4x4f FMatrix4x4f::RotateZX(float Angle)
{
	float Cos = std::cos(Angle);
	float Sin = std::sin(Angle);

	return FMatrix4x4f{
		 Cos, 0.f, Sin, 0.f,
		 0.f, 1.f, 0.f, 0.f,
		-Sin, 0.f, Cos, 0.f,
		 0.f, 0.f, 0.f, 1.f,
	};
}

//void FMatrix4x4f::SetScale(const FVector3f& S)
//{
//	// X 0 0 0
//	// 0 Y 0 0
//	// 0 0 Z 0
//	// 0 0 0 0
//	Values[0]  = S.X;
//	Values[5]  = S.Y;
//	Values[10] = S.Z;
//}
//
//void FMatrix4x4f::SetScale(const float& S)
//{
//	Scale({S, S, S});
//}
//
//void FMatrix4x4f::Translate(const FVector3f& T)
//{
//	// 0 0 0 X
//	// 0 0 0 Y
//	// 0 0 0 Z
//	// 0 0 0 0
//	FMatrix4x4f TranslationMatrix = FMatrix4x4f::Identity();
//	TranslationMatrix[3]  = T.X;
//	TranslationMatrix[7]  = T.Y;
//	TranslationMatrix[11] = T.Z;
//
//	*this = (*this * TranslationMatrix);
//}
//
//void FMatrix4x4f::RotateXY(float Angle)
//{
//	float Cos = cos(Angle);
//	float Sin = sin(Angle);
//
//	// cos -sin 0 0
//	// sin  cos 0 0
//	//  0    0  0 0
//	//  0    0  0 0
//	FMatrix4x4f RotationMatrix = FMatrix4x4f::Identity();
//	RotationMatrix[0] = Cos;
//	RotationMatrix[1] = -Sin;
//	RotationMatrix[4] = Sin;
//	RotationMatrix[5] = Cos;
//
//	*this = (*this * RotationMatrix);
//}
//
//void FMatrix4x4f::RotateYZ(float Angle)
//{
//	float Cos = cos(Angle);
//	float Sin = sin(Angle);
//
//	// 0  0    0  0
//	// 0 cos -sin 0
//	// 0 sin  cos 0
//	// 0  0    0  0
//	FMatrix4x4f RotationMatrix = FMatrix4x4f::Identity();
//	RotationMatrix[5]  = Cos;
//	RotationMatrix[6]  = -Sin;
//	RotationMatrix[9]  = Sin;
//	RotationMatrix[10] = Cos;
//
//	*this = (*this * RotationMatrix);
//}
//
//void FMatrix4x4f::RotateZX(float Angle)
//{
//	float Cos = cos(Angle);
//	float Sin = sin(Angle);
//
//	//  cos 0 sin 0
//	//   0  0  0  0
//	// -sin 0 cos 0
//	//   0  0  0  0
//	FMatrix4x4f RotationMatrix = FMatrix4x4f::Identity();
//	RotationMatrix[0]  = Cos;
//	RotationMatrix[2]  = Sin;
//	RotationMatrix[8]  = -Sin;
//	RotationMatrix[10] = Cos;
//
//	*this = (*this * RotationMatrix);
//}
