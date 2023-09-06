//*****************************************************
//
// コックピットの処理[cockpit.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "cockpit.h"
#include "object2D.h"
#include "manager.h"
#include "texture.h"
#include "number.h"
#include "player.h"
#include "game.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_PLACE	(3)	// 桁数
#define GAUGE_HEIGHT	(40.0f)	// ゲージの縦幅
#define GAUGE_LENGTH	(200.0f)	// ゲージの長さ
#define BETWEEN_HEIGHT	(80.0f)	// ゲージ間の幅
#define ROLL_SPEED	(0.05f)	// 回転速度

//=====================================================
// コンストラクタ
//=====================================================
CCockPit::CCockPit(int nPriority)
{
	m_pFrame = nullptr;
	m_pLife = nullptr;

	for (int nCnt = 0; nCnt < PARAM_MAX; nCnt++)
	{
		m_apGauge[nCnt] = nullptr;
		m_apFrame[nCnt] = nullptr;
		m_apIcon[nCnt] = nullptr;
	}
}

//=====================================================
// デストラクタ
//=====================================================
CCockPit::~CCockPit()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CCockPit::Init(void)
{
	char *apPath[PARAM_MAX] =
	{
		"data\\TEXTURE\\UI\\icon00.png",
		"data\\TEXTURE\\UI\\icon01.png",
		"data\\TEXTURE\\UI\\icon02.png",
	};

	if (m_pFrame == nullptr)
	{// フレームの生成
		m_pFrame = CObject2D::Create(7);
	}

	if (m_pFrame != nullptr)
	{
		m_pFrame->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		m_pFrame->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

		// テクスチャ番号取得
		int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\UI\\frame01.png");

		m_pFrame->SetIdxTexture(nIdx);
		m_pFrame->SetVtx();
		m_pFrame->SetCol(D3DXCOLOR(1.0f,1.0f,1.0f,0.1f));
	}

	if (m_pLife == nullptr)
	{
		m_pLife = CNumber::Create(NUM_PLACE, 0);
		m_pLife->SetPosition(D3DXVECTOR3(900.0f, 600.0f, 0.0f));
		m_pLife->SetSizeAll(20.0f, 50.0f);
	}

	for (int nCnt = 0; nCnt < PARAM_MAX; nCnt++)
	{
		if (m_apGauge[nCnt] == nullptr)
		{// ブーストゲージの生成
			m_apGauge[nCnt] = CObject2D::Create(7);

			if (m_apGauge[nCnt] != nullptr)
			{
				m_apGauge[nCnt]->SetPosition(D3DXVECTOR3(100.0f, 50.0f + BETWEEN_HEIGHT * nCnt, 0.0f));
				m_apGauge[nCnt]->SetSize(10.0f, GAUGE_HEIGHT);
				m_apGauge[nCnt]->SetVtx();
			}
		}

		if (m_apFrame[nCnt] == nullptr)
		{// ブーストゲージの生成
			m_apFrame[nCnt] = CObject2D::Create(7);

			if (m_apFrame[nCnt] != nullptr)
			{
				// テクスチャ番号取得
				int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\UI\\frame02.png");

				m_apFrame[nCnt]->SetIdxTexture(nIdx);
				m_apFrame[nCnt]->SetVtx();
				m_apFrame[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				m_apFrame[nCnt]->SetPosition(D3DXVECTOR3(40.0f, 50.0f + BETWEEN_HEIGHT * nCnt, 0.0f));
				m_apFrame[nCnt]->SetSize(40.0f, 40.0f);
				m_apFrame[nCnt]->SetVtx();
			}
		}

		if (m_apIcon[nCnt] == nullptr)
		{
			m_apIcon[nCnt] = CObject2D::Create(7);

			if (m_apIcon[nCnt] != nullptr)
			{
				// テクスチャ番号取得
				int nIdx = CManager::GetTexture()->Regist(apPath[nCnt]);

				m_apIcon[nCnt]->SetIdxTexture(nIdx);
				m_apIcon[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				m_apIcon[nCnt]->SetPosition(D3DXVECTOR3(40.0f, 50.0f + BETWEEN_HEIGHT * nCnt, 0.0f));
				m_apIcon[nCnt]->SetSize(40.0f, 40.0f);
				m_apIcon[nCnt]->SetVtx();
			}
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CCockPit::Uninit(void)
{
	if (m_pFrame != nullptr)
	{
		m_pFrame->Uninit();

		m_pFrame = nullptr;
	}

	if (m_pLife != nullptr)
	{
		m_pLife->Uninit();

		m_pLife = nullptr;
	}

	for (int nCnt = 0; nCnt < PARAM_MAX; nCnt++)
	{
		if (m_apGauge[nCnt] != nullptr)
		{
			m_apGauge[nCnt]->Uninit();

			m_apGauge[nCnt] = nullptr;
		}

		if (m_apFrame[nCnt] != nullptr)
		{
			m_apFrame[nCnt]->Uninit();

			m_apFrame[nCnt] = nullptr;
		}

		if (m_apIcon[nCnt] != nullptr)
		{
			m_apIcon[nCnt]->Uninit();

			m_apIcon[nCnt] = nullptr;
		}
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CCockPit::Update(void)
{
	// ライフ管理
	ManageLife();

	// パラメーター管理
	ManageParam();
}

//=====================================================
// ライフ管理
//=====================================================
void CCockPit::ManageLife(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();
	int nLife;
	int nRate;

	if (pPlayer == nullptr)
	{
		return;
	}

	// ライフ取得
	nLife = pPlayer->GetLife();

	// ライフが減っている割合を算出
	nRate = (INITIAL_LIFE_PLAYER - nLife) * 100 / INITIAL_LIFE_PLAYER;

	if (m_pLife != nullptr)
	{
		m_pLife->SetValue(nRate,NUM_PLACE);
	}
}

//=====================================================
// パラメーター管理
//=====================================================
void CCockPit::ManageParam(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();
	float fWidth;
	D3DXVECTOR3 pos;

	if (pPlayer == nullptr)
	{
		return;
	}

	float aParam[PARAM_MAX] =
	{
		pPlayer->GetRapid(),
		pPlayer->GetPower(),
		pPlayer->GetCost()
	};

	for (int nCnt = 0; nCnt < PARAM_MAX; nCnt++)
	{
		if (m_apGauge[nCnt] != nullptr)
		{
			fWidth = aParam[nCnt] * GAUGE_LENGTH;

			// 位置設定
			pos = { 40.0f + fWidth,50.0f + BETWEEN_HEIGHT * nCnt,0.0f };

			// サイズ設定
			m_apGauge[nCnt]->SetPosition(pos);
			m_apGauge[nCnt]->SetSize(fWidth, GAUGE_HEIGHT * 0.5f);
			m_apGauge[nCnt]->SetVtx();
		}

		if (m_apFrame[nCnt] != nullptr)
		{
			m_apFrame[nCnt]->SetRot(m_apFrame[nCnt]->GetRot() + ROLL_SPEED * aParam[nCnt]);
			m_apFrame[nCnt]->SetVtx();
		}
	}
}

//=====================================================
// 生成処理
//=====================================================
CCockPit *CCockPit::Create(void)
{
	CCockPit *pCockPit = nullptr;

	if (pCockPit == nullptr)
	{// インスタンス生成
		pCockPit = new CCockPit;

		// 初期化処理
		pCockPit->Init();
	}

	return pCockPit;
}