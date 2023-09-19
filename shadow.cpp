//*****************************************************
//
// 影の処理[shadow.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "meshfield.h"
#include "game.h"
#include "objectmanager.h"

//=====================================================
// コンストラクタ
//=====================================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority)
{
	m_nor = { 0.0f,0.0f,0.0f };
}

//=====================================================
// デストラクタ
//=====================================================
CShadow::~CShadow()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CShadow::Init(void)
{
	// 継承クラスの初期化
	CObject3D::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CShadow::Uninit(void)
{
	// 継承クラスの終了
	CObject3D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CShadow::Update(void)
{
	// 変数宣言
	float fHeight = 0.0f;

	CMeshField *pMesh = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// メッシュフィールドの取得
		pMesh = pObjManager->GetMeshField();
	}

	// 継承クラスの更新
	CObject3D::Update();

	if (pMesh != nullptr)
	{
		fHeight = pMesh->GetHeight(D3DXVECTOR3(GetPosition().x, -100.0f, GetPosition().z), nullptr, &m_nor);
	}

	// 影の位置設定
	SetPosition(D3DXVECTOR3(GetPosition().x, fHeight + 3, GetPosition().z));
}

//=====================================================
// 描画処理
//=====================================================
void CShadow::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 継承クラスの描画
	CObject3D::DrawNormal(m_nor);
}

//=====================================================
// 生成処理
//=====================================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, float width, float height)
{
	CShadow *pShadow = nullptr;

	if (pShadow == nullptr)
	{
		pShadow = new CShadow;

		if (pShadow != nullptr)
		{
			pShadow->SetPosition(D3DXVECTOR3(pos));
			pShadow->SetSize(width, height);

			// 初期化
			pShadow->Init();

			// テクスチャの読込
			int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\EFFECT\\effect000.png");
			pShadow->SetIdxTexture(nIdx);

			pShadow->SetColor(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f));
		}
	}

	return pShadow;
}