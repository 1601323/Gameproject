#pragma once
class ModelMgr
{
private:
	ModelMgr();

	std::map<std::string, int> ModelID;					//���f���̃n���h��
	std::map < std::string, SCENE_TYPE> deleteTiming;	//���f���̏����^�C�~���O
public:
	~ModelMgr();
	static ModelMgr& Instance();
	int ModelIdReturn(const std::string path, const SCENE_TYPE delScene);	//���f���̃n���h����Ԃ�
	
};

