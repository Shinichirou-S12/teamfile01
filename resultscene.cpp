//! @file	resultscene.cpp
//! @author	�܂�
//! @date	2020-07-13
//! @brief	���U���g�V�[���̎���

// �C���N���[�h�t�@�C��
#include "main.h"
#include "result.h"
#include "resultscene.h"

// �}�N����`

// ����������
void InitResultScene(void)
{
	// ���S�Ɣw�i�̏���������
	InitResult(0);
}

// �I������
void UninitResultScene(void)
{
	// ���S�Ɣw�i�̏I������
	UninitResult();
}

// �X�V����
void UpdateResultScene(void)
{
	// ���S�Ɣw�i�̍X�V����
	UpdateResult();
}

// �`�揈��
void DrawResultScene(void)
{
	// ���S�Ɣw�i�̕`�揈��
	DrawResult();
}