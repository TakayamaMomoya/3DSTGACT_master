//*****************************************************
//
// 評価の処理[assess.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "assess.h"
#include "object2D.h"
#include "manager.h"
#include "debugproc.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define GAUGE_POS	(D3DXVECTOR3(50.0f,550.0f,0.0f))	// ゲージの初期位置
#define GAUGE_WIDTH	(20.0f)	// ゲージの横幅
#define GAUGE_HEIGHT	(125.0f)	// ゲージの縦の長さ
#define BETWEEN_HEIGHT	(50.0f)	// ゲージ間の幅

//=====================================================
// コンストラクタ
//=====================================================
CAssess::CAssess(int nPriority)
{
	m_fAssessHit = 0.0f;
	m_fAssessDodge = 0.0f;
	m_fAssessAttack = 0.0f;
}

//=====================================================
// デストラクタ
//=====================================================
CAssess::~CAssess()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CAssess::Init(void)
{
	for (int nCnt = 0; nCnt < PARAM_MAX; nCnt++)
	{
		if (m_apGauge[nCnt] == nullptr)
		{// ブーストゲージの生成
			m_apGauge[nCnt] = CObject2D::Create(7);

			if (m_apGauge[nCnt] != nullptr)
			{
				m_apGauge[nCnt]->SetPosition(D3DXVECTOR3(GAUGE_POS.x + BETWEEN_HEIGHT * nCnt, GAUGE_POS.y, 0.0f));
				m_apGauge[nCnt]->SetSize(GAUGE_WIDTH, GAUGE_HEIGHT);
				m_apGauge[nCnt]->SetVtx();
			}
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CAssess::Uninit(void)
{
	for (int nCnt = 0; nCnt < PARAM_MAX; nCnt++)
	{
		if (m_apGauge[nCnt] != nullptr)
		{
			m_apGauge[nCnt]->Uninit();

			m_apGauge[nCnt] = nullptr;
		}
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CAssess::Update(void)
{
	// 評価の制限
	LimitAssess();

	// ゲージの管理
	ManageGauge();
}

//=====================================================
// ゲージ管理
//=====================================================
void CAssess::ManageGauge(void)
{
	float aParam[PARAM_MAX] =
	{
		m_fAssessHit,
		m_fAssessDodge,
		m_fAssessAttack
	};

	for (int nCnt = 0; nCnt < PARAM_MAX; nCnt++)
	{
		if (m_apGauge[nCnt] != nullptr)
		{
			float fHeight = aParam[nCnt] * GAUGE_HEIGHT;

			// 位置設定
			D3DXVECTOR3 pos = { GAUGE_POS.x + BETWEEN_HEIGHT * nCnt,GAUGE_POS.y - fHeight,0.0f };
 
			// サイズ設定
			m_apGauge[nCnt]->SetPosition(pos);
			m_apGauge[nCnt]->SetSize(GAUGE_WIDTH, fHeight);
			m_apGauge[nCnt]->SetVtx();
		}
	}
}

//=====================================================
// 評価の制限
//=====================================================
void CAssess::LimitAssess(void)
{
	if (m_fAssessAttack >= 1.0f)
	{
		m_fAssessAttack = 1.0f;
	}
	else if (m_fAssessAttack < 0.0f)
	{
		m_fAssessAttack = 0.0f;
	}

	if (m_fAssessHit >= 1.0f)
	{
		m_fAssessHit = 1.0f;
	}
	else if (m_fAssessHit < 0.0f)
	{
		m_fAssessHit = 0.0f;
	}

	if (m_fAssessDodge >= 1.0f)
	{
		m_fAssessDodge = 1.0f;
	}
	else if (m_fAssessDodge < 0.0f)
	{
		m_fAssessDodge = 0.0f;
	}
}

//=====================================================
// 描画処理
//=====================================================
void CAssess::Draw(void)
{
	CManager::GetDebugProc()->Print("\n命中率：[%f]", m_fAssessHit);
}

//=====================================================
// 生成処理
//=====================================================
CAssess *CAssess::Create(void)
{
	CAssess *pAssess = nullptr;

	if (pAssess == nullptr)
	{// インスタンス生成
		pAssess = new CAssess;

		// 初期化処理
		pAssess->Init();
	}

	return pAssess;
}