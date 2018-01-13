#pragma once
class ModelMgr
{
private:
	ModelMgr();

	std::map<std::string, int> ModelID;					//モデルのハンドル
	std::map < std::string, SCENE_TYPE> deleteTiming;	//モデルの消去タイミング
public:
	~ModelMgr();
	static ModelMgr& Instance();
	int ModelIdReturn(const std::string path, const SCENE_TYPE delScene);	//モデルのハンドルを返す
	
};

