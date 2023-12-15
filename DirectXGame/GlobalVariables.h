#pragma once
#include "Vector3.h"
#include <map>
#include <string>
#include <variant>
#include <json.hpp>

class GlobalVariables final {

public:
	static GlobalVariables* GetInstance();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	

	/// <summary>
	/// グループの作成
	/// </summary>
	///<param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	///<param name="groupName">グループ</param>
	void SaveFile(const std::string& groupName);


	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	void SetValue(const std::string& groupName, const std::string& key, float value);
	void SetValue(const std::string& groupName, const std::string& key, Vector3& value);





private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables& obj) = delete;
	GlobalVariables& operator=(const GlobalVariables& obj) = delete;


	//
	struct Item {
		// 項目の値
		std::variant<int32_t, float, Vector3> value;
	};

	struct Group {
		std::map<std::string, Item> items;
	};

	// 全データ
	std::map<std::string, Group> datas_;


	using json=nlohmann::json;

	const std::string kDirectoryPath = "Resources/GrobalVariables/";


};
