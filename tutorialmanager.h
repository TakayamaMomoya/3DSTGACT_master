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
// �N���X�̒�`
//*****************************************************
class CTutorialManager : public CObject
{
public:
	enum PROGRESS
	{// �i�s��
		PROGRESS_START,	// �J�n
		PROGRESS_MOVE,	// �ړ�
		PROGRESS_SHOT,	// �ˌ�
		PROGRESS_CHARGE,	// �`���[�W�ˌ�
		PROGRESS_MAX
	};
	enum ACTION
	{// �A�N�V����
		ACTION_MOVE,	// �ړ�
		ACTION_SHOT,	// �ʏ�ˌ�
		ACTION_RAPID,	// �A�ˎˌ�
		ACTION_CHARGE,	// �`���[�W�ˌ�
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
	void Debug(void);

	PROGRESS m_progress;	// �i�s��
	int m_nCntProgress;	// �i�s�J�E���^�[
};

#endif