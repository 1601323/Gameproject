#pragma once
#include "GameMain.h"

class ModelMgr
{
private:
	ModelMgr();
	ModelMgr(const ModelMgr&);
	static ModelMgr* ptr;
	
	std::map<std::string, int> ModelID;					//���f���̃n���h��
	std::map < std::string, SCENE_TYPE> deleteTiming;	//���f���̏����^�C�~���O
	int materialNum;

public:
	~ModelMgr();

	static ModelMgr* Instance(void);
	int ModelIdReturn(const std::string path, const SCENE_TYPE delScene);
	void SetMaterialDotLine(int modelhandle, float DotLineNum);
	void ModelManager(const SCENE_TYPE deleScene);
	void ModelIdAllDelete(void);
};

