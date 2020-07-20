//! @file	checkhit.cpp
//! @author	�܂�
//! @date	2020-06-18
//! @brief	�Փ˔���̎���

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "checkhit.h"
#include "player.h"
#include "file.h"

//=============================================================================
// BB�ɂ��}�b�v�Ɖ����̓����蔻��
// ��]�͍l�����Ȃ�
// �߂�l�F�}�b�v�̐G��Ă���ʒu�ɉ����ā@RIGHT LEFT TOP UNDER NOTTOUCHED ��Ԃ�
//=============================================================================
int CheckHitBB_MAP(D3DXVECTOR3 pos1, D3DXVECTOR3 pos_mapchip, D3DXVECTOR2 size1, D3DXVECTOR2 size_mapchip, float moveSpeed)
{
	if ((pos1.x + size1.x > pos_mapchip.x - size_mapchip.x) && (pos1.x - size1.x < pos_mapchip.x + size_mapchip.x) &&
		(pos1.y + size1.y > pos_mapchip.y - size_mapchip.y) && (pos1.y + size1.y < pos_mapchip.y + size_mapchip.y))		// �Ώۂ̉��ӂ��}�b�v�`�b�v�ɐG��Ă��邩
	{
		return TOP;	// �}�b�v�`�b�v�̏�ɐG��Ă���
	}

	if ((pos1.x + size1.x > pos_mapchip.x - size_mapchip.x + moveSpeed) && (pos1.x - size1.x < pos_mapchip.x + size_mapchip.x - moveSpeed) &&	// PLAYER_MOVE���������Ă���̂́A�c�ɂȂ������u���b�N�ɑ΂��āu�����͂����Ȃ���W�����v�v���ł��Ȃ��Ȃ�o�O��}�����邽�߁B
		(pos1.y - size1.y > pos_mapchip.y - size_mapchip.y) && (pos1.y - size1.y < pos_mapchip.y + size_mapchip.y))		// �Ώۂ̏�ӂ��}�b�v�`�b�v�ɐG��Ă��邩
	{
		return UNDER;	// �}�b�v�`�b�v�̉��ɐG��Ă���
	}

	if ((pos1.x + size1.x > pos_mapchip.x - size_mapchip.x) && (pos1.x + size1.x < pos_mapchip.x + size_mapchip.x) &&
		(pos1.y + size1.y > pos_mapchip.y - size_mapchip.y) && (pos1.y - size1.y < pos_mapchip.y + size_mapchip.y)) 	// �Ώۂ̉E�ӂ��}�b�v�`�b�v�ɐG��Ă��邩
	{
		return LEFT_MAPCHIP;	// �}�b�v�`�b�v�̍��ɐG��Ă���
	}

	if ((pos1.x - size1.x > pos_mapchip.x - size_mapchip.x) && (pos1.x - size1.x < pos_mapchip.x + size_mapchip.x) &&
		(pos1.y + size1.y > pos_mapchip.y - size_mapchip.y) && (pos1.y - size1.y < pos_mapchip.y + size_mapchip.y))	// �Ώۂ̍��ӂ��}�b�v�`�b�v�ɐG��Ă��邩
	{
		return RIGHT_MAPCHIP;	// �}�b�v�`�b�v�̉E�ɐG��Ă���
	}

	return NOTTOUCHED;
}


//=============================================================================
// BB�ɂ�铖���蔻�菈��
// ��]�͍l�����Ȃ�
// �߂�l�F�������Ă���true
//=============================================================================
bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2)
{
	size1 /= 2.0f;	// ���T�C�Y�ɂ���
	size2 /= 2.0f;	// ����

	if (pos2.x + size2.x > pos1.x - size1.x && pos1.x + size1.x > pos2.x - size2.x &&
		pos2.y + size2.y > pos1.y - size1.y && pos1.y + size1.y > pos2.y - size2.y)
	{
		return true;
	}

	return false;
}

//=============================================================================
// BC�ɂ�铖���蔻�菈��
// �T�C�Y�͔��a
// �߂�l�F�������Ă���true
//=============================================================================
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{
	if ((size1 + size2) * (size1 + size2) >
		(pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y))
	{
		return true;
	}

	return false;
}
