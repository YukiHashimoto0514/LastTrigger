//@file GameObject.h
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include "../Engine/Sprite.h"
#include "../Engine/Rect.h"
#include "Component.h"
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <type_traits>

//先行宣言
class Engine;
class GameObject;
class MeshRenderer;
class ProgramPipeline;

using GameObjectPtr = std::shared_ptr<GameObject>;		//ゲームオブジェクトのポインタ
using GameObjectList = std::vector<GameObjectPtr>;		//ゲームオブジェクトのポインタの配列
using MeshRendererPtr = std::shared_ptr<MeshRenderer>;	//描画コンポーネント

//ゲーム内に登場する様々なゲームオブジェトを表す基底クラス
class GameObject
{
public:
	GameObject() = default;				//コンストラクタ
	virtual ~GameObject() = default;	//デストラクタ

	//イベントの操作
	virtual void Start();							//最初のUpdateの直前で呼び出される
	virtual void Update(float deltaTime);			//毎フレーム1回呼び出される
	virtual void OnCollision(GameObject& object);	//衝突が起きたときに呼び出される
	virtual void OnDestroy();						//ゲームオブジェクトがエンジンから削除されるときに呼び出される
	virtual void TakeDamage(GameObject& other, const Damage& damage);	//ダメージを受けたときに呼び出される

	bool IsStarted() const { return isStarted; }	//始まっているかどうか

	//スプライトの操作
	void AddSprite(const Texcoord& tc,
		float x = 0, float y = 0, float scale = 1, float rotation = 0);

	const SpriteList& GetSprite() const { return spriteList; }

	//コリジョンの操作
	void AddCollision(const Rect& r);
	const RectList& GetCollision()const { return collisionList; }
	const Rect& GetAabb()const { return aabb; }
	
	//コンポーネントの操作
	template<typename T>
	std::shared_ptr<T> AddComponent()	//これはどんな型にでも対応できる関数の書き方（中身が一緒の場合）
	{
		std::shared_ptr<T> p = std::make_shared<T>();
		componentList.push_back(p);

		//メッシュレンダラの場合は専用リストに追加
		if constexpr (std::is_base_of<MeshRenderer, T>::value)
		{
			meshRendererList.push_back(p);
		}
		return p;
	}

	//描画
	void DrawStaticMesh(const ProgramPipeline& program) const;

	Engine* engine = nullptr;		//エンジンのアドレス
	std::string name;				//オブジェクト名
	float x = 0;					//x座標
	float y = 0;					//y座標
	float z = 0;					//z座標
	float rotation[3] = { 0,0,0 };	//回転
	float radias = 0;				//角度
	float Scale = 1;				//大きさ
	float Red = 1;					//赤色
	float Green = 1;				//緑色
	float Blue = 1;					//青色
	float alpha = 1;				//透明度
	float HP = 1;					//体力
	int priority = 0;				//表示優先順位
	SpriteList spriteList;			//スプライト配列
	ComponentList componentList;	//コンポーネント配列
	bool isDead = false;			//死亡フラグ
	int Point = 0;					//スコア倍率

	int Power[5] = { 0,0,0,0,0 };	//パワーアップ状況
	float UnbeatableTime = 0.0f;			//無敵時間
	bool PinchFlg = false;			//true=HPが半分より小さい false=HPが半分以上

private:
	bool isStarted = false;			//スタート実行済みフラグ
	RectList collisionList;			//コリジョン配列
	Rect aabb = { 0,0,0,0 };		//コリジョン境界ボックス(最小の長方形)
	//   aabb = Axis Aligned Bounding Box

	std::vector<MeshRendererPtr> meshRendererList;
};




#endif //GAMEOBJECT_H_INCLUDED