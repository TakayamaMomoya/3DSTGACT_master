//*****************************************************
//
// �o�̊Ǘ�[dustmanager.h]
// Author:���R����
//
//*****************************************************

#ifndef _DUSTMANAGER_H_
#define _DUSTMANAGER_H_

//*****************************************************
// �C���N���[�h
//****************************************************
#include "object.h"

//*****************************************************
// �N���X��`
//****************************************************
class CDustManager : public CObject
{
public:
	CDustManager();	// �R���X�g���N�^
	~CDustManager();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CDustManager *Create(void);

private:
	void SetTimer(void);

	int m_nTimer;	// ���̐o���o���܂ł̃^�C�}�[
};

#endif