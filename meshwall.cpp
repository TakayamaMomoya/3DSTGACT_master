//*****************************************************
//
// メッシュウォールの処理[meshwall.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "meshwall.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "debugproc.h"
#include "game.h"
#include <stdio.h>

//*****************************************************
// マクロ定義
//*****************************************************
#define MESHFIELD_TEX_FILE			"data\\TEXTURE\\MATERIAL\\wall003.png"				// テクスチャファイル名
#define MOVE_SPEED					(1.0f)										// 移動速度
#define MESH_LENGTH					(200.0f)									// メッシュの一辺の長さ
#define MESH_U						(254)											// 横のブロック数
#define MESH_V						(254)											// 縦のブロック数
#define SPLIT_TEX					(20)										// テクスチャ分割数

//=====================================================
// コンストラクタ
//=====================================================
CMeshWall::CMeshWall(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_MeshWall,sizeof(m_MeshWall));
	m_pIdxBuff = nullptr;
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	m_fLengthMesh = 0.0f;
	m_nDivNumU = 0;
	m_nDivNumV = 0;
	m_col = { 0.0f,0.0f,0.0f,0.0f };
}

//=====================================================
// デストラクタ
//=====================================================
CMeshWall::~CMeshWall()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMeshWall::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 値の初期設定
	m_fLengthMesh = MESH_LENGTH;
	m_nDivNumU = MESH_U;
	m_nDivNumV = MESH_V;
	m_nDivTex = SPLIT_TEX;
	m_col = { 1.0f,0.0f,0.0f,0.0f };

	m_MeshWall.nNumVtx = (m_nDivNumU + 1) * (m_nDivNumV + 1);

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		MESHFIELD_TEX_FILE,
		&m_pTexture);

	if (m_pVtxBuff == nullptr)
	{
		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_MeshWall.nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr);
	}

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCountV, nCountU;
	int nCntVtx = 0;
	float fAngleX, fAngleY;
	float fLength;
	D3DXVECTOR3 pos;

	for (nCountV = 0;nCountV < m_nDivNumV + 1; nCountV++)
	{// 頂点座標の設定
		for (nCountU = 0; nCountU < m_nDivNumU + 1; nCountU++)
		{
			// 頂点座標
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.x = (nCountU - m_nDivNumU * 0.5f) * m_fLengthMesh;
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.y = (((m_nDivNumU) * 0.5f) - nCountV) * m_fLengthMesh;
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.z = 0.0f;

			// X軸角度を計算
			fAngleX = atan2f(pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.x, pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.y);

			// Y軸角度を計算
			fAngleY = atan2f(pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.x, pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.z);

			if (pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.x < 0.0f)
			{
				fAngleY += D3DX_PI;
			}

			// 距離を計算
			fLength = D3DXVec3Length(&pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos);

			// 角度と距離をもとに位置を再設定
			pos =
			{
				m_MeshWall.pos.x + sinf(fAngleX + m_MeshWall.rot.x) * sinf(fAngleY + m_MeshWall.rot.y) * fLength,
				m_MeshWall.pos.y + cosf(fAngleX + m_MeshWall.rot.x) * fLength,
				m_MeshWall.pos.z + sinf(fAngleX + m_MeshWall.rot.x) * cosf(fAngleY + m_MeshWall.rot.y) * fLength
			}; 

			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos = pos;

			// テクスチャ座標
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].tex = D3DXVECTOR2
			(
			    ((float)m_nDivTex / m_nDivNumU) * nCountU,
				((float)m_nDivTex / m_nDivNumU) * nCountV
			);
		}
	}

	for (int nCnt = 0;nCnt < m_MeshWall.nNumVtx;nCnt++)
	{
		// 頂点カラーの設定
		pVtx[nCnt].col = m_col;

		pVtx[nCnt].nor = D3DXVECTOR3(0.0f,0.0f,1.0f);
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	m_MeshWall.nNumIdx = (m_nDivNumU + 1) * (m_nDivNumV + 1) + ((m_nDivNumV + 1) - 2) * (m_nDivNumU + 1) + (((m_nDivNumV + 1) - 2) * 2);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_MeshWall.nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	// インデックスバッファへのポインタ
	WORD *pIdx;

	// インデックスバッファをロックし、頂点番号へのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCount = 0; nCount < m_MeshWall.nNumIdx / 2; nCount++)
	{// インデックス決定
		if (
			nCount % ((m_nDivNumU + 1) + (m_nDivNumU + 2) * (nCount / (m_nDivNumU + 2))) == 0
			&& nCount != 0
			)
		{
			pIdx[nCount * 2 + 1] = ((m_nDivNumU + 1) * 2) + (m_nDivNumU + 1) * (nCount / (m_nDivNumU + 2));

			pIdx[nCount * 2] = m_nDivNumU + (m_nDivNumU + 1) * (nCount / (m_nDivNumU + 2));
		}
		else
		{
			pIdx[nCount * 2 + 1] = nCount - (nCount / (m_nDivNumU + 2));

			pIdx[nCount * 2] = (nCount - (nCount / (m_nDivNumU + 2))) + (m_nDivNumU + 1);
		}
	}

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CMeshWall::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{// 頂点バッファポインタの破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pIdxBuff != nullptr)
	{// インデックスバッファポインタの破棄
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CMeshWall::Update(void)
{
	
}

//=====================================================
// プレイヤーが近づいたら色を変える処理
//=====================================================
void CMeshWall::CheckPlayer(void)
{
	// プレイヤーの取得
	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer == nullptr)
	{
		return;
	}

	// 計算用変数
	D3DXVECTOR3 vecDiff;
	float fLength;

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < m_MeshWall.nNumVtx; nCntVtx++)
	{
		// 位置の差分を取得
		vecDiff = pPlayer->GetPosition() - pVtx[nCntVtx].pos;

		// 差分距離を取得
		fLength = D3DXVec3Length(&vecDiff);

		if (fLength < CHENGE_LENGTH)
		{
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f - (fLength / CHENGE_LENGTH));
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// 描画処理
//=====================================================
void CMeshWall::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	// カリングを無効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_MeshWall.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll (&mtxRot,
		0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_MeshWall.mtxWorld, &m_MeshWall.mtxWorld,&mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_MeshWall.mtxWorld, &m_MeshWall.mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD,&m_MeshWall.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャ設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴン描画
	pDevice->DrawIndexedPrimitive (D3DPT_TRIANGLESTRIP, 0, 0, m_MeshWall.nNumVtx, 0, m_MeshWall.nNumIdx - 2);

	// カリングを有効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=====================================================
// 生成処理
//=====================================================
CMeshWall *CMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CMeshWall *pMeshField = nullptr;

	if (pMeshField == nullptr)
	{
		pMeshField = new CMeshWall;

		if (pMeshField != nullptr)
		{
			pMeshField->m_MeshWall.pos = pos;
			pMeshField->m_MeshWall.rot = rot;
			pMeshField->Init();
		}
	}

	return pMeshField;
}