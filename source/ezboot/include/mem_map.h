//------------------------------------------------------------------------------
// �� �� �� : mem_map.h
// ������Ʈ : ezboot
// ��    �� : ezboot���� ����ϴ� �� ���� ���ǿ� ���õ� ��� 
// �� �� �� : ����â
// �� �� �� : 2003�� 5�� 21��
// �� �� �� : 2003-10-16	PCB Ver 1.1 �� �°� ����
//                              - BackLight �� ����
// ��    �� : 
//------------------------------------------------------------------------------

#ifndef _MEM_MAP_HEADER_
#define _MEM_MAP_HEADER_

#define LINUX_MACH_TYPE                 303

#define BANK0_START                     0xA0000000              // ��ũ0 ���� ��ġ
#define BANK0_SIZE                      (64*1024*1024)          // ��ũ0 ũ�� 

#define DEFAULT_nCS_CS8900_BASE         (PXA_CS0_PHYS+0x00400000 ) 

#define DEFAULT_nCS_NAND_BASE           (PXA_CS1_PHYS+0x00000000 ) 
#define DEFAULT_nCS_MK712_BASE          (PXA_CS1_PHYS+0x00400000 ) 

#define DEFAULT_FLASH_BOOT              0x00000000              // �÷������� ��Ʈ ���� ��巹�� 
#define DEFAULT_BOOT_SIZE              	(128*1024)		// ��Ʈ������ ũ��
#define DEFAULT_BOOT_PARAM         	(DEFAULT_FLASH_BOOT+DEFAULT_BOOT_SIZE)	// �÷������� ��Ʈ �Ķ���� ����
#define	DEFAULT_BOOT_PARAM_SIZE		(2*1024)
							   	
                                   
#define DEFAULT_RAM_BOOT_START          0xA0F00000              // ������ ��Ʈ     ���� ��巹�� 
#define DEFAULT_RAM_BOOT_PARAMS         0xA0000100              // Ŀ�� ��Ʈ �Ķ���� ����   
#define DEFAULT_RAM_KERNEL_START        0xA0008000              // ������ Ŀ��     ���� ��巹�� 
#define DEFAULT_RAM_RAMDISK_START       0xA0800000              // ������ ����ũ ���� ��巹�� 
#define DEFAULT_RAM_WORK_START          0xA1000000              // �Ϲ����� ��Ʈ �δ� �۾� ���� 

#define DEFAULT_RAM_KERNEL_ZERO_PAGE    0xA0000000              // ��Ʈ�δ����� Ŀ�η� �����ϴ� ���� ���� ��巹�� 


#define DEFAULT_RAM_KERNEL_IMG_SIZE    	(  1*1024*1024)
#define DEFAULT_RAM_RAMDISK_IMG_SIZE   	(  4*1024*1024)
#define DEFAULT_RAM_RAMDISK_SIZE       	(  8*1024*1024)

#define DEFAULT_PARAM_SIZE             	(  2*1024)


#endif //_MEM_MAP_HEADER_

