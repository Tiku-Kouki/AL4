#include "Player.h"
#include "assert.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "ImGuiManager.h"
#include "GlobalVariables.h"


void Player::Initalize(const std::vector<Model*>& models) { 
	
	BaseCharacter::Initalize(models);

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	GlobalVariables::GetInstance()->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "Test", 90);

	

	globalVariables->SaveFile("Player");

	
	//親子関係の設定
	worldTransform_[1].parent_ = &worldTransform_[0];
	worldTransform_[2].parent_ = &worldTransform_[0];
	worldTransform_[3].parent_ = &worldTransform_[0];

	hammer.parent_ = &worldTransform_[0];

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


	globalVariables->AddItem(groupName, "Head", worldTransform_[1].translation_);
	globalVariables->AddItem(groupName, "ArmL", worldTransform_[2].translation_);
	globalVariables->AddItem(groupName, "ArmR", worldTransform_[3].translation_);
	
	  

	hammer.scale_ = {1.0f, 1.0f, 1.0f};

	hammer.rotation_ = {0.0f, 0.0f, 0.0f};

	hammer.translation_ = {0.0f, 1.0f, 0.0f};

	for ( int i = 0; i < 4; i++) {
		worldTransform_[i].Initialize();
	}
	hammer.Initialize();
	InitializeFloatingGimmick();

}

void Player::Update() {
	
	

	if (behaviorRequest_) {
	
	behavior_ = behaviorRequest_.value();
	
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootInitialize();
		break;
	case Behavior::kAttack:
		
		BehaviorAttackInitialize();
		
		break;

		case Behavior::kJump:

		BehaviorJumpInitialize();

		break;
	}
	   //振る舞いリセット
	  behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
	case Behavior::kRoot:
	default:
	  BehaviorRootUpdate();
	  
	  break;
	case Behavior::kAttack:

	  BehaviorAttackUpdate();

	  break;

	  case Behavior::kJump:

	  BehaviorJumpUpdate();

	  break;
	}

	 


	//BehaviorRootUpdate();
	//BehaviorAttackUpdate();

	for (int i = 0; i < 4; i++) {
		

		worldTransform_[i].UpdateMatrix();


	}

	

	hammer.UpdateMatrix();


	ImGui::Begin("player");

	ImGui::DragFloat3("Body", &worldTransform_[0].translation_.x, 0.01f);
	ImGui::DragFloat3("Head", &worldTransform_[1].translation_.x, 0.01f);
	ImGui::DragFloat3("Arm_L", &worldTransform_[2].translation_.x, 0.01f);
	ImGui::DragFloat3("Arm_R", &worldTransform_[3].translation_.x, 0.01f);

	ImGui::DragFloat3("hammer", &worldTransform_[2].rotation_.x, 0.01f);
	ImGui::DragFloat3("hammer", &worldTransform_[3].rotation_.x, 0.01f);
	ImGui::DragFloat3("hammer", &hammer.rotation_.x, 0.01f);

	ImGui::End();

	ApplyGrobalVariables();

}

void Player::Draw(ViewProjection& viewProjection){

	for (int i = 0; i < 4; i++) {
		models_[i]->Draw(worldTransform_[i], viewProjection);
	}

	if (behavior_ == Behavior::kAttack) {

		models_[4]->Draw(hammer, viewProjection);
	}
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

void Player::BehaviorRootUpdate() {

   BaseCharacter::Update();

	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		velocity_ = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX};


		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_B) {

		behaviorRequest_ = Behavior::kAttack;
		}

		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A) {

		behaviorRequest_ = Behavior::kJump;
		}

		 velocity_ = Multiply(kCharacterSpeed, Normalize(velocity_));

		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(viewProjection_->rotation_.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(viewProjection_->rotation_.z);

		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

		velocity_ = TransformNormal(velocity_, rotateXYZMatrix);

		if (velocity_.x != 0 || velocity_.z != 0) {

			for (int i = 0; i < 4; i++) {
			worldTransform_[0].rotation_.y = std::atan2(velocity_.x, velocity_.z);
			}
		}
	}

	UpdateFloatingGimmick();

	for (int i = 0; i < 4; i++) {
		worldTransform_[0].translation_.x += velocity_.x;
		worldTransform_[0].translation_.y += velocity_.y;
		worldTransform_[0].translation_.z += velocity_.z;

		worldTransform_[i].UpdateMatrix();
	}

	
	



}

void Player::BehaviorAttackUpdate() {

	X += 0.05f;

	const float c1 = 1.70158f;
	const float c3 = c1 + 1;
	
	if (X < 1.13) {

		
	Ease = c3 * X * X * X - c1 * X * X;
	}

	
	worldTransform_[2].rotation_.x = Ease + 3.0f;

	  worldTransform_[3].rotation_.x = Ease + 3.0f;
	
	hammer.rotation_.x = Ease;

	if (X >= 2.6) {
	

	X = 0;
	
	behaviorRequest_ = Behavior::kRoot;

	}



}

void Player::BehaviorRootInitialize() {


	worldTransform_[2].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_[2].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_[2].translation_ = {-0.51f, 1.26f, 0.0f};

	worldTransform_[3].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_[3].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_[3].translation_ = {0.51f, 1.26f, 0.0f};

}

void Player::BehaviorAttackInitialize() {

	worldTransform_[0].translation_.y = 0.0f;

	worldTransform_[2].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_[2].rotation_ = {3.0f, 0.0f, 0.0f};
	worldTransform_[2].translation_ = {-0.51f, 1.26f, 0.0f};

	worldTransform_[3].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_[3].rotation_ = {3.0f, 0.0f, 0.0f};
	worldTransform_[3].translation_ = {0.51f, 1.26f, 0.0f};

	hammer.scale_ = {1.0f, 1.0f, 1.0f};

	hammer.rotation_ = {0.0f, 0.0f, 0.0f};

	hammer.translation_ = {0.0f, 1.0f, 0.0f};
	X = 0;

}

void Player::BehaviorJumpInitialize() {
	worldTransform_[0].translation_.y = 0.0f;

	worldTransform_[2].rotation_.x = 0;
	;
	worldTransform_[3].rotation_.x = 0;

	const float kJumpFirstSpeed = 1.0f;

	velocity_.y = kJumpFirstSpeed;
}

void Player::BehaviorJumpUpdate() {

	worldTransform_[0].translation_.x += velocity_.x;

	worldTransform_[0].translation_.y += velocity_.y;

	worldTransform_[0].translation_.z += velocity_.z;

	const float kGravityAcceleration = 0.06f;

	Vector3 accelerationVector = {0, -kGravityAcceleration, 0};

	velocity_ = Add(velocity_, accelerationVector);

	if (worldTransform_[0].translation_.y < 0.0f) {
	worldTransform_[0].translation_.y = 0;

	behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::ApplyGrobalVariables() {
	
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	worldTransform_[1].translation_ = globalVariables->GetVector3Value(groupName, "Head");

	worldTransform_[2].translation_ = globalVariables->GetVector3Value(groupName, "ArmL");

	worldTransform_[3].translation_ = globalVariables->GetVector3Value(groupName, "ArmR");



}
