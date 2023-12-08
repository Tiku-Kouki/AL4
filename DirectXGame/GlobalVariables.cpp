#include "GlobalVariables.h"
#include <ImGuiManager.h>


GlobalVariables* GlobalVariables::GetInstance() { 

	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::Update() {

if (!ImGui::Begin("Global variables", nullptr, ImGuiWindowFlags_MenuBar))
		return;
if (!ImGui::BeginMenuBar())
	return;

//各グループについて
for (std::map<std::string, Group>::iterator itGroup = datas_.begin();
	itGroup != datas_.end();
	 ++itGroup) {
	//グループ名を取得
	const std::string& groupName = itGroup->first;

	Group& group = itGroup->second;

	if (!ImGui::BeginMenu(groupName.c_str()))
		continue;

	// 各項目について
	for (std::map<std::string, Item>::iterator itItem = group.items.begin();
		 itItem != group.items.end(); ++itItem) {
		

		const std::string& itemName = itItem->first;

		Item& item = itItem->second;

		//int32_t型の値を保持していれば
		if (std::holds_alternative<int32_t>(item.value)) {
			int32_t* ptr = std::get_if<int32_t>(&item.value);
			ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);

		} 
		else 
		if (std::holds_alternative<float>(item.value)) {
			
			float* ptr = std::get_if<float>(&item.value);
			ImGui::SliderFloat(itemName.c_str(), ptr, 0, 100);
			
		} 
		else 
		if (std::holds_alternative<Vector3>(item.value)) {
			Vector3* ptr = std::get_if<Vector3>(&item.value);
			ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
		}


	}

	ImGui::EndMenu();
}




ImGui::EndMenuBar();
ImGui::End();

}

void GlobalVariables::CreateGroup(const std::string& groupName) {
 	//指定のオブジェクトがなければ追加する
	datas_[groupName];

}

void GlobalVariables::SetValue(
    const std::string& groupName, const std::string& key, int32_t value) {


   //グループの参照を取得
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



