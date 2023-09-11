//*****************************************************
//
// ���b�N�I���̏���[lockon.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "lockon.h"
#include "billboard.h"
#include "camera.h"
#include "manager.h"
#include "enemy.h"
#include "texture.h"
#include "camera.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define PATH_LOCKON	("data\\TEXTURE\\UI\\lockon002.png")	// ���b�N�I���}�[�J�[�̃p�X
#define MOVE_FACT	(0.3f)	// �������x
#define DEFAULT_SIZE	(200.0f)		// ��̑傫��
#define LOCK_SIZE	(80.0f)		// ���b�N�I�����̑傫��
#define DEFAULT_DIST	(2000.0f)		// ��̋���
#define SIZE_FACT	(0.1f)		// �傫���̕ς�鑬��

//=====================================================
// �R���X�g���N�^
//=====================================================
CLockon::CLockon(int nPriority)
{
	m_pLockOn = nullptr;
	m_pEnemy = nullptr;
	m_posDest = { 0.0f,0.0f,0.0f };
	m_fSize = 0.0f;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CLockon::~CLockon()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CLockon::Init(void)
{
	// ���b�N�I���}�[�J�[�̐���
	m_pLockOn = CBillboard::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), DEFAULT_SIZE, DEFAULT_SIZE);

	if (m_pLockOn != nullptr)
	{// ���b�N�I���}�[�J�[�̏����ݒ�
		int nIdx = CManager::GetTexture()->Regist(PATH_LOCKON);
		m_pLockOn->SetIdxTexture(nIdx);
		m_pLockOn->SetZTest(true);
	}

	m_fSize = DEFAULT_SIZE;

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CLockon::Uninit(void)
{
	if (m_pLockOn != nullptr)
	{
		m_pLockOn->Uninit();

		m_pLockOn = nullptr;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CLockon::Update(void)
{
	D3DXVECTOR3 vecDiff;
	D3DXVECTOR3 pos;

	// �J�����擾
	CCamera *pCamera = CManager::GetCamera();
	CCamera::Camera *pCameraInfo = nullptr;

	if (pCamera != nullptr)
	{
		pCameraInfo = pCamera->GetCamera();

		if (pCameraInfo != nullptr)
		{
			// �����̃x�N�g�����擾
			vecDiff = m_pLockOn->GetPosition() - pCameraInfo->posV;

			// �����̒������擾
			float fLength = D3DXVec3Length(&vecDiff);

			// ��̋����Ƃ̊������Z�o
			float fSize = fLength / DEFAULT_DIST;
			fSize *= m_fSize;

			// �T�C�Y�ݒ�
			m_pLockOn->SetSize(fSize, fSize);
		}
	}

	if (m_pEnemy != nullptr && m_pLockOn != nullptr)
	{
		m_posDest = m_pEnemy->GetPosition();

		m_fSize += (LOCK_SIZE - m_fSize) * SIZE_FACT;
	}
	else
	{
		m_fSize += (DEFAULT_SIZE - m_fSize) * SIZE_FACT;
	}

	vecDiff = m_posDest - m_pLockOn->GetPosition();

	pos = m_pLockOn->GetPosition() + vecDiff * MOVE_FACT;

	m_pLockOn->SetPosition(pos);
}

//=====================================================
// ��������
//=====================================================
CLockon *CLockon::Create(void)
{
	CLockon *pLockon = nullptr;

	if (pLockon == nullptr)
	{// �C���X�^���X����
		pLockon = new CLockon;

		// ����������
		pLockon->Init();
	}

	return pLockon;
}