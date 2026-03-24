#include "ResourceManager.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool ResourceManager::SoundInit(int frequency, Uint16 format, int channels, int chunksize)
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0) return false;

	if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0)
		return false;

	Mix_AllocateChannels(16);  // チャンネル数、必要なら増やす
	return true;
}

void ResourceManager::DrawTexture(GLuint textureID, float x, float y, float width, float height)
{
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(x, y);
	glTexCoord2f(1, 0); glVertex2f(x + width, y);
	glTexCoord2f(1, 1); glVertex2f(x + width, y + height);
	glTexCoord2f(0, 1); glVertex2f(x, y + height);
	glEnd();
}

void ResourceManager::PlaySoundEffect(int handle, int loop, bool playback)
{
	//音源コンテナ内を確認
	std::map<int, Mix_Chunk*>::iterator it = se_container.find(handle);
	if (it == se_container.end()) return;
	Mix_Chunk* chunk = it->second;

	// -1 = 自動で空いているチャンネルで再生
	if (!playback)
	{
		if (!IsPlayingSE(chunk))
		{
			int ch = Mix_PlayChannel(-1, chunk, loop);
			if (ch != -1)
			{
				se_playing[ch] = chunk;
			}
		}
	}
	else
	{
		int ch = Mix_PlayChannel(-1, chunk, loop);
		if (ch != -1)
		{
			se_playing[ch] = chunk;
		}
	}
}

void ResourceManager::PlayBackGroundMusic(int handle, int loop)
{
	std::map<int, Mix_Music*>::iterator it = bgm_container.find(handle);
	if (it == bgm_container.end()) return;
	Mix_Music* chunk = it->second;

	Mix_PlayMusic(chunk, loop);
}

void ResourceManager::StopBackGroundMusic()
{
	Mix_HaltMusic();
}

void ResourceManager::Begin2DDraw(int screenWidth, int screenHeight)
{
	glPushAttrib(GL_ENABLE_BIT | GL_TRANSFORM_BIT | GL_VIEWPORT_BIT);
	glDisable(GL_DEPTH_TEST);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, screenWidth, screenHeight, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void ResourceManager::End2DDraw()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();
}

GLuint ResourceManager::LoadTexture(const char* filename)
{
	int width, height, channels;

	unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
	if (!image) return 0;

	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(image);
	return texID;
}

std::vector<GLuint> ResourceManager::LoadTextureDiv(const char* file,int all_num,int num_x,int num_y,int size_x,int size_y)
{
	std::vector<GLuint> textures;

	int w, h, channels;
	unsigned char* src = stbi_load(file, &w, &h, &channels, STBI_rgb_alpha);
	if (!src) return textures;

	for (int n = 0; n < all_num; n++)
	{
		int ix = (n % num_x) * size_x;
		int iy = (n / num_x) * size_y;

		std::vector<unsigned char> part(size_x * size_y * 4);

		for (int y = 0; y < size_y; y++)
		{
			memcpy(
				&part[y * size_x * 4],
				&src[((iy + y) * w + ix) * 4],
				size_x * 4
			);
		}

		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			size_x, size_y, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, part.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		textures.push_back(texID);
	}

	stbi_image_free(src);
	return textures;
}

/// <summary>
/// 画像取得処理
/// </summary>
/// <param name="file_name">ファイルパス</param>
/// <param name="all_num">画像の分割総数</param>
/// <param name="num_x">横の分割</param>
/// <param name="num_y">縦の分割</param>
/// <param name="size_x">横のサイズ(px)</param>
/// <param name="size_y">縦のサイズ(px)</param>
/// <returns>読み込んだ画像ハンドルのvector配列</returns>
const std::vector<GLuint>& ResourceManager::GetImages(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y)
{
	// コンテナ内に画像データが無い場合、画像読み込みを行う
	if (images_container.count(file_name) == 0)
	{
		// 分割読み込みを行う？行わない？
		if (all_num == 1)
		{
			// 分割読み込みを行わない
			CreateImagesResource(file_name);
		}
		else
		{
			// 分割読み込みを行う
			CreateImagesResource(file_name, all_num, num_x, num_y, size_x, size_y);
		}
	}

	// コンテナ内のデータを返す
	return images_container[file_name];
}
const std::vector<GLuint>& ResourceManager::GetImages(const char* file_name, int all_num, int num_x, int num_y, int size_x, int size_y)
{
	return GetImages(std::string(file_name), all_num, num_x, num_y, size_x, size_y);
}

