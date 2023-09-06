//*****************************************************
//
// 塵の処理[dust.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "dust.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define SPEED_MOVE	(7.0f)	// 移動速度
#define ALPHA_SPEED	(0.01f)	// 消えるスピード
#define WIND_ANGLE	(D3DX_PI * 0.3f)	// 風向き
#define WIND_RANGE	(D3DX_PI * 0.1f)	// 風向きのランダム幅

//=====================================================
// コンストラクタ
//=====================================================
CDust::CDust(int nPriority) : CBillboard(nPriority)
{
	m_fAlpha = 0.0f;
	m_state = STATE_NONE;
}

//=====================================================
// デストラクタ
//=====================================================
CDust::~CDust()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CDust::Init(void)
{
	// 継承クラスの初期化
	CBillboard::Init();

	D3DXCOLOR col = { 0.97f, 0.90f, 0.71f, 0.00f };

	SetColor(col);

	// 値の初期化
	m_state = STATE_IN;
	m_move = { 0.0f,0.0f,0.0f };

	// 移動量の設定
	SetMove();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CDust::Uninit(void)
{
	// 継承クラスの終了
	CBillboard::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CDust::Update(void)
{
	// 継承クラスの更新
	CBillboard::Update();

	// 移動量を位置に反映
	D3DXVECTOR3 pos = GetPosition();

	pos += m_move;

	SetPosition(pos);

	// 色の取得
	D3DXCOLOR col = GetColor();

	switch (m_state)
	{
	case CDust::STATE_IN:

		col.a += ALPHA_SPEED;

		if (col.a >= 1.0f)
		{
			col.a = 1.0f;
			
			m_state = STATE_OUT;
		}

		break;
	case CDust::STATE_OUT:

		col.a -= ALPHA_SPEED;

		if (col.a <= 0)
		{
			Uninit();
		}

		break;
	default:
		break;
	}

	SetColor(col);
}

//=====================================================
// 移動量の設定
//=====================================================
void CDust::SetMove(void)
{
	float fSpeed;
	float fMoveY;
	float fAngle;

	// 移動スピードの決定
	fSpeed = (float)(rand() % 5 + 1);
	fMoveY = (float)(rand() % 5 + 1) * 0.1f;

	// 移動方向の決定
	fAngle = WIND_ANGLE + (float)(rand() % (int)(WIND_RANGE * 100.0f) - WIND_RANGE * 50.0f) / 100.0f;

	m_move = 
	{
		sinf(fAngle) * fSpeed,
		fMoveY,
		cosf(fAngle) * fSpeed
	};
}

//=====================================================
// 描画処理
//=====================================================
void CDust::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 20);

	// 継承クラスの描画
	CBillboard::Draw();

	// アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//=====================================================
// 生成処理
//=====================================================
CDust *CDust::Create(D3DXVECTOR3 pos)
{
	CDust *pDust = nullptr;

	if (pDust == nullptr)
	{// インスタンス生成
		pDust = new CDust;

		if (pDust != nullptr)
		{
			pDust->SetPosition(pos);

			pDust->SetSize(500,500);

			// 初期化処理
			pDust->Init();

			// テクスチャの読込
			int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\EFFECT\\smoke.png");
			pDust->SetIdxTexture(nIdx);
		}
	}
	
	return pDust;
}