#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("mario.png");

	model_.reset(Model::CreateFromOBJ("player", true));

	viewProjection_.Initialize();

	player_ = std::make_unique<Player>();

	player_->Initalize(model_.get(), textureHandle_);

	debugCamera_ = new DebugCamera(1280, 720);

	skydome_ = std::make_unique<Skydome>();
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	skydome_->Initialize(modelSkydome_.get());

	ground_ = std::make_unique<Ground>();
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	ground_->Initialize(modelGround_.get());
}

void GameScene::Update() {

	player_->Update();
	skydome_->Update();
	ground_->Update();

#ifdef _DEBUG

	if (input_->PushKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}

#endif // DEBUG
	if (isDebugCameraActive_ == true) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
	} else {

		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player_->Draw(viewProjection_);

	skydome_->Draw(viewProjection_);

	ground_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
