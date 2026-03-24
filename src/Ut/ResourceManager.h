#pragma once
#include <map>
#include <string>
#include <vector>
#include "../GameLibrary/Utility/Singleton.h"
#include <SDL_mixer.h>
#include <SDL2-2.0.8/include/SDL.h>
#include <GL/glut.h>

class ResourceManager : public Singleton<ResourceManager>
{
private:
	int next_id = 1;
	std::map<std::string, std::vector<GLuint>> images_container;	// 画像コンテナ
	std::map<int,  Mix_Chunk*> se_container;				// サウンドエフェクトコンテナ
	std::map<int, Mix_Music*> bgm_container;				//BGMコンテナ
	std::map<int, Mix_Chunk*> se_playing;					//再生中のSE格納

public:

	/// <summary>
	/// 音源初期化処理
	/// </summary>
	/// <returns>初期化できたか</returns>
	bool SoundInit(int frequency = 44100, Uint16 format = AUDIO_S16SYS, int channels = 2, int chunksize = 2048);

	/// <summary>
	/// 画像描画処理
	/// </summary>
	/// <param name="textureID">画像ハンドル</param>
	/// <param name="x">ｘ要素</param>
	/// <param name="y">ｙ要素</param>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	void DrawTexture(GLuint textureID, float x, float y, float width, float height);

	/// <summary>
	/// 音源再生処理
	/// </summary>
	/// <param name="handle">音源ハンドル</param>
	/// <param name="loop">ループするかどうか 0:しない -1:する</param>
	/// <param name="playback">連続再生するかどうか</param>
	void PlaySoundEffect(int handle, int loop, bool playback);

	/// <summary>
	/// BGM再生処理
	/// </summary>
	/// <param name="handle">音源ハンドル</param>
	/// <param name="loop">ループする回数　-1で最大値</param>
	void PlayBackGroundMusic(int handle, int loop);

	/// <summary>
	/// BGM停止処理
	/// </summary>
	/// <param name="handle">音源ハンドル</param>
	/// <param name="loop">ループする回数　-1で最大値</param>
	void StopBackGroundMusic();

	/// <summary>
	/// 正射影モードに変更
	/// </summary>
	/// <param name="screenWidth">ウィンドウサイズ横</param>
	/// <param name="screenHeight">ウィンドウサイズ縦</param>
	void Begin2DDraw(int screenWidth, int screenHeight);

	/// <summary>
	/// 正射影モード終了
	/// </summary>
	void End2DDraw();

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
	const std::vector<GLuint>& GetImages(std::string file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
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
	const std::vector<GLuint>& GetImages(const char* file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);

	/// <summary>
	/// 音源取得処理
	/// </summary>
	/// <param name="file_path">音源のファイルパス</param>
	/// <returns>音源ハンドルデータ</returns>
	int GetSounds(std::string file_path, bool bgmcheck);
	/// <summary>
	/// 音源取得処理
	/// </summary>
	/// <param name="file_path">音源のファイルパス</param>
	/// <returns>音源ハンドルデータ</returns>
	int GetSounds(const char* file_path, bool bgmcheck);

	/// <summary>
	/// 画像解放処理
	/// </summary>
	void UnLoadImages();

	/// <summary>
	/// 音源解放処理
	/// </summary>
	void UnLoadSounds();

private:
	/// <summary>
	/// 画像取得処理
	/// </summary>
	GLuint LoadTexture(const char* filename);

	/// <summary>
	/// 画像取得処理
	/// </summary>
	/// <param name="file">ファイルパス</param>
	/// <param name="all_num">画像の分割総数</param>
	/// <param name="num_x">縦の分割数</param>
	/// <param name="num_y">横の分割数</param>
	/// <param name="size_x">縦のサイズ</param>
	/// <param name="size_y"></param>
	/// <returns></returns>
	std::vector<GLuint> LoadTextureDiv(const char* file, int all_num, int num_x, int num_y, int size_x, int size_y);

	/// <summary>
	/// 画像読み込み処理
	/// </summary>
	/// <param name="file_path">ファイルパス</param>
	void CreateImagesResource(std::string file_path);

	/// <summary>
	/// 画像分割読み込み処理
	/// </summary>
	/// <param name="file_name">ファイルパス</param>
	/// <param name="all_num">画像の分割総数</param>
	/// <param name="num_x">横の分割</param>
	/// <param name="num_y">縦の分割</param>
	/// <param name="size_x">横のサイズ(px)</param>
	/// <param name="size_y">縦のサイズ(px)</param>
	void CreateImagesResource(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y);

	/// <summary>
	/// 特定のSEが再生中かどうかの確認処理
	/// </summary>
	/// <param name="target">ターゲットSE</param>
	/// <returns>再生中かどうか</returns>
	bool IsPlayingSE(Mix_Chunk* target);

};