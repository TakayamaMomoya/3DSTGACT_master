//*****************************************************
//
// �Q�[������[game.h]
// Author:���R����
//
//*****************************************************
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPlayer;
class CScore;
class CTimer;
class CGame;
class CDefend;
class CEdit;
class CMeshWall;
class CPause;
class CRank;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CGame : public CScene
{
public:
	enum STATE
	{// ���
		STATE_NONE,	// �������Ă��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_END,	// �I�����
		STATE_MAX
	};

	CGame();	// �R���X�g���N�^
	~CGame();	// �f�X�g���N�^

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();
	static CScore *GetScore(void) { return m_pScore; }
	static CTimer *GetTimer(void) { return m_pTimer; }
	static CDefend *GetDefend(void) { return m_pDefend; }
	static CEdit *GetEdit(void) { return m_pEdit; }
	static CRank *GetRank(void) { return m_pRank; }
	static void ReleaseScore(void);
	static void ReleaseTimer(void);
	static void ReleaseDefend(void);
	bool IsPhoto(void) { return m_bPhoto; }
	static void EnablePause(bool bPause) { m_bPause = bPause; }
	static bool IsPause(void) { return m_bPause; }
	static void TogglePause(CPause *pPause);

private:
	enum WALLTYPE
	{
		WALLTYPE_FRONT = 0,	// �O���̕�
		WALLTYPE_RIGHT,	// �E�̕�
		WALLTYPE_LEFT,	// ���̕�
		WALLTYPE_REAR,	// ���̕�
		WALLTYPE_MAX
	};

	void Debug(void);	// �f�o�b�O����
	void ManageState(void);	// ��ԊǗ�
	void LimitPlayerPos(void);	// �v���C���[�ړ�����
	void ManageWall(void);	// �ǂ̊Ǘ�
	void TimeBonus(void);	// �^�C���{�[�i�X�̊Ǘ�

	static CScore *m_pScore;	// �X�R�A�̃|�C���^
	static CTimer *m_pTimer;	// �^�C�}�[�̃|�C���^
	static CDefend *m_pDefend;	// ��q�Ώۂւ̃|�C���^
	static CEdit *m_pEdit;	// �G�f�B�^�[�ւ̃|�C���^
	static CRank *m_pRank;	// �����N�ւ̃|�C���^
	static STATE m_state;	// ���
	CMeshWall *m_apMeshWall[WALLTYPE_MAX];	// �ǂ̃|�C���^
	bool m_bPhoto;	// �t�H�g���[�h���ǂ���
	static bool m_bPause;	// �|�[�Y���ǂ���
	static CPause *m_pPause;	// �|�[�Y�̃A�h���X
	int m_nTimerWave;	// �E�F�[�u�^�C�}�[
	int m_nCntSecond;	// ��b�̃J�E���^�[
};

#endif