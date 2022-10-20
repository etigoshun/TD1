#include "Enemy.h"
#include "Player.h"
#include "MyMath.h"
#include "GameScene.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle, const Vector3& position, const Vector3& velocity)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	//�ڋ߃t�F�[�Y������
	ApproachInitialize();
}

void Enemy::Update()
{
	////�f�X�t���O�̗������e���폜
	//bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
	//	{
	//		return bullet->IsDead();
	//	});

	//�G�̍s��
	switch (phase_)
	{
		case Phase::Approach:
		default:
			//�ړ�(�x�N�g�������Z)
			worldTransform_.translation_ += velocity_;

			////���˃^�C�}�[���f�N�������g
			//kFireTimerCount--;

			////�w�莞�Ԃɓ��B����
			//if (kFireTimerCount < 0)
			//{
			//	//�e�𔭎�
			//	Fire();

			//	//���˃^�C�}�[��������
			//	kFireTimerCount = kFireInterval;
			//}

			////�K��̈ʒu�ɓ��B�����痣�E
			//if (worldTransform_.translation_.z < 0.0f)
			//{
			//	phase_ = Phase::Leave;
			//}
			break;

		//case Phase::Leave:
		//	//�ړ�(�x�N�g�������Z)
		//	worldTransform_.translation_ += -velocity_;

		//	//���˃^�C�}�[���f�N�������g
		//	kFireTimerCount--;

		//	//�w�莞�Ԃɓ��B����
		//	if (kFireTimerCount < 0)
		//	{
		//		//�e�𔭎�
		//		Fire();

		//		//���˃^�C�}�[��������
		//		kFireTimerCount = kFireInterval;
		//	}

		//	//�K��̈ʒu�ɓ��B�����痣�E
		//	if (worldTransform_.translation_.z > 100.0f)
		//	{
		//		phase_ = Phase::Approach;
		//	}
		//	break;
	}

	//�s��̍X�V
	worldTransform_.matWorld_ = CreateIdentity();
	worldTransform_.matWorld_ *= CreateScale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= CreateRotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateRotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateRotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateTrans(worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	/*debugText_->SetPos(50, 150);
	debugText_->Printf("rotation:(%d)", kFireTimerCount);*/

	//�L�����N�^�[�̍��W����ʂɕ\������
	debugText_->SetPos(50, 150);
	debugText_->Printf("translation:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);


	////�e�X�V
	//for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	//{
	//	bullet->Update();
	//}
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	////�e��`��
	//for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	//{
	//	bullet->Draw(viewProjection);
	//}
}

void Enemy::Fire()
{
	assert(player_);

	//�e�̑��x
	const float kBulletSpeed = -0.5f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//���L�����̃��[���h���W���擾����
	Vector3 goal(0, 0, 0);
	 goal = player_->GetWorldPosition();
	//�G�L�����̃��[���h���W���擾����
	 Vector3 start(0, 0, 0);
	start = this->GetWorldPosition();
	//�G�L���������L�����̍����x�N�g�������߂�
	Vector3 difVec(0, 0, 0);
	difVec = goal;
	difVec -= start;
	//�x�N�g���̐��K��
	Normalize(difVec);
	//�x�N�g���̒������A�����ɍ��킹��
	velocity = difVec;

	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//�e��o�^����
	gameScene_->AddEnemyBullet(std::move(newBullet));
}

void Enemy::ApproachInitialize()
{
	//���˃^�C�}�[��������
	kFireTimerCount = kFireInterval;
}

//���[���h���W���擾
Vector3 Enemy::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
void Enemy::OnCollision()
{
	isDead_ = true;
}

Vector3 Enemy::GetTransPosition()
{
	//���[���h���W������ϐ�
	Vector3 transPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	transPos.x = worldTransform_.translation_.x;
	transPos.y = worldTransform_.translation_.y;
	transPos.z = worldTransform_.translation_.z;

	return transPos;
}
