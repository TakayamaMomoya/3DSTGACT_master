//*****************************************************
//
// �`���[�g���A���Ǘ��̏���[tutorialmanager.h]
// Author:���R����
//
//*****************************************************

#ifndef _TUTORIALMANAGER_H_
#define _TUTORIALMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObject2D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CTutorialManager : public CObject
{
public:
	enum PROGRESS
	{// �i�s��
		PROGRESS_START,	// �J�n
		PROGRESS_MOVE,	// �ړ�
		PROGRESS_BOOST,	// �u�[�X�g
		PROGRESS_SHOT,	// �ˌ�
		PROGRESS_RAPID,	// �A��
		PROGRESS_CHARGE,	// �`���[�W�ˌ�
		PROGRESS_PARAM,	// �p�����[�^�[
		PROGRESS_ASSESS,	// �]��
		PROGRESS_FREE,	// �t���[
		PROGRESS_MAX
	};
	enum ACTION
	{// �A�N�V����
		ACTION_MOVE,	// �ړ�
		ACTION_BOOST,	// �u�[�X�g
		ACTION_SHOT,	// �ʏ�ˌ�
		ACTION_RAPID,	// �A�ˎˌ�
		ACTION_CHARGE,	// �`���[�W�ˌ�
		ACTION_BUTTON,	// ���Ԍo��
		ACTION_MAX
	};

	CTutorialManager(int nPriority = 3);	// �R���X�g���N�^
	~CTutorialManager();	// �f�X�g���N�^

	static CTutorialManager *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void AddProgress(ACTION action);
	PROGRESS GetProgress(void) { return m_progress; }

private:
	enum STATE
	{
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_IN,	// �o�����
		STATE_WAIT,	// �ҋ@���
		STATE_OUT,	// ���ŏ��
		STATE_MAX
	};

	void Debug(void);
	void ManageGauge(void);
	void ManageState(void);
	void NextProgress(PROGRESS progress);
	void TimeTutorial(void);
	void EndProgress(PROGRESS progress);
	bool ButtonCheck(void);
	void CreateGauge(void);

	PROGRESS m_progress;	// �i�s��
	PROGRESS m_NextProgress;	// ���̐i�s��
	int m_nCntProgress;	// �i�s�J�E���^�[
	CObject2D *m_pCaption;	// �������̃|�C���^
	CObject2D *m_pFrameSkip;	// �X�L�b�v�\���̃t���[��
	CObject2D *m_pGaugeSkip;	// �X�L�b�v�\���̃Q�[�W

	D3DXVECTOR3 m_posDest;	// �ڕW�ʒu
	STATE m_state;	// ���
	int m_nTimerOut;	// �t�F�[�h�A�E�g�܂ł̃^�C�}�[
	int m_nCntSkip;	// �X�L�b�v�J�E���^�[
};

#endif