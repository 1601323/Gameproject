#include "Assert.h"
#include <stdlib.h>
#include <stdio.h>


//どこのファイルの何行目でエラーが起きたかファイルに出力します
//Assertファイルをインクルードすると利用できます
//例えば、プレイヤーがnullptrだったらエラーを出すなどができます
//使い方
//if(ﾁｪｯｸしたい項目、またここミスったらエラー出てほしいってところ){
//		Assert(__FILE__,__LINE__);
//		Assert("ｴﾗｰﾃｷｽﾄ",__LINE__); //テキストを出力する
//			or 
//		exit(1);	//強制終了（nullptrの時とか、そのまま進んだら困るとき）
//	}
#ifdef _DEBUG
	#define ASSERT(X,Y)  Assert(char* file, int line) 

//埋め込むときはAssert(__FILE__,__LINE__);でお願いします
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