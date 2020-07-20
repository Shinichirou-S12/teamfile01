//! @file	checkhit.h
//! @author	
//! @date	2020-06-18
//! @brief	�Փ˔���̒�`

#ifndef ___CHECKHIT_H___
#define ___CHECKHIT_H___

enum CHECKBLOCK
{
	TOP,
	UNDER,
	LEFT_MAPCHIP,
	RIGHT_MAPCHIP,
	NOTTOUCHED,
	MAXCHECKBLOCK
};

/**************************************************************************//**
	@brief		CheckHitBB_MAP
	@brief		BBMAP�̏Փ˔���
	@param[in]	pos1		pos1�̍��W
	@param[in]	pos2		pos2�̍��W
	@param[in]	size1		size1�̑傫��
	@param[in]	size2		size2�̑傫��
	@param[in]	moveSpeed	�ړ����x
	@return		true	�������Ă���
				false	�������Ă��Ȃ�
*//***************************************************************************/
int CheckHitBB_MAP(D3DXVECTOR3 pos1, D3DXVECTOR3 pos_mapchip, D3DXVECTOR2 size1, D3DXVECTOR2 size_mapchip, float moveSpeed);

/**************************************************************************//**
	@brief		CheckHitBB
	@brief		BB�̏Փ˔���
	@param[in]	pos1	pos1�̍��W
	@param[in]	pos2	pos2�̍��W
	@param[in]	size1	size1�̑傫��
	@param[in]	size2	size2�̑傫��
	@return		true	�������Ă���
				false	�������Ă��Ȃ�
*//***************************************************************************/
bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2);

/**************************************************************************//**
	@brief		CheckHitBC
	@brief		BC�̓����蔻��
	@param[in]	pos1	pos1�̍��W
	@param[in]	pos2	pos2�̍��W
	@param[in]	size1	size1�̑傫��
	@param[in]	size2	size2�̑傫��
	@return		true	pos1��pos2���������Ă���
				false	pos1��pos2���������Ă��Ȃ�
*//***************************************************************************/
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);

/**************************************************************************//**
	@brief		FallCheckHitPlayer
	@brief		�v���C���[�̗��������̎��̓����蔻��
	@param		�Ȃ�
	@return		�Ȃ�
*//***************************************************************************/
void FallCheckHitPlayer(void);

/**************************************************************************//**
	@brief		Restriction
	@brief		�v���C���[�̉��ړ��ŉ�ʊO��u���b�N���ђʂ��Ȃ����߂̏���
	@param		�Ȃ�
	@return		�Ȃ�
*//***************************************************************************/
void Restriction(void);

#endif