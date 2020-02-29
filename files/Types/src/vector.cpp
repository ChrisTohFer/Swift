#include "vector.h"
#include <math.h>

namespace st {

	//VECTOR3

	template<typename TYPE>
	VECTOR3<TYPE> VECTOR3<TYPE>::zero()
	{
		return VECTOR3();
	}
	template<typename TYPE>
	VECTOR3<TYPE> VECTOR3<TYPE>::unit_x()
	{
		return VECTOR3(1, 0, 0);
	}
	template<typename TYPE>
	VECTOR3<TYPE> VECTOR3<TYPE>::unit_y()
	{
		return VECTOR3(0, 1, 0);
	}
	template<typename TYPE>
	VECTOR3<TYPE> VECTOR3<TYPE>::unit_z()
	{
		return VECTOR3(0, 0, 1);
	}

	template<typename TYPE>
	VECTOR3<TYPE>::VECTOR3() = default;

	template<typename TYPE>
	VECTOR3<TYPE>::VECTOR3(TYPE x_in, TYPE y_in, TYPE z_in)
		: x(x_in)
		, y(y_in)
		, z(z_in)
	{}

	template<typename TYPE>
	VECTOR3<TYPE> VECTOR3<TYPE>::operator+(const VECTOR3& other) const
	{
		return VECTOR3(
			x + other.x,
			y + other.y,
			z + other.z
		);
	}
	template<typename TYPE>
	VECTOR3<TYPE> VECTOR3<TYPE>::operator-(const VECTOR3& other) const
	{
		return VECTOR3(
			x - other.x,
			y - other.y,
			z - other.z
		);
	}
	template<typename TYPE>
	VECTOR3<TYPE>& VECTOR3<TYPE>::operator+=(const VECTOR3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	template<typename TYPE>
	VECTOR3<TYPE>& VECTOR3<TYPE>::operator-=(const VECTOR3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	template<typename TYPE>
	VECTOR3<TYPE> VECTOR3<TYPE>::operator+(TYPE value) const
	{
		return VECTOR3(
			x + value,
			y + value,
			z + value
		);
	}
	template<typename TYPE>
	VECTOR3<TYPE> VECTOR3<TYPE>::operator-(TYPE value) const
	{
		return VECTOR3(
			x - value,
			y - value,
			z - value
		);
	}
	template<typename TYPE>
	VECTOR3<TYPE> VECTOR3<TYPE>::operator*(TYPE value) const
	{
		return VECTOR3(
			x * value,
			y * value,
			z * value
		);
	}
	template<typename TYPE>
	VECTOR3<TYPE> VECTOR3<TYPE>::operator/(TYPE value) const
	{
		return VECTOR3(
			x / value,
			y / value,
			z / value
		);
	}
	template<typename TYPE>
	VECTOR3<TYPE>& VECTOR3<TYPE>::operator+=(TYPE value)
	{
		x += value;
		y += value;
		z += value;
		return *this;
	}
	template<typename TYPE>
	VECTOR3<TYPE>& VECTOR3<TYPE>::operator-=(TYPE value)
	{
		x -= value;
		y -= value;
		z -= value;
		return *this;
	}
	template<typename TYPE>
	VECTOR3<TYPE>& VECTOR3<TYPE>::operator*=(TYPE value)
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}
	template<typename TYPE>
	VECTOR3<TYPE>& VECTOR3<TYPE>::operator/=(TYPE value)
	{
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}

	template<>
	float VECTOR3<float>::abs() const
	{
		return sqrtf(
			x * x +
			y * y +
			z * z
		);
	}
	template<>
	double VECTOR3<double>::abs() const
	{
		return sqrt(
			x * x +
			y * y +
			z * z
		);
	}
	template<typename TYPE>
	VECTOR3<TYPE> VECTOR3<TYPE>::normalize() const
	{
		TYPE magnitude = abs();
		if (magnitude == 0)
			return zero();
		else
			return *this / abs();
	}

	//VECTOR2

	template<typename TYPE>
	VECTOR2<TYPE> VECTOR2<TYPE>::zero()
	{
		return VECTOR2();
	}
	template<typename TYPE>
	VECTOR2<TYPE> VECTOR2<TYPE>::unit_x()
	{
		return VECTOR2(1, 0);
	}
	template<typename TYPE>
	VECTOR2<TYPE> VECTOR2<TYPE>::unit_y()
	{
		return VECTOR2(0, 1);
	}

	template<typename TYPE>
	VECTOR2<TYPE>::VECTOR2() = default;

	template<typename TYPE>
	VECTOR2<TYPE>::VECTOR2(TYPE x_in, TYPE y_in)
		: x(x_in)
		, y(y_in)
	{}

	template<typename TYPE>
	VECTOR2<TYPE> VECTOR2<TYPE>::operator+(const VECTOR2& other) const
	{
		return VECTOR2(
			x + other.x,
			y + other.y
		);
	}
	template<typename TYPE>
	VECTOR2<TYPE> VECTOR2<TYPE>::operator-(const VECTOR2& other) const
	{
		return VECTOR2(
			x - other.x,
			y - other.y
		);
	}
	template<typename TYPE>
	VECTOR2<TYPE>& VECTOR2<TYPE>::operator+=(const VECTOR2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	template<typename TYPE>
	VECTOR2<TYPE>& VECTOR2<TYPE>::operator-=(const VECTOR2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template<typename TYPE>
	VECTOR2<TYPE> VECTOR2<TYPE>::operator+(TYPE value) const
	{
		return VECTOR2(
			x + value,
			y + value
		);
	}
	template<typename TYPE>
	VECTOR2<TYPE> VECTOR2<TYPE>::operator-(TYPE value) const
	{
		return VECTOR2(
			x - value,
			y - value
		);
	}
	template<typename TYPE>
	VECTOR2<TYPE> VECTOR2<TYPE>::operator*(TYPE value) const
	{
		return VECTOR2(
			x * value,
			y * value
		);
	}
	template<typename TYPE>
	VECTOR2<TYPE> VECTOR2<TYPE>::operator/(TYPE value) const
	{
		return VECTOR2(
			x / value,
			y / value
		);
	}
	template<typename TYPE>
	VECTOR2<TYPE>& VECTOR2<TYPE>::operator+=(TYPE value)
	{
		x += value;
		y += value;
		return *this;
	}
	template<typename TYPE>
	VECTOR2<TYPE>& VECTOR2<TYPE>::operator-=(TYPE value)
	{
		x -= value;
		y -= value;
		return *this;
	}
	template<typename TYPE>
	VECTOR2<TYPE>& VECTOR2<TYPE>::operator*=(TYPE value)
	{
		x *= value;
		y *= value;
		return *this;
	}
	template<typename TYPE>
	VECTOR2<TYPE>& VECTOR2<TYPE>::operator/=(TYPE value)
	{
		x /= value;
		y /= value;
		return *this;
	}

	template<>
	float VECTOR2<float>::abs() const
	{
		return sqrtf(
			x * x +
			y * y
		);
	}
	template<>
	double VECTOR2<double>::abs() const
	{
		return sqrt(
			x * x +
			y * y
		);
	}
	template<typename TYPE>
	VECTOR2<TYPE> VECTOR2<TYPE>::normalize() const
	{
		TYPE magnitude = abs();
		if (magnitude == 0)
			return zero();
		else
			return *this / abs();
	}

	//Specialize types for common use cases
	template struct VECTOR3<float>;
	template struct VECTOR3<double>;
	
	template struct VECTOR2<float>;
	template struct VECTOR2<double>;

}

