#include "Assert.h"
#include <stdlib.h>
#include <stdio.h>


//�ǂ��̃t�@�C���̉��s�ڂŃG���[���N�������t�@�C���ɏo�͂��܂�
//Assert�t�@�C�����C���N���[�h����Ɨ��p�ł��܂�
//�Ⴆ�΁A�v���C���[��nullptr��������G���[���o���Ȃǂ��ł��܂�
//�g����
//if(�������������ځA�܂������~�X������G���[�o�Ăق������ĂƂ���){
//		Assert(__FILE__,__LINE__);
//		Assert("�װ÷��",__LINE__); //�e�L�X�g���o�͂���
//			or 
//		exit(1);	//�����I���inullptr�̎��Ƃ��A���̂܂ܐi�񂾂獢��Ƃ��j
//	}
#ifdef _DEBUG
	#define ASSERT(X,Y)  Assert(char* file, int line) 

//���ߍ��ނƂ���Assert(__FILE__,__LINE__);�ł��肢���܂�
void Assert(char* file, int line) 
{
	FILE *fp;
	fopen_s(&fp, "error.txt", "a");

	if (fp == nullptr) {
		exit(1);
	}
	fprintf(fp, "\"%s,\"%d\"\n", file, line);
	fclose;
}
#else
	#define ASSERT(X,Y)
#endif