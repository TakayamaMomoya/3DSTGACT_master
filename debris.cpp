//*****************************************************
//
// 破片の処理[debris.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "debris.h"
#include "game.h"
#include "meshfield.h"
#include "particle.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MODE_FACT	(0.9f)	// 移動量の減衰係数
#define GRAVITY	(0.7f)	// 重力

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CDebris::m_nNumAll = 0;	// 総数

//=====================================================
// コンストラクタ
//=====================================================
CDebris::CDebris(int nPriority)
{
	m_nLife = 0;

	m_nNumAll++;
}

//=====================================================
// デストラクタ
//=====================================================
CDebris::~CDebris()
{
	m_nNumAll--;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CDebris::Init(void)
{
	// 継承クラスの初期化
	CObjectX::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CDebris::Uninit(void)
{
	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CDebris::Update(void)
{
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 pos = GetPosition();

	// 継承クラスの更新
	CObjectX::Update();

	CParticle::Create(pos,CParticle::TYPE_SMOKE,D3DXVECTOR3(0.0f,0.0f,0.0f),nullptr,4);

	// 移動量を位置に加算
	SetPosition(pos + move);

	// 移動量減衰
	//move.x *= MODE_FACT;
	//move.z *= MODE_FACT;
	move.y -= GRAVITY;

	SetMove(move);

	// メッシュフィールドとの当たり判定
	CMeshField *pMesh = CGame::GetMeshField();

	if (pMesh != nullptr)
	{
		move = GetMove();
		pos = GetPosition();

		// 高さの取得
		float fHeight = CGame::GetMeshField()->GetHeight(pos, &move);

		if (fHeight >= pos.y)
		{// 取得した高さに設定
			pos.y = fHeight;

			SetPosition(pos);
		}
	}

	m_nLife--;

	if (m_nLife <= 0)
	{// 寿命による破棄
		Uninit();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CDebris::Draw(void)
{
	// 継承クラスの描画
	CObjectX::Draw();
}

//=====================================================
// 生成処理
//=====================================================
CDebris *CDebris::Create(D3DXVECTOR3 pos, int nLife)
{
	CDebris *pDebris = nullptr;

	if (pDebris == nullptr)
	{// インスタンス生成
		pDebris = new CDebris;

		pDebris->m_nLife = nLife;

		pDebris->SetPosition(pos);

		// 初期化処理
		pDebris->Init();
	}

	return pDebris;
}