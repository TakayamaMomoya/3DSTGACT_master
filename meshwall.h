//*****************************************************
//
// メッシュウォールの処理[meshwall.h
// Author:髙山桃也
//
//*****************************************************

#ifndef _MESHWALL_H_
#define _MESHWALL_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define CHENGE_LENGTH	(1000)	// 色を変える距離

//*****************************************************
// クラス定義
//*****************************************************
class CMeshWall : public CObject
{
public:
	CMeshWall(int nPriority = 5);	// コンストラクタ
	~CMeshWall();	// デストラクタ

	typedef struct
	{
		D3DXVECTOR3 pos;						//位置
		D3DXVECTOR3 rot;						//向き
		D3DXMATRIX mtxWorld;					//ワールドマトリックス
		int nNumIdx;							//インデックス数
		int nNumVtx;							//頂点数
	}MeshWall;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMeshWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void CheckPlayer(void);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//インデックスバッファへのポインタ
	MeshWall m_MeshWall;	//構造体の情報
	D3DXCOLOR m_col;	// 色
	float m_fLengthMesh;	// メッシュ一辺の長さ
	int m_nDivNumU;
	int m_nDivNumV;
	int m_nDivTex;
};
#endif