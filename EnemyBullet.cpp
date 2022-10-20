#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("enemybullet.png");

	//���[���h�g�����X�t�H�[����������
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_.translation_ += velocity_;

	//�s��̍X�V
	worldTransform_.matWorld_ = CreateIdentity();
	worldTransform_.matWorld_ *= CreateScale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= CreateRotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateRotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateRotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateTrans(worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
void EnemyBullet::OnCollision()
{
	//�f�X�t���O�𗧂Ă�
	isDead_ = true;
}

//���[���h���W���擾
Vector3 EnemyBullet::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}