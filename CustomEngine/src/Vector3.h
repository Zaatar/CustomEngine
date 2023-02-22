#pragma once
#include "Maths.h"
class Vector3
{
public:
	float x, y, z;

	Vector3() :x(0.0f), y(0.0f), z(0.0f) {}

	explicit Vector3(float xP, float yP, float zP) : x(xP), y(yP), z(zP) {}

	void set(float xP, float yP, float zP);
	float lengthSq() const;
	float length() const;
	void normalize();

	const float* getAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&x);
	}

	// Vector addition (a + b)
	friend Vector3 operator+(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	// Vector subtraction (a - b)
	friend Vector3 operator-(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	// Component-wise multiplication
	friend Vector3 operator*(const Vector3& left, const Vector3& right)
	{
		return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
	}

	// Scalar multiplication
	friend Vector3 operator*(const Vector3& vec, float scalar)
	{
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	// Scalar *=
	Vector3& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
};