#pragma once

#ifndef __MAT4_H_INCLUDED__
#define __MAT4_H_INCLUDED__

namespace maths
{
	struct Mat4
	{
		float	val[16];

		static Mat4 const identity;

		auto Perspective(float, float, float, float, float) -> Mat4;
		auto Translate(float, float, float) -> Mat4;
		auto Scale(float, float, float) -> Mat4;
		auto Scale(float) -> Mat4;
		auto RotateY(float) -> Mat4;

		auto Inverse() -> Mat4;

		auto Mat4::operator* (Mat4 const&) const -> Mat4;
		//auto Mat4::operator*= (Mat4 const&) const -> Mat4&;
	};
}

#endif // __MAT4_H_INCLUDED__