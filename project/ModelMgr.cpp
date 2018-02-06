#include <map>
#include <string>
#include <vector>
#include "DxLib.h"
#include "GameMain.h"
#include "ModelMgr.h"

//ModelMgr* ModelMgr::ptr = nullptr;

ModelMgr::ModelMgr()
{

}

ModelMgr::~ModelMgr()
{

}

ModelMgr& ModelMgr::Instance(void)
{
	static ModelMgr modelMgr;
	return modelMgr;
}

int ModelMgr::ModelIdReturn(const std::string path, const SCENE_TYPE delScene)
{
	if (ModelID.count(path) == 0) {
		ModelID[path] = MV1LoadModel(path.c_str());
	}
	deleteTiming[path] = delScene;
	return ModelID[path];
}

void ModelMgr::ModelManager(const SCENE_TYPE deleScene)
{
	auto itr = deleteTiming.begin();

	while (itr != deleteTiming.end()) {
		if (itr->second == deleScene) {
			MV1DeleteModel(ModelID[itr->first]);

			ModelID.erase(ModelID.find(itr->first));
			itr = deleteTiming.erase(itr);
		}
		else {
			itr++;
		}
	}
}

void ModelMgr::ModelIdAllDelete(void)
{
	for (auto itr = ModelID.begin(); itr != ModelID.end(); itr++) {
		MV1DeleteModel(itr->second);
	}
	ModelID.erase(ModelID.begin(), ModelID.end());
	deleteTiming.erase(deleteTiming.begin(), deleteTiming.end());
}

void ModelMgr::Draw(int modelhandle, float DotLineNum)
{
	materialNum = MV1GetMaterialNum(modelhandle);

	for (int i = 0; i < materialNum; i++)
	{
		MV1SetMaterialOutLineWidth(modelhandle,i, DotLineNum);
		MV1SetMaterialOutLineDotWidth(modelhandle, i, DotLineNum);
	}
	MV1DrawModel(modelhandle);
}
