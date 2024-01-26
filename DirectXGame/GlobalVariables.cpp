#include "GlobalVariables.h"
#include <ImGuiManager.h>
#include <fstream>

GlobalVariables* GlobalVariables::GetInstance() {

	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::Update() {

	if (!ImGui::Begin("Global variables", nullptr, ImGuiWindowFlags_MenuBar))
		return;
	if (!ImGui::BeginMenuBar())
		return;

	// 各グループについて
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end();
	     ++itGroup) {
		// グループ名を取得
		const std::string& groupName = itGroup->first;

		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str()))
			continue;

		// 各項目について
		for (std::map<std::string, Item>::iterator itItem = group.items.begin();
		     itItem != group.items.end(); ++itItem) {

			const std::string& itemName = itItem->first;

			Item& item = itItem->second;

			// int32_t型の値を保持していれば
			if (std::holds_alternative<int32_t>(item.value)) {
				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);

			} else if (std::holds_alternative<float>(item.value)) {

				float* ptr = std::get_if<float>(&item.value);
				ImGui::SliderFloat(itemName.c_str(), ptr, 0, 100);

			} else if (std::holds_alternative<Vector3>(item.value)) {
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
			}
		}

		//改行
		ImGui::Text("\n");


		if (ImGui::Button("Save")) {
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);

		}
		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();
}

void GlobalVariables::CreateGroup(const std::string& groupName) {
	// 指定のオブジェクトがなければ追加する
	datas_[groupName];
}

void GlobalVariables::SaveFile(const std::string& groupName) {

	// グループ名を検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	// 未登録チェック
	assert(itGroup != datas_.end());

	json root;

	root = json::object();

	// jsonオブジェクト登録
	root[groupName] = json::object();
	// 各項目について
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin();
	     itItem != itGroup->second.items.end(); ++itItem) {

		const std::string& itemName = itItem->first;

		Item& item = itItem->second;

		// int32_t型の値を保持していれば
		if (std::holds_alternative<int32_t>(item.value)) {

			root[groupName][itemName] = std::get<int32_t>(item.value);

		} else if (std::holds_alternative<float>(item.value)) {

			root[groupName][itemName] = std::get<float>(item.value);

		} else if (std::holds_alternative<Vector3>(item.value)) {

			Vector3 value = std::get<Vector3>(item.value);

			root[groupName][itemName] = json::array({value.x, value.y, value.z});
		}
	}

	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)) {

		std::filesystem::create_directory(kDirectoryPath);
	}

	// 書き込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	//書き込み用
	std::ofstream ofs;

	ofs.open(filePath);

	  if (ofs.fail()) {
		std::string message = "Failed open date file for write";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);

		return;
	  }

	  //ファイルにjson文字列を書き込む(インデント幅4)
	  ofs << std::setw(4) << root << std::endl;
	  //ファイルを閉じる
	  ofs.close();

}

void GlobalVariables::LoadFiles() {

	std::string dsk = kDirectoryPath;

	  if (!!std::filesystem::exists(dsk)) {

		return;
	  }

	  std::filesystem::directory_iterator dir_it(dsk);

	  for (const std::filesystem::directory_entry& entry : dir_it) {

		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		//.jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		// ファイル読み込み
		LoadFile(filePath.stem().string());
	  }


}

void GlobalVariables::LoadFile(const std::string& groupName) {

	 
	   // 書き込むJSONファイルのフルパスを合成する
	   std::string filePath = kDirectoryPath + groupName + ".json";
	   // 書き込み用
	   std::ifstream ifs;

	   ifs.open(filePath);

		if (ifs.fail()) {
		std::string message = "Failed open date file for write";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);

		return;
	   }

		json root;

		//json文字列からjsonのデータ構造に展開
	    ifs >> root;

		ifs.close();


		//グループを検索
	    json::iterator itGroup = root.find(groupName);

		//未登録チェック
	    assert(itGroup != root.end());

		//各アイテムについて
	    for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
	    //アイテム名を取得
		const std::string& itemName = itItem.key();

		 // int32_t型
		if (itItem->is_number_integer()) {
		// int型の値を登録
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		
		
		} else 
		//float				
		if (itItem->is_number_float()) {
			// float型の値を登録
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));
		
		} else 
		//　要素数3の配列であれば	
		if (itItem->is_array() && itItem->size() == 3) {
			//float型のjson配列登録
			Vector3 value = {itItem->at(0), itItem->at(1), itItem->at(2)};
			SetValue(groupName, itemName, value);

		}
		}




}

void GlobalVariables::SetValue(
    const std::string& groupName, const std::string& key, int32_t value) {

	// グループの参照を取得
	Group& group = datas_[groupName];

	Item newItem{};
	newItem.value = value;

	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
	// グループの参照を取得
	Group& group = datas_[groupName];

	Item newItem{};
	newItem.value = value;

	group.items[key] = newItem;
}

void GlobalVariables::SetValue(
    const std::string& groupName, const std::string& key, Vector3& value) {
	// グループの参照を取得
	Group& group = datas_[groupName];

	Item newItem{};
	newItem.value = value;

	group.items[key] = newItem;
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value) {

	

	if (!datas_.contains(key)) {
	
		SetValue(groupName, key, value);
	
	}


}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value) {

	if (!datas_.contains(key)) {

		SetValue(groupName, key, value);
	}

}

void GlobalVariables::AddItem(
    const std::string& groupName, const std::string& key, Vector3& value) {

if (!datas_.contains(key)) {

		SetValue(groupName, key, value);
	}

}

int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) const {

	assert(datas_.contains(groupName));


	const Group& group = datas_.at(groupName);

	assert(group.items.contains(key));

	
	return std::get<int32_t>(group.items.at(key).value);
}

float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) const {
	assert(datas_.contains(groupName));

	const Group& group = datas_.at(groupName);

	assert(group.items.contains(key));

	return std::get<float>(group.items.at(key).value);
}

Vector3 GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) const {
	assert(datas_.contains(groupName));

	const Group& group = datas_.at(groupName);

	assert(group.items.contains(key));

	return std::get<Vector3>(group.items.at(key).value);
}
