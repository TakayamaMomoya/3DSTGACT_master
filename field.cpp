//*****************************************************
//
// 地面の処理[field.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "field.h"
#include "manager.h"
#include "texture.h"

//=====================================================
// コンストラクタ
//=====================================================
CField::CField()
{

}

//=====================================================
// デストラクタ
//=====================================================
CField::~CField()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CField::Init(void)
{
	// 継承クラスの初期化
	CObject3D::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CField::Uninit(void)
{
	// 継承クラスの終了
	CObject3D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CField::Update(void)
{
	// 継承クラスの更新
	CObject3D::Update();
}

//=====================================================
// 描画処理
//=====================================================
void CField::Draw(void)
{
	// 継承クラスの描画
	CObject3D::Draw();
}

//=====================================================
// 生成処理
//=====================================================
CField *CField::Create(D3DXVECTOR2 tex,D3DXVECTOR3 pos,float width,float height)
{
	CField *pField = nullptr;

	if (pField == nullptr)
	{
		pField = new CField;

		if (pField != nullptr)
		{
			pField->SetPosition(pos);
			pField->SetSize(width, height);

			// 初期化処理
			pField->Init();

			// テクスチャの読込
			int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\BG\\metal000.jpg");
			pField->SetIdxTexture(nIdx);
		}
	}

	return pField;
}