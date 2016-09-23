#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "engine/util/object_model.h"

#define MAX_TEXTURES 32
#define MAX_MODELS 32

typedef struct TextureAsset {
    unsigned int width, height;
    unsigned char *data;
} TextureAsset;

typedef struct AssetManager {
    int num_textures;
    const char *texture_names[MAX_TEXTURES];
    TextureAsset textures[MAX_TEXTURES];

    int num_models;
    const char *model_names[MAX_MODELS];
    ObjectModel *models[MAX_MODELS];
} AssetManager;

AssetManager *asset_manager_new(const char *asset_root_path);
void asset_manager_free(AssetManager *self);

int asset_manager_get_texture_id(AssetManager *self, const char *asset_name);
int asset_manager_get_model_id(AssetManager *self, const char *asset_name);

TextureAsset asset_manager_get_texture(AssetManager *self, int asset_id);
ObjectModel *asset_manager_get_model(AssetManager *self, int asset_id);

#endif
