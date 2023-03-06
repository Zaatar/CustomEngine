#include <fstream>
#include <iostream>
#include <rapidjson/document.h>
#include <sstream>

#include "Assets.h"
#include "Log.h"

std::map<std::string, Texture> Assets::textures;
std::map<std::string, Shader> Assets::shaders;
std::map<std::string, Mesh> Assets::meshes;

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
	for (auto iter : shaders)
		iter.second.unload();
	shaders.clear();

	// (Properly) delete all meshes
	for (auto iter : meshes)
		iter.second.unload();
	meshes.clear();
}

Texture Assets::loadTextureFromFile(IRenderer& renderer, const string& filename)
{
	Texture texture;
	// texture.load(renderer, filename);
	if (renderer.type() == IRenderer::Type::SDL)
	{
		texture.loadSDL(dynamic_cast<RendererSDL&>(renderer), filename);
	}
	else if (renderer.type() == IRenderer::Type::OGL)
	{
		texture.loadOGL(dynamic_cast<RendererOGL&>(renderer), filename);
	}
	return texture;
}

Mesh Assets::loadMeshFromFile(const string& filename)
{
	Mesh mesh;

	std::ifstream file(filename);
	if (!file.is_open())
	{
		Log::error(LogCategory::Application, "File not found: Mesh " + filename);
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		std::ostringstream s;
		s << "Mesh " << filename << " is not valid json.";
		Log::error(LogCategory::Application, s.str());
	}

	mesh.setShaderName(doc["shader"].GetString());

	// Skip the vertex format/shader for now
	// (This is changed in a later chapter's code)
	size_t vertSize = 8;

	// Load textures
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		std::ostringstream s;
		s << "Mesh " << filename << " has no textures, there should be at least one";
		Log::error(LogCategory::Application, s.str());
	}

	// mesh.setSpecularPower(static_cast<float>(doc["specularPower"].GetDouble()));

	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		std::string texName = textures[i].GetString();
		Texture& t = getTexture(texName);
		mesh.addTexture(&t);
	}

	// Load in the vertices
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		std::ostringstream s;
		s << "Mesh " << filename << " has no vertices";
		Log::error(LogCategory::Application, s.str());
	}

	std::vector<float> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	float radius = 0.0f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// For now just assume we have 8 elements
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray() || vert.Size() != 8)
		{
			std::ostringstream s;
			s << "Unexpected vertex format for " << filename;
			Log::error(LogCategory::Application, s.str());
		}

		Vector3 pos(static_cast<float>(vert[0].GetDouble()),
			static_cast<float>(vert[1].GetDouble()),
			static_cast<float>(vert[2].GetDouble()));

		radius = Maths::max(radius, pos.lengthSq());

		// Add the floats
		for (rapidjson::SizeType i = 0; i < vert.Size(); i++)
		{
			vertices.emplace_back(static_cast<float>(vert[i].GetDouble()));
		}
	}

	// We were computing length squared earlier
	mesh.setRadius(Maths::sqrt(radius));

	// Load in the indices
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		std::ostringstream s;
		s << "Mesh " << filename << " has no indices";
		Log::error(LogCategory::Application, s.str());
	}

	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3.0);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			std::ostringstream s;
			s << "Invalid indices for " << filename;
			Log::error(LogCategory::Application, s.str());
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	// Now create a vertex array
	mesh.setVertexArray(new VertexArray(vertices.data(),
		static_cast<unsigned int>(vertices.size()) / vertSize,
		indices.data(), static_cast<unsigned int>(indices.size())));

	Log::info("Loaded mesh " + filename);
	return mesh;
}

Shader Assets::loadShader(const std::string& vertexShaderFileP,
	const std::string& fragmentShaderFileP, 
	const std::string& tesselationControlShaderFileP,
	const std::string& tesselationEvaluationShaderFileP, 
	const std::string& geometryShaderFileP,
	const std::string& nameP)
{
	shaders[nameP] = loadShaderFromFile(vertexShaderFileP, fragmentShaderFileP,
		tesselationControlShaderFileP, tesselationEvaluationShaderFileP,
		geometryShaderFileP);
	return shaders[nameP];
}

