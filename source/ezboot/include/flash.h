//------------------------------------------------------------------------------
// ȭ�ϸ�   : flash.h
// ������Ʈ : ezboot_x5
// ��  ��   : NOR, NAND �÷����� ���� ó�� ��ƾ�̴�.
//            ������ƾ�� ȣ���Ͽ� ����� ���ð� ��Ī�Ѵ�.
// 
// �ۼ���   : ����� (��)���̴���Ƽ freefrug@falinux.com   -- Ǫ��
// �ۼ���   : 2003�� 6�� 7��
//
// ���۱�   : (��)���̴���Ƽ 
//
//------------------------------------------------------------------------------

#ifndef _FLASH_HEADER_
#define _FLASH_HEADER_

#include <flash_29lvx.h>
#include <nand.h>

extern void CopyImage( void );
extern int  ErasePartiotion( int argc, char **argv);

#endif // _FLASH_HEADER_
