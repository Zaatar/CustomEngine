#include <fstream>
#include <iostream>
#include <sstream>

#include "Assets.h"
#include "Log.h"

std::map<std::string, Texture> Assets::textures;

Texture Assets::loadTexture(IRenderer& renderer, const string& filename, const string& name)
{
	textures[name] = loadTextureFromFile(renderer, filename.c_str());
	return textures[name];
}

Texture& Assets::getTexture(const string& name)
{
	if (textures.find(name) == end(textures))
	{
		std::ostringstream loadError;
		loadError << "Texture " << name << "does not exist in assets manager";
		Log::error(LogCategory::Application, loadError.str());
	}
	return textures[name];
}

void Assets::clear()
{
	// (Properly) delete all textures
	for (auto iter : textures)
		iter.second.unload();
	textures.clear();

	// (Properly) delete all shaders
	//for (auto iter : shaders)
	//	iter.second.unload();
	//shaders.clear();
}

Texture Assets::loadTextureFromFile(IRenderer& renderer, const string& filename)
{
	Texture texture;
	// texture.load(renderer, filename);
	if (renderer.type() == IRenderer::Type::SDL)
	{
		texture.load(dynamic_cast<RendererSDL&>(renderer), filename);
	}
	return texture;
}

//Shader Assets::loadShader(const std::string& vertexShaderFileP,
//	const std::string& fragmentShaderFileP, 
//	const std::string& tesselationControlShaderFileP,
//	const std::string& tesselationEvaluationShaderFileP, 
//	const std::string& geometryShaderFileP,
//	const std::string& nameP)
//{
//	shaders[nameP] = loadShaderFromFile(vertexShaderFileP, fragmentShaderFileP,
//		tesselationControlShaderFileP, tesselationEvaluationShaderFileP,
//		geometryShaderFileP);
//	return shaders[nameP];
//}
//
//Shader& Assets::getShader(const std::string& name)
//{
//	if (shaders.find(name) == end(shaders))
//	{
//		std::ostringstream loadError;
//		loadError << "Shader " << name << " does not exist in assets manager.";
//		Log::error(LogCategory::Application, loadError.str());
//	}
//	return shaders[name];
//}
//
//Shader Assets::loadShaderFromFile(const std::string& vertexShaderFileP,
//	const std::string& fragmentShaderFileP, 
//	const std::string& tesselationControlShaderFileP,
//	const std::string& tesselationEvaluationShaderFileP, 
//	const std::string& geometryShaderFileP)
//{
//	// 1. Retrieve the vertex / fragment source code from filePath
//	std::string vertexCode;
//	std::string fragmentCode;
//	std::string tesselationControlCode;
//	std::string tesselationEvaluationCode;
//	std::string geometryCode;
//
//	try
//	{
//		// Open Files
//		std::ifstream vertexShaderFile(vertexShaderFileP);
//		std::ifstream fragmentShaderFile(fragmentShaderFileP);
//		std::stringstream vertexShaderStream, fragmentShaderStream;
//		// Read files buffer contents into streams
//		vertexShaderStream << vertexShaderFile.rdbuf();
//		fragmentShaderStream << fragmentShaderFile.rdbuf();
//		// close file handlers
//		vertexShaderFile.close();
//		fragmentShaderFile.close();
//		// Convert stream to string
//		vertexCode = vertexShaderStream.str();
//		fragmentCode = fragmentShaderStream.str();
//		// If tesselation control shader path is present, also load a tesselation
//		// control shader
//		if (tesselationControlShaderFileP != "")
//		{
//			std::ifstream tesselationControlShaderFile(tesselationControlShaderFileP);
//			std::stringstream tesselationControlShaderStream;
//			tesselationControlShaderStream << tesselationControlShaderFile.rdbuf();
//			tesselationControlShaderFile.close();
//			tesselationControlCode = tesselationControlShaderStream.str();
//		}
//		// If tesselation evaluation shader path is present, also load a tesselation
//		// evaluation shader
//		if (tesselationEvaluationShaderFileP != "")
//		{
//			std::ifstream tesselationEvaluationShaderFile(tesselationEvaluationShaderFileP);
//			std::stringstream tesselationEvaluationShaderStream;
//			tesselationEvaluationShaderStream << tesselationEvaluationShaderFile.rdbuf();
//			tesselationEvaluationShaderFile.close();
//			tesselationControlCode = tesselationEvaluationShaderStream.str();
//		}
//		// If geometry shader path is present, also load a geomtry shader
//		if (geometryShaderFileP != "")
//		{
//			std::ifstream geometryShaderFile(geometryShaderFileP);
//			std::stringstream geometryShaderStream;
//			geometryShaderStream << geometryShaderFile.rdbuf();
//			geometryShaderFile.close();
//			geometryCode = geometryShaderStream.str();
//		}
//	}
//	catch (std::exception e)
//	{
//		std::ostringstream loadError;
//		std::string geometryShaderFile = "";
//		if (geometryShaderFile != "")
//			geometryShaderFile = geometryShaderFileP;
//
//		loadError << "ERROR::SHADER: Failed to read shader files " << vertexShaderFileP <<
//			" " << fragmentShaderFileP << " " << geometryShaderFileP << "\n"
//			<< "\n - -------------------------------- --" << std::endl;
//		Log::error(LogCategory::Render, loadError.str());
//	}
//
//	const GLchar* vertexShaderCode = vertexCode.c_str();
//	const GLchar* fragmentShaderCode = fragmentCode.c_str();
//	const GLchar* tesselationControlShaderCode = tesselationControlCode.c_str();
//	const GLchar* tesselationEvaluationShaderCode = tesselationControlCode.c_str();
//	const GLchar* geometryShaderCode = geometryCode.c_str();
//
//	// 2. Now create shader object from source code
//	Shader shader;
//	shader.compile(vertexShaderCode, fragmentShaderCode,
//		tesselationControlShaderFileP != "" ? tesselationControlShaderCode : nullptr,
//		tesselationEvaluationShaderFileP != "" ? tesselationEvaluationShaderCode : nullptr,
//		geometryShaderFileP != "" ? geometryShaderCode : nullptr);
//	return shader;
//}