/*******************************************************************************
* �^�C�g��:		�t�@�C���w�b�_�[
* �v���O������:	file.h
* �쐬��:		
* �쐬��:		2020/06/21
*******************************************************************************/
#ifndef FILE_H
#define FILE_H

#include "result_score.h"
#include "score.h"

/*******************************************************************************
* �C���N���[�h�t�@�C��****
*******************************************************************************/


/*******************************************************************************
* �}�N����`
*******************************************************************************/
#define	FILE_MAX			(5)								// �t�@�C����


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
void SaveData(void);
void LoadData(void);

SAVEDATA *GetSaveData(void);

#endif