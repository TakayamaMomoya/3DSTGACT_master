//*****************************************************
//
// ロックオンの処理[lockon.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "lockon.h"
#include "billboard.h"
#include "camera.h"
#include "manager.h"
#include "enemy.h"
#include "texture.h"
#include "camera.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define PATH_LOCKON	("data\\TEXTURE\\UI\\lockon002.png")	// ロックオンマーカーのパス
#define MOVE_FACT	(0.3f)	// 動く速度
#define DEFAULT_SIZE	(200.0f)		// 基準の大きさ
#define LOCK_SIZE	(80.0f)		// ロックオン時の大きさ
#define DEFAULT_DIST	(2000.0f)		// 基準の距離
#define SIZE_FACT	(0.1f)		// 大きさの変わる速さ

//=====================================================
// コンストラクタ
//=====================================================
CLockon::CLockon(int nPriority)
{
	m_pLockOn = nullptr;
	m_pEnemy = nullptr;
	m_posDest = { 0.0f,0.0f,0.0f };
	m_fSize = 0.0f;
}

//=====================================================
// デストラクタ
//=====================================================
CLockon::~CLockon()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CLockon::Init(void)
{
	// ロックオンマーカーの生成
	m_pLockOn = CBillboard::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), DEFAULT_SIZE, DEFAULT_SIZE);

	if (m_pLockOn != nullptr)
	{// ロックオンマーカーの初期設定
		int nIdx = CManager::GetTexture()->Regist(PATH_LOCKON);
		m_pLockOn->SetIdxTexture(nIdx);
		m_pLockOn->SetZTest(true);
	}

	m_fSize = DEFAULT_SIZE;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CLockon::Uninit(void)
{
	if (m_pLockOn != nullptr)
	{
		m_pLockOn->Uninit();

		m_pLockOn = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CLockon::Update(void)
{
	D3DXVECTOR3 vecDiff;
	D3DXVECTOR3 pos;

	// カメラ取得
	CCamera *pCamera = CManager::GetCamera();
	CCamera::Camera *pCameraInfo = nullptr;

	if (pCamera != nullptr)
	{
		pCameraInfo = pCamera->GetCamera();

		if (pCameraInfo != nullptr)
		{
			// 差分のベクトルを取得
			vecDiff = m_pLockOn->GetPosition() - pCameraInfo->posV;

			// 差分の長さを取得
			float fLength = D3DXVec3Length(&vecDiff);

			// 基準の距離との割合を算出
			float fSize = fLength / DEFAULT_DIST;
			fSize *= m_fSize;

			// サイズ設定
			m_pLockOn->SetSize(fSize, fSize);
		}
	}

	if (m_pEnemy != nullptr && m_pLockOn != nullptr)
	{
		m_posDest = m_pEnemy->GetPosition();

		m_fSize += (LOCK_SIZE - m_fSize) * SIZE_FACT;
	}
	else
	{
		m_fSize += (DEFAULT_SIZE - m_fSize) * SIZE_FACT;
	}

	vecDiff = m_posDest - m_pLockOn->GetPosition();

	pos = m_pLockOn->GetPosition() + vecDiff * MOVE_FACT;

	m_pLockOn->SetPosition(pos);
}

//=====================================================
// 生成処理
//=====================================================
CLockon *CLockon::Create(void)
{
	CLockon *pLockon = nullptr;

	if (pLockon == nullptr)
	{// インスタンス生成
		pLockon = new CLockon;

		// 初期化処理
		pLockon->Init();
	}

	return pLockon;
}