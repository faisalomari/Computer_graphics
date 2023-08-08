#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <glad/glad.h>
#include <string>
#include <vector>
using std::string;

class Texture2D
{
public:
	Texture2D();
	virtual ~Texture2D();

	bool loadTexture(const string& fileName, bool generateMipMaps = true);
	bool loadTextureCube(const string& fileName, bool generateMipMaps);
	unsigned int Texture2D::loadCubemap(std::vector<std::string> faces);
	void bind(GLuint texUnit = 0);
	void unbind(GLuint texUnit = 0);

private:
	Texture2D(const Texture2D& rhs) {}
	Texture2D& operator = (const Texture2D& rhs) {}

	GLuint mTexture;
	GLuint mTexture2;
	GLuint mTexture3;




};
#endif