/// <summary>
/// 音源取得処理
/// </summary>
/// <param name="file_path">音源のファイルパス</param>
/// <returns>音源ハンドルデータ</returns>
int ResourceManager::GetSounds(std::string file_path, bool bgmcheck)
{
	int id;
	if (bgmcheck)
	{
		Mix_Music* music = Mix_LoadMUS(file_path.c_str());
		if (!music) return -1;

		id = next_id++;

		bgm_container[id] = music;

	}
	else
	{
		Mix_Chunk* chunk = Mix_LoadWAV(file_path.c_str());
		if (!chunk) return -1;

		id = next_id++;

		se_container[id] = chunk;
	}

	return id;
}
int ResourceManager::GetSounds(const char* file_path, bool bgmcheck)
{
	return GetSounds(std::string(file_path), bgmcheck);
}

/// <summary>
/// 画像解放処理
/// </summary>
void ResourceManager::UnLoadImages()
{
	// コンテナ内に画像が無ければ、処理を終了する
	if (images_container.size() == NULL)
	{
		return;
	}

	// 全ての画像を削除
	for (auto& p : images_container)
	{
		for (auto tex : p.second)
			glDeleteTextures(1, &tex);
	}

	// コンテナを解放
	images_container.clear();
}

/// <summary>
/// 音源解放処理
/// </summary>
void ResourceManager::UnLoadSounds()
{
	// コンテナ内に音源が無ければ、処理を終了する
	if (se_container.size() == NULL)
	{
		return;
	}

	// 全ての音源を削除
	for (std::pair<const int, Mix_Chunk*>& p : se_container)
	{
		Mix_FreeChunk(p.second);
	}

	// コンテナを解放
	se_container.clear();
	Mix_CloseAudio();
	SDL_Quit();
}

/// <summary>
/// 画像読み込み処理
/// </summary>
/// <param name="file_path">ファイルパス</param>
void ResourceManager::CreateImagesResource(std::string file_path)
{
	// 指定された画像ファイル読み込む
	GLuint handle = LoadTexture(file_path.c_str());
	// エラーチェック
	if (handle == -1)
	{
		throw (file_path + "がありません\n");
	}

	// コンテナに読み込んだ画像を追加する
	images_container[file_path].push_back(handle);
}

/// <summary>
/// 画像分割読み込み処理
/// </summary>
/// <param name="file_name">ファイルパス</param>
/// <param name="all_num">画像の分割総数</param>
/// <param name="num_x">横の分割</param>
/// <param name="num_y">縦の分割</param>
/// <param name="size_x">横のサイズ(px)</param>
/// <param name="size_y">縦のサイズ(px)</param>
void ResourceManager::CreateImagesResource(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y)
{
	auto texs = LoadTextureDiv(file_name.c_str(), all_num, num_x, num_y, size_x, size_y);
	if (texs.empty())
		throw (file_name + " がありません\n");

	images_container[file_name] = texs;
}

/// <summary>
/// 特定のSEが再生中かどうかの確認処理
/// </summary>
/// <param name="target">ターゲットSE</param>
/// <returns>再生中かどうか</returns>
bool ResourceManager::IsPlayingSE(Mix_Chunk* target)
{
	for (auto& pair : se_playing)
	{
		int ch = pair.first;
		Mix_Chunk* chunk = pair.second;

		//ターゲットと同じSEがse_playing内にあり、そのチャンネルが再生中だったら
		if (chunk == target && Mix_Playing(ch))
		{
			return true;
		}
		else if (chunk == target && !Mix_Playing(ch))
		{
			se_playing.erase(ch);
			return false;
		}
	}
	return false;
}
