#include "mat4.h"
#include <cmath>
#include <iostream>


namespace maths
{
	Mat4 const Mat4::identity = {
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	auto Mat4::Perspective(float width, float height, float fov, float near, float far) -> Mat4
	{
		float aspectRatio = width / height;
		float d = 1.f / tanf(fov / 2.f);
		float range = 1.f / (near - far);

		return {
			d / aspectRatio, 0.f, 0.f, 0.f,
			0.f, d, 0.f, 0.f,
			0.f, 0.f, (near + far) * range, -1.f,
			0.f, 0.f, near * far * 2.f * range, 0.f
		};
	}

	auto Mat4::Translate(float x, float y, float z) -> Mat4
	{
		return{
			1.f,	0.f,	0.f,	0.f,
			0.f,	1.f,	0.f,	0.f,
			0.f,	0.f,	1.f,	0.f,
			x,		y,		z,		1.f
		};
	}

	auto Mat4::Scale(float x, float y, float z) -> Mat4
	{
		return{
			x,		0.f,	0.f,	0.f,
			0.f,	y,		0.f,	0.f,
			0.f,	0.f,	z,		0.f,
			0.f,	0.f,	0.f,	1.f
		};
	}

	auto Mat4::Scale(float s) -> Mat4
	{
		return{
			s,		0.f,	0.f,	0.f,
			0.f,	s,		0.f,	0.f,
			0.f,	0.f,	s,		0.f,
			0.f,	0.f,	0.f,	1.f
		};
	}

	auto Mat4::RotateY(float angle) -> Mat4
	{
		return{
			cos(angle), 0.f, -sin(angle), 0.f, 		// 1ere colonne
			0.f, 1.f, 0.f, 0.f,						// 2eme colonne
			sin(angle), 0.f, cos(angle), 0.f,		// 3eme colonne
			0.f, 0.f, 0.f, 1.f						// 4eme colonne
		};
	}

	auto Mat4::operator* (Mat4 const& mat) const	-> Mat4
	{
		Mat4 res = {	0.f, 0.f, 0.f, 0.f,
						0.f, 0.f, 0.f, 0.f,
						0.f, 0.f, 0.f, 0.f,
						0.f, 0.f, 0.f, 0.f
		};

		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				for (int k = 0; k < 4; ++k)
					res.val[4* j + i] += this->val[4 * k + i] * mat.val[4 * j + k];
		return res;
	}

	/*auto Mat4::operator*= (Mat4 const& mat) const -> Mat4&
	{
		*this = *this * mat;
		return *this;
	}*/

	auto Mat4::Inverse()->Mat4
	{
		Mat4 inv = *this;

		for (int i = 3; i < 12; i += 4)
			inv.val[i] *= -1;

		std::swap(inv.val[1], inv.val[4]);
		std::swap(inv.val[2], inv.val[8]);
		std::swap(inv.val[6], inv.val[9]);
		return inv;
	}
}