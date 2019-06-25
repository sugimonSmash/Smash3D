/**
* @file Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <algorithm>
#include <memory>
#include <vector>

namespace Texture {

	class Image2D;
	using Image2Dptr = std::shared_ptr<Image2D>;

	/**
	* 画像データ.
	*/
	class ImageData {
	public:
		glm::vec4 GetPixelColor(int x, int y) const;

	public:
		GLint width = 0; ///< 横のピクセル数.
		GLint height = 0; ///< 縦のピクセル数.
		GLenum format = GL_NONE; ///< 記録されている色の種類.
		GLenum type = GL_NONE; ///< それぞれの色情報のビット配置.
		std::vector<uint8_t> data; ///< バイトデータ.
	};

	GLuint CreateImage2D(GLsizei width, GLsizei height, const GLvoid* data, GLenum format, GLenum type);
	GLuint LoadImage2D(const char* path);
	bool LoadImage2D(const char* path, ImageData* imageData);

	/**
	*
	*/
	class Image2D
	{
	public:
		static Image2Dptr Create(const char*);
		Image2D() = default;
		explicit Image2D(GLuint texId);
		~Image2D();
		void Reset(GLuint texId);
		bool IsNull() const;
		GLint Width() const { return width; }
		GLint Height() const { return height; }
		GLuint GetId() const { if (!id) { return 0; } else { return id; } }
		GLenum Target() const;
		void Bind(int no) const;
		void Unbind(int no) const;

	private:
		GLuint id = 0;
		GLint width = 0;
		GLint height = 0;
		bool isCubemap = false;
	};

} // namespace Texture

#endif // TEXTURE_H_INCLUDED