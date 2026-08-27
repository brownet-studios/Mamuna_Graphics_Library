#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

#include <Mamuna.h>
#include <stdio.h>

typedef struct Mamuna_Image {
	unsigned char* pixels;
	int w, h;
	int channels;
} Mamuna_Image;

typedef struct Mamuna_Font {
	Mamuna_Image* image;
	stbtt_packedchar* charData;
	size_t count;
	int sizeFont;
} Mamuna_Font;

char* Mamuna_LoadTextFile(const char* path){
	if(!path && (path[0] == EOF || path[0] == '\0')){
		return NULL;
	}

	FILE* file = fopen(path, "rb");
	if(!file) return NULL;
	fseek(file, 0, SEEK_END);

	size_t len = ftell(file);
	if(len == 0){
		fclose(file);
		return NULL;
	}

	char* buff = malloc(sizeof(char) * (len + 1));
	buff[len] = '\0';
	
	rewind(file);
	fread(buff, sizeof(char), len, file);

	fclose(file);

	return buff;
}

Mamuna_Image* Mamuna_CreateImage(unsigned char* pixels, int w, int h, int channels){
	Mamuna_Image* image = malloc(sizeof(Mamuna_Image));

	image->pixels = malloc(sizeof(unsigned char) * w * h * channels);
	memcpy(image->pixels, pixels, sizeof(unsigned char) * w * h * channels);
	image->w = w;
	image->h = h;
	image->channels = channels;

	return image;
}


Mamuna_Image* Mamuna_LoadImage(const char* path){
	Mamuna_Image* image = malloc(sizeof(Mamuna_Image));
	image->pixels = stbi_load(path, &image->w, &image->h, &image->channels, 0);

	return image;
}

void Mamuna_UnloadImage(Mamuna_Image* image){
	stbi_image_free(image->pixels);
	free(image);
}

Mamuna_Font* Mamuna_LoadFont(const char* path, int pixelHeight, size_t countChars){
	if(!path){
		return NULL;
	}

	FILE* file = fopen(path, "rb");
	if(!file) return NULL;
	fseek(file, 0, SEEK_END);

	size_t len = ftell(file);
	if(len == 0){
		fclose(file);
		return NULL;
	}	

	unsigned char* buff = malloc(sizeof(unsigned char) * (len + 1));
	buff[len] = '\0';
	
	rewind(file);
	fread(buff, sizeof(unsigned char), len, file);

	fclose(file);
	Mamuna_Font* font = malloc(sizeof(Mamuna_Font));
	font->image = malloc(sizeof(Mamuna_Image));
	font->image->pixels = calloc(2048 * 2048, sizeof(unsigned char));
	font->image->w = 2048;
	font->image->h = 2048; 
	font->charData = calloc(countChars, sizeof(stbtt_packedchar));
	

	stbtt_pack_context spc;
	int res = stbtt_PackBegin(&spc, font->image->pixels, font->image->w, font->image->h,
			0, 1, NULL);
	if(!res){

		return NULL;
	}

	stbtt_PackSetOversampling(&spc, 2, 2);


	res = stbtt_PackFontRange(&spc, buff, 0, pixelHeight, 32, countChars, font->charData);
	if(!res){

		return NULL;
	}	

	stbtt_PackEnd(&spc);
	font->sizeFont = pixelHeight;
	font->image->channels = 1;
	font->count = countChars;
	free(buff);
	return font;
}

void Mamuna_DestroyFont(Mamuna_Font* font){
	free(font->image->pixels);
	free(font->image);
	free(font->charData);
	free(font);
}

Mamuna_Image* Mamuna_FontGetBitmap(Mamuna_Font* font){
	return font->image;
}

void Mamuna_FontGetUVsChar(Mamuna_Font* font, char32_t c, float* cursorX, float sizeFont, float baselineY, vec4 uv, vec4 pos){
	int index = c - 32;	
	if(font->count <= index || index < 0) return;
	stbtt_packedchar* glyph = &font->charData[index];
	
	uv[0] = (float)glyph->x0 / 2048.f;
	uv[1] = (float)glyph->y0 / 2048.f;
	uv[2] = (float)glyph->x1 / 2048.f;
	uv[3] = (float)glyph->y1 / 2048.f;

	pos[0] = *cursorX + (sizeFont * glyph->xoff);
	pos[1] = baselineY + (sizeFont * glyph->yoff);
	pos[2] = *cursorX + (sizeFont * glyph->xoff2);
	pos[3] = baselineY + (sizeFont * glyph->yoff2);

	*cursorX += (glyph->xadvance * sizeFont);
}

float Mamuna_FontGetSizeXChar(Mamuna_Font* font, char32_t c){
	if(!font || c < 32) return 0.f;
	return font->charData[c - 32].xadvance; 
}

void Mamuna_GetImageSize(Mamuna_Image* image, int* w, int* h){
	*w = image->w;
	*h = image->h;
}

int Mamuna_GetImageChannels(Mamuna_Image* image){
	return image->channels;
}

unsigned char* Mamuna_GetImagePixels(Mamuna_Image* image){
	return image->pixels;
}
