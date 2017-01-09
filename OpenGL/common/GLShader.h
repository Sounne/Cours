#pragma once

#include <cstdint>

class GLShader
{
public:
	GLShader() = default;
	~GLShader() = default;

	auto LoadShader(uint32_t type, const char* path) -> bool;
	auto Create() -> bool;
	auto Bind() -> void;
	auto Unbind() -> void;

	inline auto Get() -> uint32_t { return _Program; }

private:
	uint32_t _Program;
	uint32_t _VertexShader;
	uint32_t _FragmentShader;
};