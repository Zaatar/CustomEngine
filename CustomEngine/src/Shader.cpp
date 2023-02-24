#include <SDL2/SDL.h>
#include <sstream>
#include <string>

#include "Log.h"
#include "Shader.h"

void Shader::unload()
{
	glDeleteProgram(id);
}

Shader& Shader::use()
{
	glUseProgram(id);
	return *this;
}

void Shader::compile(const GLchar* vertexSource, const GLchar* fragmentSource,
	const GLchar* tesselationControlSource, const GLchar* tesselationEvaluationSource,
	const GLchar* geometrySource)
{
	compileVertexShader(vertexSource);
	bool tesselationExists = compileTesselationControlShader(tesselationControlSource);
	tesselationExists &= compileTesselationEvaluationShader(tesselationEvaluationSource);
	bool geometryShaderExists = compileGeometryShader(geometrySource);
	compileFragmentShader(fragmentSource);
	createShaderProgram(tesselationExists, geometryShaderExists);
	printAllParams(id);
}

void Shader::compileVertexShader(const GLchar* vertexSourceP)
{
	vertexSource = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexSource, 1, &vertexSourceP, NULL);
	glCompileShader(vertexSource);
	checkShaderErrors(vertexSource, "Vertex");
}

void Shader::compileFragmentShader(const GLchar* fragmentSourceP)
{
	fragmentSource = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentSource, 1, &fragmentSourceP, NULL);
	glCompileShader(fragmentSource);
	checkShaderErrors(fragmentSource, "Fragment");
}

bool Shader::compileTesselationControlShader(const GLchar* tesselationControlSourceP)
{
	if (tesselationControlSourceP == nullptr) { return false; }

	tesselationControlSource = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(tesselationControlSource, 1, &tesselationControlSourceP, NULL);
	glCompileShader(tesselationControlSource);
	checkShaderErrors(tesselationControlSource, "Tesselation Control");
	return true;
}

bool Shader::compileTesselationEvaluationShader(const GLchar* tesselationEvaluationSourceP)
{
	if (tesselationEvaluationSourceP == nullptr) { return false; }

	tesselationEvaluationSource = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(tesselationEvaluationSource, 1, &tesselationEvaluationSourceP, NULL);
	glCompileShader(tesselationEvaluationSource);
	checkShaderErrors(tesselationEvaluationSource, "Tesselation Evaluation");
	return true;
}

bool Shader::compileGeometryShader(const GLchar* geometrySourceP)
{
	if (geometrySourceP == nullptr) { return false; }

	geometrySource = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometrySource, 1, &geometrySourceP, NULL);
	glCompileShader(geometrySource);
	checkShaderErrors(geometrySource, "Geometry");
	return true;
}

void Shader::createShaderProgram(bool tesselationShaderExistsP, bool geometryShaderExistsP)
{
	// Create program
	id = glCreateProgram();
	glAttachShader(id, fragmentSource);
	
	if (tesselationShaderExistsP)
	{
		glAttachShader(id, tesselationControlSource);
		glAttachShader(id, tesselationEvaluationSource);
	}

	if (geometryShaderExistsP)
	{
		glAttachShader(id, geometrySource);
	}

	glAttachShader(id, vertexSource);
	glLinkProgram(id);

	// Check for linking error
	int params = -1;
	glGetProgramiv(id, GL_LINK_STATUS, &params);
	
	if (params != GL_TRUE)
	{
		std::ostringstream s;
		s << "Could not link shader program GL index " << id;
		Log::error(LogCategory::Render, s.str());
		printProgramInfoLog(id);
	}

	if (!isValid(id))
	{
		std::ostringstream s;
		s << "Could not validate shader " << id;
		Log::error(LogCategory::Render, s.str());
	}

	// Delete shaders if they are no longer used
	glDeleteShader(vertexSource);
	
	if (tesselationShaderExistsP)
	{
		glDeleteShader(tesselationControlSource);
		glDeleteShader(tesselationEvaluationSource);
	}

	if (geometryShaderExistsP)
	{
		glDeleteShader(geometrySource);
	}

	glDeleteShader(fragmentSource);
}

void Shader::setFloat(const GLchar* name, GLfloat value)
{
	glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::setInteger(const GLchar* name, GLint value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::setVector2f(const GLchar* name, GLfloat x, GLfloat y)
{
	glUniform2f(glGetUniformLocation(id, name), x, y);
}

void Shader::setVector2f(const GLchar* name, const Vector2& value)
{
	glUniform2f(glGetUniformLocation(id, name), value.x, value.y);
}


void Shader::setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z)
{
	glUniform3f(glGetUniformLocation(id, name), x, y, z);
}

void Shader::setVector3f(const GLchar* name, const Vector3& value)
{
	glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z);
}
/*
void Shader::setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
}

void Shader::setVector4f(const GLchar* name, const Vector4& value)
{
	glUniform4f(glGetUniformLocation(id, name), value.x, value.y, value.z, value.w);
}
*/
void Shader::setMatrix4(const GLchar* name, const Matrix4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_TRUE, matrix.getAsFloatPtr());
}


