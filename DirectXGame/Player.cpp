#include "Player.h"
#include "assert.h"



void Player::Initalize(Model* model, uint32_t textureHandle) { 
	
	assert(model); 
	model_ = model;
	textureHandle = 0;

	
	worldTransform_.scale_ = {10.0f, 10.0f, 10.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};


	worldTransform_.Initialize();
}

void Player::Update() {

	worldTransform_.TransferMatrix();

}

void Player::Draw(ViewProjection &viewProjection) {

model_->Draw(worldTransform_, viewProjection);

}


