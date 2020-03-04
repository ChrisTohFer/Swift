#pragma once

namespace st {

	//generic 3d vector type
	template<typename TYPE>
	struct VECTOR3 {
		static VECTOR3 zero();
		static VECTOR3 unit_x();
		static VECTOR3 unit_y();
		static VECTOR3 unit_z();

		VECTOR3();
		VECTOR3(TYPE x_in, TYPE y_in, TYPE z_in);

		VECTOR3 operator-() const;

		VECTOR3 operator+(const VECTOR3&) const;
		VECTOR3 operator-(const VECTOR3&) const;
		VECTOR3& operator+=(const VECTOR3&);
		VECTOR3& operator-=(const VECTOR3&);

		VECTOR3 operator+(TYPE value) const;
		VECTOR3 operator-(TYPE value) const;
		VECTOR3 operator*(TYPE value) const;
		VECTOR3 operator/(TYPE value) const;
		VECTOR3& operator+=(TYPE value);
		VECTOR3& operator-=(TYPE value);
		VECTOR3& operator*=(TYPE value);
		VECTOR3& operator/=(TYPE value);

		//TODO: comparison and static generation

		TYPE abs() const;
		VECTOR3 normalize() const;

		TYPE x = 0;
		TYPE y = 0;
		TYPE z = 0;
	};

	//generic 2d vector type
	template<typename TYPE>
	struct VECTOR2 {
		static VECTOR2 zero();
		static VECTOR2 unit_x();
		static VECTOR2 unit_y();

		VECTOR2();
		VECTOR2(TYPE x_in, TYPE y_in);

		VECTOR2 operator-() const;

		VECTOR2 operator+(const VECTOR2&) const;
		VECTOR2 operator-(const VECTOR2&) const;
		VECTOR2& operator+=(const VECTOR2&);
		VECTOR2& operator-=(const VECTOR2&);

		VECTOR2 operator+(TYPE value) const;
		VECTOR2 operator-(TYPE value) const;
		VECTOR2 operator*(TYPE value) const;
		VECTOR2 operator/(TYPE value) const;
		VECTOR2& operator+=(TYPE value);
		VECTOR2& operator-=(TYPE value);
		VECTOR2& operator*=(TYPE value);
		VECTOR2& operator/=(TYPE value);

		TYPE abs() const;
		VECTOR2 normalize() const;

		TYPE x = 0;
		TYPE y = 0;
	};

	//typenames for common use cases
	using VECTOR3F = VECTOR3<float>;
	using VECTOR3D = VECTOR3<double>;
	
	using VECTOR2F = VECTOR2<float>;
	using VECTOR2D = VECTOR2<double>;
}