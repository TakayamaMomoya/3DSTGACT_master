//*****************************************************
//
// 矢印の処理[arrow.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "arrow.h"
#include "player.h"
#include "game.h"
#include "enemy.h"
#include "objectmanager.h"
#include "manager.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define LENGTH (300.0f)	// プレイヤーからの距離
#define HEIGHT	(100.0f)	// 高さ

//=====================================================
// コンストラクタ
//=====================================================
CArrow::CArrow(int nPriority)
{
	m_posDest = { 0.0f,0.0f,0.0f };
}

//=====================================================
// デストラクタ
//=====================================================
CArrow::~CArrow()
{

}

//=====================================================
// 生成処理
//=====================================================
CArrow *CArrow::Create(void)
{
	CArrow *pDebris = nullptr;

	if (pDebris == nullptr)
	{// インスタンス生成
		pDebris = new CArrow;

		// 初期化処理
		pDebris->Init();
	}

	return pDebris;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CArrow::Init(void)
{
	// 継承クラスの初期化
	CObjectX::Init();

	// モデル番号の設定
	int nIdx = CModel::Load("data\\MODEL\\arrow.x");

	SetIdxModel(nIdx);

	BindModel(nIdx);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CArrow::Uninit(void)
{
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CArrow::Update(void)
{
	CObjectX::Update();

	RollPlayer();
}

//=====================================================
// プレイヤーの周りにいる処理
//=====================================================
void CArrow::RollPlayer(void)
{
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// プレイヤーの適用
		pPlayer = pObjManager->GetPlayer();
	}

	D3DXVECTOR3 posTarget;
	D3DXVECTOR3 vecDiff;
	D3DXVECTOR3 rot = GetRot();

	if (pPlayer == nullptr)
	{
		return;
	}

	// 目的座標との差分を取得
	posTarget = pPlayer->GetPosition();

	vecDiff = posTarget - m_posDest;

	D3DXVec3Normalize(&vecDiff,&vecDiff);

	// 一定の距離に補正
	vecDiff *= LENGTH;

	posTarget = D3DXVECTOR3(posTarget.x - vecDiff.x,posTarget.y + HEIGHT, posTarget.z - vecDiff.z);

	SetPosition(posTarget);

	// 差分ベクトルの方向を向く
	rot.y = atan2f(vecDiff.x, vecDiff.z);

	SetRot(rot);
}

//=====================================================
// 描画処理
//=====================================================
void CArrow::Draw(void)
{
	CObjectX::Draw();
}