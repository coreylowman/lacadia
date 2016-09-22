#ifndef TEXT_SHADER_H
#define TEXT_SHADER_H

#include "engine/graphics/shader.h"
#include "engine/util/vec3.h"

#define MAX_CHARS 64
#define MAX_CHAR_VERTS (6 * MAX_CHARS)

typedef struct CharacterStats {
  unsigned int textureID;
  int size[2];
  int bearing[2];
  unsigned int advance;
} CharacterStats;

typedef struct TextCharacterVertex {
  float x, y, u, v;
  Vec3 color;
} TextCharacterVertex;

typedef struct TextShader {
  Shader base_shader;

  CharacterStats char_stats[128];
  int num_character_vertices[128];
  TextCharacterVertex characters[128][MAX_CHAR_VERTS];
  unsigned int vbo, vao;
} TextShader;

TextShader *text_shader_new();
void text_shader_free(TextShader *self);
void text_shader_add_text(TextShader *self, const char *buffer, int len,
                          Vec3 xyscale, Vec3 color);

#endif