Mesh Assets::loadMesh(const string& filename, const string& name)
{
	meshes[name] = loadMeshFromFile(filename);
	return meshes[name];
}

Shader& Assets::getShader(const std::string& name)
{
	if (shaders.find(name) == end(shaders))
	{
		std::ostringstream loadError;
		loadError << "Shader " << name << " does not exist in assets manager.";
		Log::error(LogCategory::Application, loadError.str());
	}
	return shaders[name];
}

Mesh& Assets::getMesh(const std::string& name)
{
	if (meshes.find(name) == end(meshes))
	{
		std::ostringstream loadError;
		loadError << "Mesh " << name << " does not exist in assets manager.";
		Log::error(LogCategory::Application, loadError.str());
	}
	return meshes[name];
}

Shader Assets::loadShaderFromFile(const std::string& vertexShaderFileP,
	const std::string& fragmentShaderFileP, 
	const std::string& tesselationControlShaderFileP,
	const std::string& tesselationEvaluationShaderFileP, 
	const std::string& geometryShaderFileP)
{
	// 1. Retrieve the vertex / fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string tesselationControlCode;
	std::string tesselationEvaluationCode;
	std::string geometryCode;

	try
	{
		// Open Files
		std::ifstream vertexShaderFile(vertexShaderFileP);
		std::ifstream fragmentShaderFile(fragmentShaderFileP);
		std::stringstream vertexShaderStream, fragmentShaderStream;
		// Read files buffer contents into streams
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream to string
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
		// If tesselation control shader path is present, also load a tesselation
		// control shader
		if (tesselationControlShaderFileP != "")
		{
			std::ifstream tesselationControlShaderFile(tesselationControlShaderFileP);
			std::stringstream tesselationControlShaderStream;
			tesselationControlShaderStream << tesselationControlShaderFile.rdbuf();
			tesselationControlShaderFile.close();
			tesselationControlCode = tesselationControlShaderStream.str();
		}
		// If tesselation evaluation shader path is present, also load a tesselation
		// evaluation shader
		if (tesselationEvaluationShaderFileP != "")
		{
			std::ifstream tesselationEvaluationShaderFile(tesselationEvaluationShaderFileP);
			std::stringstream tesselationEvaluationShaderStream;
			tesselationEvaluationShaderStream << tesselationEvaluationShaderFile.rdbuf();
			tesselationEvaluationShaderFile.close();
			tesselationControlCode = tesselationEvaluationShaderStream.str();
		}
		// If geometry shader path is present, also load a geomtry shader
		if (geometryShaderFileP != "")
		{
			std::ifstream geometryShaderFile(geometryShaderFileP);
			std::stringstream geometryShaderStream;
			geometryShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = geometryShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::ostringstream loadError;
		std::string geometryShaderFile = "";
		if (geometryShaderFile != "")
			geometryShaderFile = geometryShaderFileP;

		loadError << "ERROR::SHADER: Failed to read shader files " << vertexShaderFileP <<
			" " << fragmentShaderFileP << " " << geometryShaderFileP << "\n"
			<< "\n - -------------------------------- --" << std::endl;
		Log::error(LogCategory::Render, loadError.str());
	}

	const GLchar* vertexShaderCode = vertexCode.c_str();
	const GLchar* fragmentShaderCode = fragmentCode.c_str();
	const GLchar* tesselationControlShaderCode = tesselationControlCode.c_str();
	const GLchar* tesselationEvaluationShaderCode = tesselationControlCode.c_str();
	const GLchar* geometryShaderCode = geometryCode.c_str();

	// 2. Now create shader object from source code
	Shader shader;
	shader.compile(vertexShaderCode, fragmentShaderCode,
		tesselationControlShaderFileP != "" ? tesselationControlShaderCode : nullptr,
		tesselationEvaluationShaderFileP != "" ? tesselationEvaluationShaderCode : nullptr,
		geometryShaderFileP != "" ? geometryShaderCode : nullptr);
	return shader;
}