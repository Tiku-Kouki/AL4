﻿#pragma once
#include "Affine.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <list>
#include "Windows.h"
#include "FollowCamera.h"

class Player {

 public:

	void Initalize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm);


	void Update();



	void Draw(ViewProjection &viewProjection);

	Vector3 GetWorldPosition();

	const WorldTransform& GetWorldTransform() { return worldTransform_[0]; }

	void SetViewProjection(const ViewProjection* viewProjection) {viewProjection_ = viewProjection;}

	//浮遊ギミック初期化
	void InitializeFloatingGimmick();
	//浮遊ギミック更新
	void UpdateFloatingGimmick();

private:

	WorldTransform worldTransform_[4];

	Model* model_ = nullptr;

	// 3Dモデル
	Model* modelFighterBody_ = nullptr;
	Model* modelFighterHead_ = nullptr;
	Model* modelFighterL_arm_ = nullptr;
	Model* modelFighterR_arm_ = nullptr;


	Input* input_ = nullptr;

	uint32_t textureHandle_ = 0u;

	//　カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;




	};
