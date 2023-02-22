#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <fstream>
#include <string>

#include "Vector2.h"

// General purpose shader object, compiles from file, 
// generates compile/link-time error messages and hosts several utility
// functions for easy management

class Shader
{
public:
	// State
	GLuint id;

	// Constructor
	Shader() : 
		id(0), vertexSource(0), fragmentSource(0), geometrySource(0),
		tesselationControlSource(0), tesselationEvaluationSource(0) {}

	// Unload the shader
	void unload();

	// Sets the current shader as active
	Shader& use();

	// Compiles the shader from given source code
	void compile(
		const GLchar* vertexSource,
		const GLchar* fragmentSource,
		const GLchar* tesselationControlSource = nullptr,
		const GLchar* tesselationEvaluationSource = nullptr,
		const GLchar* geometrySource = nullptr
	);

	// Utility functions
	void setFloat(const GLchar* name, GLfloat value);
	void setInteger(const GLchar* name, GLint value);
	void setVector2f(const GLchar* name, GLfloat x, GLfloat y);
	void setVector2f(const GLchar* name, const Vector2& value);

	/*
	void setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
	void setVector3f(const GLchar* name, const Vector3& value);
	void setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void setVector4f(const GLchar* name, const Vector4& value);
	void setMatrix4(const GLchar* name, const Matrix4& matrix);
	*/

private:
	GLuint vertexSource = 0;
	GLuint fragmentSource;
	GLuint tesselationControlSource;
	GLuint tesselationEvaluationSource;
	GLuint geometrySource;

	void compileVertexShader(const GLchar* vertexSource);
	void compileFragmentShader(const GLchar* fragmentSource);
	bool compileTesselationControlShader(const GLchar* tesselationControlSource);
	bool compileTesselationEvaluationShader(const GLchar* tesselationEvaluationSource);
	bool compileGeometryShader(const GLchar* geometryShader);
	void createShaderProgram(bool tesselationShadersExist, bool geometryShaderExists);

	void checkShaderErrors(GLuint shader, std::string shaderType);
	void printShaderInfoLog(GLuint shaderIndex);
	void printProgramInfoLog(GLuint program);
	const char* GLTypeToString(GLenum type);
	void printAllParams(GLuint program);
	bool isValid(GLuint program);


};