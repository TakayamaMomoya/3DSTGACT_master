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
		PROGRESS_SHOT,	// �ˌ�
		PROGRESS_CHARGE,	// �`���[�W�ˌ�
		PROGRESS_MAX
	};
	enum ACTION
	{// �A�N�V����
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
	void Draw(void) {};

	void AddProgress(ACTION action);

private:
	PROGRESS m_progress;	// �i�s��
};

#endif