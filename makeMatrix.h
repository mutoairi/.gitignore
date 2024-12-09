#pragma once
#include <math.h>
#include<cmath>
#define _USE_MATH_DEFINES

struct Vector4 final
{
	float x;
	float y;
	float z;
	float w;
};
struct Vector3 {
	float x;
	float y;
	float z;
};
struct Vector2 {
	float x;
	float y;
};
struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
struct Matrix3x3 {
	float m[3][3];
};
struct Matrix4x4 {
	float m[4][4];
};
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
struct TransformationMatrix
{
	Matrix4x4 WVP;
	Matrix4x4 World;
};

struct DirectionalLight
{
	Vector4 color;
	Vector3 direction;
	float intensity;

};
// 行列の掛け算
Matrix4x4 MatrixMultiply(Matrix4x4 m1, Matrix4x4 m2);
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

Matrix4x4 MakeScaleMatrix(Vector3& scale);

Matrix4x4 MakeRotateZMatrix(float& rot);

Matrix4x4 MakeTranslateMatrix(Vector3& translate);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);



Matrix4x4 MakeIdentity4x4();

Matrix4x4 Inverse(const Matrix4x4& m);


