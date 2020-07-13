//! @file	scene.h
//! @author	�܂�
//! @date	2020-07-13
//! @brief	�V�[���̒�`
#pragma once

// �C���N���[�h�t�@�C��
#include "main.h"

// �V�[���J�ڗp�񋓌^
enum SCENE
{
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_MAX
};

// �v���g�^�C�v�錾

// �V�[���̏�����
void InitScene(void);

// �V�[���̏I������
void UninitScene(void);

// �V�[���̍X�V����
void UpdateScene(void);

// �V�[���̕`�揈��
void DrawScene(void);

// �V�[���؂�ւ�
void SetScene(int scene);

// �V�[���̎擾
int GetScene(void);