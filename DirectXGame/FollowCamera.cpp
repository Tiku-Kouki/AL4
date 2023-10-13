#include "FollowCamera.h"
#include "ImGuiManager.h"
#include "cassert"
void FollowCamera::Initialize() { 
	
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	worldTransform_.translation_ = {1.0f, 1.0f, 1.0f};
	
	viewProjection_.Initialize();


}

void FollowCamera::Update() {

	if (target_) {
	
	 Vector3 offset = {0.0f, 10.0f, -20.0f};


	viewProjection_.translation_ = Add(target_->translation_, offset);
	
	
	}


	
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	ImGui::Begin("Camera");

	ImGui::DragFloat3("Translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Rotation", &worldTransform_.rotation_.x, 0.01f);

	ImGui::End();

}
Vector3 FollowCamera::GetWorldPosition() {
	Vector3 worldPos = {};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}