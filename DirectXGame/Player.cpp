#include "Player.h"
#include "assert.h"



void Player::Initalize(Model* model, uint32_t textureHandle) { 
	
	assert(model); 
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
}

void Player::Update() {

	worldTransform_.TransferMatrix();

}

void Player::Draw(ViewProjection &viewProjection) {

model_->Draw(worldTransform_, viewProjection, textureHandle_);

}


