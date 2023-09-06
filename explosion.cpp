//*****************************************************
//
// 爆発処理[explosion.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "explosion.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define ANIM_TIME				(6)	//アニメーションの速度
#define NUM_ANIM	(8)	// アニメーションの枚数

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CExplosion::m_nNumAll = 0;	// 総数

//=====================================================
// コンストラクタ
//=====================================================
CExplosion::CExplosion(int nPriority) : CBillboard(nPriority)
{
	m_nLife = 0;
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;

	m_nNumAll++;
}

//=====================================================
// デストラクタ
//=====================================================
CExplosion::~CExplosion()
{
	m_nNumAll--;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CExplosion::Init(void)
{
	// 継承クラスの初期化
	CBillboard::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CExplosion::Uninit(void)
{
	// 継承クラスの終了
	CBillboard::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CExplosion::Update(void)
{
	// 寿命減衰
	m_nLife--;

	// カウンターを加算
	m_nCounterAnim++;

	if (m_nCounterAnim % ANIM_TIME == 0)
	{// 一定時間ごとにアニメーションを行う
		m_nCounterAnim = 0;
		m_nPatternAnim++;

		// アニメーション数を超えたら0枚に戻す
		m_nPatternAnim = m_nPatternAnim % NUM_ANIM;
	}

	D3DXVECTOR2 leftUp = { 0.125f * m_nPatternAnim,0.0f };
	D3DXVECTOR2 rightDown = { 0.125f + 0.125f * m_nPatternAnim,1.0f };

	// テクスチャ座標設定
	SetTex(leftUp, rightDown);

	// 継承クラスの更新
	CBillboard::Update();

	if (m_nLife < 0)
	{// 自分の削除
		Uninit();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CExplosion::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//Zテストの無効化
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// 継承クラスの描画
	CBillboard::Draw();

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=====================================================
// 生成処理
//=====================================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos)
{
	CExplosion *pExplosion = nullptr;

	if (pExplosion == nullptr)
	{// インスタンス生成
		pExplosion = new CExplosion;
	}
	
	pExplosion->SetPosition(pos);
	pExplosion->SetSize(20.0f, 20.0f);

	// 初期化処理
	pExplosion->Init();

	// テクスチャの読込
	int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\EFFECT\\explosion000.png");
	pExplosion->SetIdxTexture(nIdx);

	pExplosion->m_nLife = 8 * 4;

	return pExplosion;
}

//=====================================================
// 読込処理
//=====================================================
HRESULT CExplosion::Load(void)
{
	return S_OK;
}

//=====================================================
// テクスチャ破棄
//=====================================================
void CExplosion::Unload(void)
{

}