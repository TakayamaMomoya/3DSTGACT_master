//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 高山桃也
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSound
{
public:
	typedef enum
	{
		LABEL_BGM000 = 0,				// タイトルBGM
		LABEL_SHOT,	// 発射音
		LABEL_EXPLOSION,	// 爆発音
		LABEL_MAX,
	} LABEL;

	typedef enum
	{
		FADESTATE_NONE = 0,	//何もしていない状態
		FADESTATE_IN,	//フェードイン状態
		FADESTATE_OUT,	//フェードアウト状態
		FADESTATE_MAX
	}FADESTATE;

	CSound();	// コンストラクタ
	~CSound();	// デストラクタ

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);

	HRESULT Play(LABEL label);
	void Stop(LABEL label);
	void Stop(void);

	void SetVolume(LABEL label, float fVolume);
	void SetFade(LABEL LabelNow, LABEL LabelNext, float fSpeed);

private:
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDINFO;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX];					// オーディオデータサイズ
	FADESTATE m_fadeSound;								// フェード状態
	LABEL m_SoundLabelNow;								// 現在のサウンドラベル
	LABEL m_SoundLabelNext;								// 次のサウンドラベル
	float m_fSpeedFadeSound;									// サウンドがフェードするスピード
	float m_fVolumeSound;										// サウンドのボリューム
	// サウンドの情報
	SOUNDINFO m_aSoundInfo[LABEL_MAX] =
	{
		{ "data/SOUND/BGM/title.wav", -1 },			// タイトル	
		{ "data/SOUND/SE/laser000.wav", 0 },			// 発射音レーザー	
		{ "data/SOUND/SE/explSmall.wav", 0 },			// 爆発音
	};
};

#endif