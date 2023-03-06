#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <map>
#include <string>

using std::map;
using std::string;

/*
A static singleton Assets class that hosts several functions to load resources.
Each loaded resource is also stored for future reference by string handles.
All functions and resources are static and no public constructor is defined.
*/

class Assets
{
public:
	static std::map<std::string, Texture> textures;
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Mesh> meshes;

	// Loads a texture from file
	static Texture loadTexture(IRenderer& renderer, const string& filename, const string& name);

	// Loads and generates a shader program from file loading vertex, fragment 
	// ( and tesselation control, evaluation, geometry) shader's source code.
	// If tesselationControlShaderFile, tesselationEvaluationShaderFile, geometryShaderFile,
	// are not nullptr, it also loads the corresponding shaders.
	static Shader loadShader(const std::string& vertexShaderFile,
		const std::string& fragmentShaderFile,
		const std::string& tesselationControlShaderFile,
		const std::string& tesselationEvaluationShaderFile,
		const std::string& geometryShaderFile, const std::string& name);

	// Retrieves a stored shader
	static Shader& getShader(const std::string& name);

	// Retrieves a stored texture
	static Texture& getTexture(const std::string& name);

	// Loads a mesh from file
	static Mesh loadMesh(const string& filenameP, const string& name);

	// Retrieves a stored mesh
	static Mesh& getMesh(const std::string& name);

	// Properly de-allocates all loaded resources
	static void clear();

private:
	Assets() {}

	// Loads a single texture from a file
	static Texture loadTextureFromFile(IRenderer& renderer, const string& filename);

	// Loads and generates a shader from file
	static Shader loadShaderFromFile(const std::string& vShaderFile,
		const std::string& fShaderFile, 
		const std::string& tesselationControlShaderFile = "",
		const std::string& tesselationEvaluationShaderFile = "",
		const std::string& geometryShaderFile = "");

	static Mesh loadMeshFromFile(const string& filename);
};
