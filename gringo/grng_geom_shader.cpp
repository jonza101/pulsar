
#include "grng_geom_shader.h"


void					grng_geom_shader::copy_assign(const grng_geom_shader &s)
{
	this->shader_blob = s.shader_blob;
	this->shader = s.shader;
	if (this->shader_blob)
		this->shader_blob->AddRef();
	if (this->shader)
		this->shader->AddRef();
}


grng_geom_shader		&grng_geom_shader::operator=(const grng_geom_shader &s)
{
	if (this->shader_blob != s.shader_blob)
		grng_shader::remove_shader_memory();
	if (this->shader != s.shader)
		this->remove_geom_shader_memory();

	this->copy_assign(s);

	return (*this);
}

grng_geom_shader::grng_geom_shader(const grng_geom_shader &s) : GRNG_SHADER()
{
	this->type = GRNG_BINDABLE_TYPE::GEOM_SHADER;
	this->copy_assign(s);
}

grng_geom_shader::grng_geom_shader() : GRNG_SHADER()
{
	this->type = GRNG_BINDABLE_TYPE::GEOM_SHADER;
}

grng_geom_shader::grng_geom_shader(const LPCWSTR shader_file, const LPCSTR entry, const D3D_SHADER_MACRO *defines) : GRNG_SHADER()
{
	this->type = GRNG_BINDABLE_TYPE::GEOM_SHADER;
	this->set_shader_memory(shader_file, entry, defines);
}

grng_geom_shader::~grng_geom_shader()
{
	this->remove_shader_memory();
}
