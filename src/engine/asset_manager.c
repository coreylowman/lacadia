#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "lodepng.h"
#include "asset_manager.h"

static int load_png_data(const char *filename, TextureAsset *texture) {
	unsigned error;
	unsigned char *png = 0;
	size_t pngsize;

	error = lodepng_load_file(&png, &pngsize, filename);
	if (!error)
		error = lodepng_decode32(&texture->data, &texture->width, &texture->height, png, pngsize);
	if (error) {
		printf("load_png_data: error %u: %s\n", error, lodepng_error_text(error));
		free(png);
		return 0;
	}

	free(png);
	return 1;
}

static int load_textures(AssetManager *self, char *dir_path, int dir_name_len) {
	DIR *directory;
	struct dirent *entry;
	int result;

	char name_buffer[128] = { 0 };

	directory = opendir(dir_path);
	if (directory != NULL) {
		while ((entry = readdir(directory)) != NULL) {
			switch (entry->d_type) {
			case DT_REG:
			{
				char *extension = strchr(entry->d_name, '.');
				if (strcmp(extension, ".png") == 0) {
					self->texture_names[self->num_textures] = strdup(entry->d_name);

					memset(name_buffer, 0, 128 * sizeof(char));
					strcpy(name_buffer, dir_path);
					strcat(name_buffer, "/");
					strcat(name_buffer, entry->d_name);
					printf("Loading texture %s\n", name_buffer);
					load_png_data(name_buffer, &self->textures[self->num_textures++]);
				}
				break;
			}
			case DT_DIR:
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
					continue;
				}
				strcat(dir_path, entry->d_name);
				result = load_textures(self, dir_path, dir_name_len + entry->d_namlen);
				dir_path[dir_name_len] = 0;
				if (result) {
					return result;
				}
				break;
			default:
				break;
			}
		}
	} else {
		printf("load_textures: Cannot open directory %s\n", dir_path);
		return 1;
	}

	return 0;
}

static int load_models(AssetManager *self, char *dir_path, int dir_name_len) {
	DIR *directory;
	struct dirent *entry;
	int result;

	char name_buffer[128] = { 0 };

	directory = opendir(dir_path);
	if (directory != NULL) {
		while ((entry = readdir(directory)) != NULL) {
			switch (entry->d_type) {
			case DT_REG:
			{
				char *extension = strchr(entry->d_name, '.');
				if (strcmp(extension, ".obj") == 0) {
					self->model_names[self->num_models] = strdup(entry->d_name);

					memset(name_buffer, 0, 128 * sizeof(char));
					strcpy(name_buffer, dir_path);
					strcat(name_buffer, "/");
					extension[0] = 0;
					strcat(name_buffer, entry->d_name);
					extension[0] = '.';
					printf("Loading model %s\n", name_buffer);
					self->models[self->num_models++] = obj_model_from_file(name_buffer);
				}
				break;
			}
			case DT_DIR:
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
					continue;
				}
				strcat(dir_path, entry->d_name);
				result = load_models(self, dir_path, dir_name_len + entry->d_namlen);
				dir_path[dir_name_len] = 0;
				if (result) {
					return result;
				}
				break;
			default:
				break;
			}
		}
	} else {
		printf("load_models: Cannot open directory %s\n", dir_path);
		return 1;
	}

	return 0;
}

AssetManager *asset_manager_new(const char *asset_root_path) {
	AssetManager *self = malloc(sizeof(*self));

	char name_buffer[128] = { 0 };
	strcpy(name_buffer, asset_root_path);

	self->num_textures = 0;
	int result;
	result = load_textures(self, name_buffer, strlen(name_buffer));
	if (result) {
		printf("Failed loading textures\n");
		exit(result);
	}

	memset(name_buffer, 0, 128 * sizeof(char));
	strcpy(name_buffer, asset_root_path);
	self->num_models = 0;
	result = load_models(self, name_buffer, strlen(name_buffer));
	if (result) {
		printf("Failed loading models\n");
		exit(result);
	}

	return self;
}

void asset_manager_free(AssetManager *self) {
	int i;
	for (i = 0; i < self->num_textures; i++) {
		free(self->textures[i].data);
		free(self->texture_names[i]);
	}

	for (i = 0; i < self->num_models; i++) {
		free(self->model_names[i]);
		obj_model_free(self->models[i]);
	}

	free(self);
}

int asset_manager_get_texture_id(AssetManager *self, const char *asset_name);
int asset_manager_get_model_id(AssetManager *self, const char *asset_name);

unsigned char *asset_manager_get_texture(AssetManager *self, int asset_id);
ObjectModel *asset_manager_get_model(AssetManager *self, int asset_id);
