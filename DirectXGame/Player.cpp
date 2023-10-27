#include "Player.h"
#include "assert.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "ImGuiManager.h"

void Player::Initalize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm) { 
	
	//assert(model); 
	modelFighterBody_= modelBody;
	modelFighterHead_ = modelHead;
	modelFighterL_arm_ = modelL_arm;
	modelFighterR_arm_ = modelR_arm;

	//textureHandle = 0;

	
	//親子関係の設定
	worldTransform_[1].parent_ = &worldTransform_[0];
	worldTransform_[2].parent_ = &worldTransform_[0];
	worldTransform_[3].parent_ = &worldTransform_[0];


	worldTransform_[0].scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform_[0].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_[0].translation_ = {0.0f, 0.0f, 0.0f};

	worldTransform_[1].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_[1].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_[1].translation_ = {0.0f, 1.57f, 0.0f};

	worldTransform_[2].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_[2].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_[2].translation_ = {-0.51f, 1.26f, 0.0f};

	worldTransform_[3].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_[3].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_[3].translation_ = {0.51f, 1.26f, 0.0f};

	for (int i = 0; i < 4; i++) {
		worldTransform_[i].Initialize();
	}
	InitializeFloatingGimmick();

}

void Player::Update() {
	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.1f;


	XINPUT_STATE joyState;

	

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;

		move.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	
		//move = Multiply(kCharacterSpeed, Normalize(move));
		 
		

		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(viewProjection_->rotation_.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(viewProjection_->rotation_.z);
		
		

		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

		move = TransformNormal(move, rotateXYZMatrix);
		if (move.x != 0 || move.z != 0) {

			for (int i = 0; i < 4; i++) {
				worldTransform_[0].rotation_.y = std::atan2(move.x, move.z);
			}

		}
	}

	UpdateFloatingGimmick();
	
	

	

	for (int i = 0; i < 4; i++) {
		worldTransform_[0].translation_.x += move.x;
		worldTransform_[0].translation_.y += move.y;
		worldTransform_[0].translation_.z += move.z;

		worldTransform_[i].UpdateMatrix();
	}
	
	ImGui::Begin("player");

	ImGui::DragFloat3("Body", &worldTransform_[0].translation_.x, 0.01f);
	ImGui::DragFloat3("Head", &worldTransform_[1].translation_.x, 0.01f);
	ImGui::DragFloat3("Arm_L", &worldTransform_[2].translation_.x, 0.01f);
	ImGui::DragFloat3("Arm_R", &worldTransform_[3].translation_.x, 0.01f);

	ImGui::End();
	

}

void Player::Draw(ViewProjection &viewProjection) {

	modelFighterBody_->Draw(worldTransform_[0], viewProjection);
	modelFighterHead_->Draw(worldTransform_[1], viewProjection);
	
	modelFighterL_arm_->Draw(worldTransform_[2], viewProjection);
	modelFighterR_arm_->Draw(worldTransform_[3], viewProjection);





}


Vector3 Player::GetWorldPosition() {

	Vector3 worldPos = {};

	worldPos.x = worldTransform_[0].matWorld_.m[3][0];
	worldPos.y = worldTransform_[0].matWorld_.m[3][1];
	worldPos.z = worldTransform_[0].matWorld_.m[3][2];

	return worldPos;
}

void Player::InitializeFloatingGimmick() {


	floatingParameter_ = 0.0f;

}

void Player::UpdateFloatingGimmick() {

	// 浮遊移動のサイクル<frame>
	const uint16_t period = 160;
	// 1フレームでのパラメータ加数値
	const float step = float(2.0f * M_PI) / period;
	//　パラメータを1ステップ分加算
	floatingParameter_ += step;
	//　2πを超えたら0にも戻す
	floatingParameter_ = std::fmod(floatingParameter_, float(2.0f * M_PI));

	const float swingWidth = 1;

	
	for (int i = 0; i < 4; i++) {
		worldTransform_[0].translation_.y = std::sin(floatingParameter_) * swingWidth;
	}
}
