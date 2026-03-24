#pragma once
#include "../GameLibrary/Mathematics/Vector3.h"
#include "../GameLibrary/Mathematics/Vector2.h"
#include "../Ut/Collision.h"
#include <fbxsdk.h>

#define DRAWPOS_Z_SHIFT (0.5f)	//描画の際に地面に影が埋まらないように少し上にずらす
#define DIVISION_NUM (16)	//球体の分割数
#define DECELERATION_RATE (0.97f)	//止まるまで徐々に減速する際の減速率
#define GRAVITY	(9.8)	//重力
#define LIMIT (-5.0f)	//落ちた際の死亡する高さい

enum class State
{
	move,
	damage,
	deth,
	attack,
	attackredy
};

class Object
{
protected:
	GameLib::Vector3 location;
	GameLib::Vector3 size;
	GameLib::Vector3 velocity;
	Collision collision;
	float color[4];
	double dir;
	int HP;
	std::vector<Vertex> vertices;
	std::vector<Vertex> area;
	std::vector<Vertex> enemy;
	std::vector<Vertex> enemy2;
	std::vector<Vertex> enemy3;
	std::vector<Vertex> camera;
	std::vector<Vertex> shading;
	std::vector<Vertex> wall;
	FbxNode* node;
	FbxArray<FbxString*> AnimStackNameArray;
	FbxScene* scene;
	State state;
	int sound;
	bool this_help;

public:
	Object();
	virtual ~Object();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	virtual void Update(double delta_second);
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();
	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize();

public:
	/// <summary>
	/// 座標設定処理
	/// </summary>
	/// <param name="location">座標</param>
	virtual void SetLocation(const GameLib::Vector3& location);

	/// <summary>
	/// 当たり判定用サイズ設定処理
	/// </summary>
	/// <param name="size">サイズ</param>
	virtual void SetSize(const GameLib::Vector3& size);

	/// <summary>
	/// 移動量設定処理
	/// </summary>
	/// <param name="velo">移動量</param>
	virtual void SetVelocity(const GameLib::Vector3& velo);

	/// <summary>
	/// 座標取得処理
	/// </summary>
	/// <returns>座標</returns>
	virtual GameLib::Vector3 GetLocation();

	/// <summary>
	/// 当たり判定用サイズ取得処理
	/// </summary>
	/// <returns>当たり判定用サイズ</returns>
	virtual GameLib::Vector3 GetSize();

	/// <summary>
	/// 移動量取得処理
	/// </summary>
	/// <returns>移動量</returns>
	virtual GameLib::Vector3 GetVelocity();

	/// <summary>
	/// コリジョン情報取得処理
	/// </summary>
	/// <returns>コリジョン情報</returns>
	const Collision& GetCollision() const;

	/// <summary>
	/// 現在HP取得処理
	/// </summary>
	/// <returns>HP</returns>
	virtual int GetHP() const;

	/// <summary>
	/// HP減算処理
	/// </summary>
	virtual void DecHP();

	/// <summary>
	/// FBX生成処理
	/// </summary>
	/// <param name="filename">生成するFBXファイルパス</param>
	virtual void CreateFBX(const char* filename);

	/// <summary>
	/// オブジェクトの頂点情報取得処理
	/// </summary>
	/// <returns>頂点情報</returns>
	virtual std::vector<Vertex> GetVertices();

	/// <summary>
	///	敵生成エリア情報取得処理
	/// </summary>
	/// <param name="num">3種類あるためどれか判別用</param>
	/// <returns>敵生成エリア情報</returns>
	virtual std::vector<Vertex> GetEnemyArea(int num);

	/// <summary>
	/// キャラクター状態情報取得処理
	/// </summary>
	/// <returns>状態情報</returns>
	virtual State GetState() const;

	/// <summary>
	/// キャラクター状態情報変更処理
	/// </summary>
	/// <param name="st"></param>
	virtual void ChangeState(State st);

	/// <summary>
	/// 影を描画
	/// </summary>
	/// <param name="groundZ">描画する地面の座標</param>
	virtual void DrawShadow(float groundZ);

	/// <summary>
	/// オブジェクトから伸びたレイと地面との接触判定
	/// </summary>
	/// <param name="rayOrigin">レイの原点</param>
	/// <param name="rayDir">レイの向き</param>
	/// <param name="tri">地面の情報</param>
	/// <param name="outT">どれくらいで当たったか</param>
	/// <returns>当たったかどうか</returns>
	virtual bool RayIntersectTriangle(const GameLib::Vector3& rayOrigin, const GameLib::Vector3& rayDir, const Vertex& tri, float& outT);

	/// <summary>
	/// オブジェクトの下にある地面の高さを算出
	/// </summary>
	/// <returns>オブジェクトの真下の地面の高さ</returns>
	virtual float FindGroundZFromFBX(std::vector<Vertex> triangles);

protected:

	virtual void DrawMesh();

	virtual void DrawMeshRecursive(FbxNode* node);

	virtual void SetVerticesRecursive(FbxNode* node);

	virtual void SetVertices(FbxNode* node, std::vector<Vertex>& out);

	virtual void SetVerticesShading(FbxNode* node, std::vector<Vertex>& out);
};

