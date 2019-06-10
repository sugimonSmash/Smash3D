/**
* @file Font.h
*/
#ifndef  FONT_H_INCLUDED
#define  FONT_H_INCLUDED
#include <GL/glew.h>
#include "Sprite.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

/**
* �r�b�g�}�b�v�t�H���g�`��N���X.
*/
class FontRenderer
{
public:
	FontRenderer() = default;
	~FontRenderer() = default;
	FontRenderer(const FontRenderer&) = delete;
	FontRenderer& operator=(const FontRenderer&) = delete;
	
	bool Init(size_t maxChar);
	bool LoadFromFile(const char* path);
	void BeginUpdate();
	bool AddString(const glm::vec2&, const wchar_t*);
	void EndUpdate();
	void Draw(const glm::vec2&) const;
	float LineHeight() const;

	/// �t�H���g�̐F��ύX����
	void Color(const glm::vec4& c) { color = glm::clamp(c, 0.0f, 1.0f) * 255.0f; }
	void Scale(const glm::vec2& s) { scale = s; }

private:
	SpriteRenderer spriteRenderer;	///< �`��Ɏg�p����X�v���C�g�����_���[.
	std::vector<Texture::Image2Dptr> textures; ///< �t�H���g�e�N�X�`�����X�g.
	float lineHeight = 0;			///< �s�̍���.
	float base = 0;					///< �s�̏㕔����x�[�X���C���܂ł̋���.
	
	glm::vec4 color = glm::vec4(255, 255, 255, 255); ///< �t�H���g��`�悷��Ƃ��̐F.
	glm::vec2 scale = glm::vec2(1); ///< �t�H���g��`�悷��Ƃ��̃X�P�[��.


	/// �������.
	struct CharacterInfo {
		int id = -1;					///< �����R�[�h.
		int page = 0;					///< �������܂܂��t�H���g�e�N�X�`���̔ԍ�.
		glm::vec2 uv = glm::vec2(0);	///< �����̃e�N�X�`�����W.
		glm::vec2 size = glm::vec2(0);	///< �����̕\���T�C�Y.
		glm::vec2 offset = glm::vec2(0);///< �\���ʒu�̕␳�l.
		float xadvance = 0;				///< ���̕�����\������ʒu.
	};
	std::vector<CharacterInfo> characterInfoList; ///< �������̃��X�g.
};

#endif // ! FONT_H_INCLUDED
