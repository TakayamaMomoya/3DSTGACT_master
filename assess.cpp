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
#include "texture.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define GAUGE_POS	(D3DXVECTOR3(50.0f,550.0f,0.0f))	// ゲージの初期位置
#define GAUGE_WIDTH	(SCREEN_WIDTH / 3)	// ゲージの横幅
#define GAUGE_HEIGHT	(9.0f)	// ゲージの縦の長さ
#define BETWEEN_HEIGHT	(50.0f)	// ゲージ間の幅
#define FRAME_HEIGHT	(10.0f)	// フレームの縦の長さ
#define FRAME_WIDTH	(SCREEN_WIDTH / 3)	// フレームの横幅
#define FRAME_PATH	"data\\TEXTURE\\UI\\frame04.png"	// フレームのパス
#define GAUGE_PATH	"data\\TEXTURE\\UI\\gauge00.png"	// ゲージのパス
#define TURN_RATE	(4.0f)	// ゲージの動きが反転するまでの範囲
#define GAUGE_SPEED	(0.05f)	// ゲージの動く速度
#define EXTEND_SPEED	(0.2f)	// ゲージの伸びる速度
#define SHRINK_SPEED	(0.03f)	// ゲージの縮む速度

//=====================================================
// コンストラクタ
//=====================================================
CAssess::CAssess(int nPriority)
{
	for (int nCnt = 0; nCnt < PARAM_MAX; nCnt++)
	{
		m_apParam[nCnt] = nullptr;
	}
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
		if (m_apParam[nCnt] != nullptr)
		{
			continue;
		}
		else
		{
			m_apParam[nCnt] = new Param;

			ZeroMemory(m_apParam[nCnt],sizeof(Param));
		}

		if (m_apParam[nCnt]->pGauge == nullptr)
		{// ブーストゲージの生成
			m_apParam[nCnt]->pGauge = CObject2D::Create(6);

			if (m_apParam[nCnt]->pGauge != nullptr)
			{
				// テクスチャ番号取得
				int nIdx = CManager::GetTexture()->Regist(GAUGE_PATH);

				m_apParam[nCnt]->pGauge->SetIdxTexture(nIdx);
				m_apParam[nCnt]->pGauge->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apParam[nCnt]->pGauge->SetPosition(D3DXVECTOR3(FRAME_WIDTH * 0.5f + FRAME_WIDTH * nCnt, FRAME_HEIGHT, 0.0f));
				m_apParam[nCnt]->pGauge->SetSize(0.0f, GAUGE_HEIGHT);
				m_apParam[nCnt]->pGauge->SetVtx();
				m_apParam[nCnt]->state = GAUGESTATE_EXTEND;
			}
		}

		if (m_apParam[nCnt]->pFrame == nullptr)
		{// パラメーターフレームの生成
			m_apParam[nCnt]->pFrame = CObject2D::Create(7);

			if (m_apParam[nCnt]->pFrame != nullptr)
			{
				// テクスチャ番号取得
				int nIdx = CManager::GetTexture()->Regist(FRAME_PATH);

				m_apParam[nCnt]->pFrame->SetIdxTexture(nIdx);
				m_apParam[nCnt]->pFrame->SetVtx();
				m_apParam[nCnt]->pFrame->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				m_apParam[nCnt]->pFrame->SetPosition(D3DXVECTOR3(FRAME_WIDTH * 0.5f + FRAME_WIDTH * nCnt, FRAME_HEIGHT, 0.0f));
				m_apParam[nCnt]->pFrame->SetSize(FRAME_WIDTH * 0.5f, FRAME_HEIGHT);
				m_apParam[nCnt]->pFrame->SetVtx();
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
		if (m_apParam[nCnt] != nullptr)
		{
			if (m_apParam[nCnt]->pGauge != nullptr)
			{// ゲージの破棄
				m_apParam[nCnt]->pGauge->Uninit();

				m_apParam[nCnt]->pGauge = nullptr;
			}

			if (m_apParam[nCnt]->pFrame != nullptr)
			{// フレームの破棄
				m_apParam[nCnt]->pFrame->Uninit();

				m_apParam[nCnt]->pFrame = nullptr;
			}

			delete m_apParam[nCnt];

			m_apParam[nCnt] = nullptr;
		}
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CAssess::Update(void)
{
	// ゲージの管理
	ManageGauge();
}

//=====================================================
// ゲージ管理
//=====================================================
void CAssess::ManageGauge(void)
{
	for (int nCnt = 0; nCnt < PARAM_MAX; nCnt++)
	{
		if (m_apParam[nCnt] != nullptr)
		{
			// ゲージの脈動
			//PalsingGauge(m_apParam[nCnt]);
		}
	}
}

//=====================================================
// ゲージの脈動
//=====================================================
void CAssess::PalsingGauge(Param *pParam)
{
	if (pParam->pGauge == nullptr)
	{
		return;
	}

	// 変数宣言
	CObject2D *pObj;
	float fWidth;

	pObj = pParam->pGauge;
	
	// 幅取得
	fWidth = pObj->GetWidth();

	float fWidthDiff = pParam->fWidthDest - fWidth;

	if (TURN_RATE * TURN_RATE > fWidthDiff * fWidthDiff)
	{// 差分が設定よりも小さくなったら反転
		SetWidthDest(pParam);
	}
	else
	{// サイズの設定
		if (pParam->state == GAUGESTATE_EXTEND)
		{
			fWidthDiff *= EXTEND_SPEED;
		}
		else
		{
			fWidthDiff *= SHRINK_SPEED;
		}

		if (pParam->pGauge != nullptr)
		{// ゲージ設定
			pParam->pGauge->SetSize(fWidth + fWidthDiff, GAUGE_HEIGHT);
			pParam->pGauge->SetVtx();
		}
	}
}

//=====================================================
// 目的の幅を決める処理
//=====================================================
void CAssess::SetWidthDest(Param *pParam)
{
	// 目的値を設定
	pParam->fWidthDest = pParam->fParam * GAUGE_WIDTH * 0.5f;

	float fWidthDest = pParam->fWidthDest;

	switch (pParam->state)
	{
	case GAUGESTATE_EXTEND:

		pParam->state = GAUGESTATE_SHRINK;

		break;
	case GAUGESTATE_SHRINK:

		// 短めの位置に目的値を設定
		fWidthDest *= 0.6f;

		pParam->state = GAUGESTATE_EXTEND;

		break;
	default:
		break;
	}

	pParam->fWidthDest = fWidthDest;
}

//=====================================================
// パラメーター加算処理
//=====================================================
void CAssess::AddParam(float fValue, PARAM param)
{
	if (m_apParam[param] != nullptr)
	{
		m_apParam[param]->fParam += fValue;

		if (m_apParam[param]->fParam >= 1.0f)
		{
			m_apParam[param]->fParam = 1.0f;
		}
		else if (m_apParam[param]->fParam < 0.0f)
		{
			m_apParam[param]->fParam = 0.0f;
		}
	}
}

//=====================================================
// パラメーター取得処理
//=====================================================
float CAssess::GetParam(PARAM param)
{
	float fParam = 0.0f;

	if (m_apParam[param] != nullptr)
	{
		fParam = m_apParam[param]->fParam;
	}

	return fParam;
}

//=====================================================
// 描画処理
//=====================================================
void CAssess::Draw(void)
{

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