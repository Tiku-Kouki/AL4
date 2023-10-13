#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <list>
#include "Windows.h"

class Player {
public:

	void Initalize(Model* model_, uint32_t textureHandle);


	void Update();



	void Draw(ViewProjection &viewProjection);

	Vector3 GetWorldPosition();

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	Input* input_ = nullptr;

	uint32_t textureHandle_ = 0u;

	};
