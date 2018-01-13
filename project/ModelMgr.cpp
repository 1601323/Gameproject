#include <map>
#include <string>
#include "GameMain.h"
#include "ModelMgr.h"

ModelMgr::ModelMgr()
{
}

ModelMgr::~ModelMgr()
{
}

ModelMgr& ModelMgr::Instance()
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
