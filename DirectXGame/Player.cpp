#include "Player.h"
#include "assert.h"



void Player::Initalize(Model* model, uint32_t textureHandle) { 
	
	assert(model); 
	model_ = model;
	textureHandle = 0;

	
	worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};


	worldTransform_.Initialize();
}

void Player::Update() {
	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;


	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;

		move.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.UpdateMatrix();

	

}

void Player::Draw(ViewProjection &viewProjection) {

model_->Draw(worldTransform_, viewProjection);

}


Vector3 Player::GetWorldPosition() {

	Vector3 worldPos = {};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}