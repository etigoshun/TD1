#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"
#include <cassert>
#include "EnemyBullet.h"
#include <memory>
#include <list>
#include "DebugText.h"

//���@�N���X�̑O���錾
class Player;

//GameScene�̑O���錾
class GameScene;

//�s���t�F�[�Y
enum class Phase
{
	Approach,	//�ڋ߂���
	Leave,		//���E����
};

class Enemy
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="texturehandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle,const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// �e����
	/// </summary>
	void Fire();

	//�ڋ߃t�F�[�Y������
	void ApproachInitialize();

	//�v���C���[�̃Z�b�^�[
	void SetPlayer(Player* player) { player_ = player; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�e���X�g���擾
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//�Q�[���V�[���̃Z�b�^�[
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//���W���擾
	Vector3 GetTransPosition();

public:
	//���ˊԊu
	static const int kFireInterval = 60;

	//���a
	const float radius = 1.0f;

	bool IsDead() const { return isDead_; }

private:

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 velocity_;

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//�e
	//std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//���˃^�C�}�[
	int32_t kFireTimerCount = 0;

	//���L����
	Player* player_ = nullptr;

	//�Q�[���V�[��
	GameScene* gameScene_ = nullptr;

	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	bool isDead_ = false;
};