void Shader::printShaderInfoLog(GLuint shaderIndex)
{
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetShaderInfoLog(id, max_length, &actual_length, log);
	std::ostringstream s;
	s << "Shader info log for GL index " << shaderIndex;
	Log::info(s.str());
}

void Shader::printProgramInfoLog(GLuint id)
{
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetProgramInfoLog(id, max_length, &actual_length, log);
	std::ostringstream s;
	s << "Program info log for GL index " << id;
	Log::info(s.str());
}

void Shader::checkShaderErrors(GLuint shader, std::string shaderType)
{
	int params = -1;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
	if (params != GL_TRUE)
	{
		std::ostringstream s;
		s << "GL " << shaderType << " shader index " << shader << " did not compile.";
		Log::error(LogCategory::Render, s.str());
		printShaderInfoLog(shader);
	}
}

const char* Shader::GLTypeToString(GLenum type)
{
	switch (type)
	{
	case GL_BOOL:
		return "bool";
	case GL_INT:
		return "int";
	case GL_FLOAT:
		return "float";
	case GL_FLOAT_VEC2:
		return "vec2";
	case GL_FLOAT_VEC3:
		return "vec3";
	case GL_FLOAT_VEC4:
		return "vec4";
	case GL_SAMPLER_2D:
		return "sampler2D";
	case GL_SAMPLER_3D:
		return "sampler3D";
	case GL_SAMPLER_CUBE:
		return "samplerCuber";
	case GL_SAMPLER_2D_SHADOW:
		return "sampler2DShadow";
	default:
		break;
	}
	return "other";
}

void Shader::printAllParams(GLuint id)
{
	Log::info("-------------------------");
	std::ostringstream s;
	s << "Shader Program " << id << " info: ";
	Log::info(s.str());
	int params = -1;
	glGetProgramiv(id, GL_LINK_STATUS, &params);
	s.str("");
	s << "GL LINK_STATUS = " << params;
	Log::info(s.str());

	glGetProgramiv(id, GL_ATTACHED_SHADERS, &params);
	s.str("");
	s << "GL_ATTACHED_SHADERS = " << params;
	Log::info(s.str());

	glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &params);
	s.str("");
	s << "GL_ACTIVE_ATTRIBUTES = " << params;
	Log::info(s.str());

	for (GLuint i = 0; i < (GLuint)params; i++)
	{
		char name[64];
		int max_length = 64;
		int actual_length = 0;
		int size = 0;
		GLenum type;
		glGetActiveAttrib(id, i, max_length, &actual_length, &size, &type, name);
		if (size > 1)
		{
			for (int j = 0; j < size; j++)
			{
				char long_name[77];
				sprintf_s(long_name, "%s[%i]", name, j);
				int location = glGetAttribLocation(id, long_name);
				std::ostringstream s;
				s << " " << i << ") type: " << GLTypeToString(type) << " name: " <<
					long_name << " location: " << location;
				Log::info(s.str());
			}
		}
		else
		{
			int location = glGetAttribLocation(id, name);
			std::ostringstream s;
			s << " " << i << ") type: " << GLTypeToString(type) << " name: " <<
				name << " location: " << location;
			Log::info(s.str());
		}
	}

	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &params);
	s.str("");
	s << "GL_ACTIVE_UNIFORMS = " << params;
	Log::info(s.str());
	for (GLuint i = 0; i < (GLuint)params; i++)
	{
		char name[64];
		int max_length = 64;
		int actual_length = 0;
		int size = 0;
		GLenum type;
		glGetActiveUniform(id, i, max_length, &actual_length, &size, &type, name);
		if (size > 1)
		{
			for (int j = 0; j < size; j++)
			{
				char long_name[77];
				sprintf_s(long_name, "%s[%i]", name, j);
				int location = glGetUniformLocation(id, long_name);
				std::ostringstream s;
				s << " " << i << ") type: " << GLTypeToString(type) << " name: " <<
					long_name << " location: " << location;
				Log::info(s.str());
			}
		}
		else
		{
			int location = glGetUniformLocation(id, name);
			std::ostringstream s;
			s << " " << i << ") type: " << GLTypeToString(type) << " name: " << name <<
				" location: " << location;
			Log::info(s.str());
		}
	}
	printProgramInfoLog(id);
}

bool Shader::isValid(GLuint id)
{
	glValidateProgram(id);
	int params = -1;
	glGetProgramiv(id, GL_VALIDATE_STATUS, &params);
	Log::info("");
	std::ostringstream s;
	s << "Program " << id << " GL_VALIDATE_STATUS = " << params;
	Log::info(s.str());
	if (params != GL_TRUE)
	{
		printProgramInfoLog(id);
		return false;
	}
	return true;
}