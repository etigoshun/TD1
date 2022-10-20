#include "Player.h"
#include "MyMath.h"

/// <summary>
/// ������
/// </summary>
/// <param name="model">���f��</param>
/// <param name="texturehandle">�e�N�X�`���n���h��</param>
void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	//worldTransform_.translation_.z = 50;
	worldTransform_.Initialize();

	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.translation_ = { 0, 0, 0 };
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update()
{
	////�f�X�t���O�̗������e���폜
	//bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
	//	{
	//		return bullet->IsDead();
	//	});

	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	////�ړ��x�N�g����ύX���鏈��
	//if (input_->PushKey(DIK_RIGHT))
	//{
	//	move.x = 0.1f;
	//}
	//else if (input_->PushKey(DIK_LEFT))
	//{
	//	move.x = -0.1f;
	//}

	//if (input_->PushKey(DIK_UP))
	//{
	//	move.y = 0.1f;
	//}
	//else if (input_->PushKey(DIK_DOWN))
	//{
	//	move.y = -0.1f;
	//}

	////���W�ړ�
	//worldTransform_.translation_ += move;

	//�v���C���[�ړ�����
	switch (mode)
	{
		case 0:
			if (input_->TriggerKey(DIK_A)) {
				if (num == 0) {
					worldTransform_.translation_ = b;
					count = 0;
				}
				else if (num == 1) {
					worldTransform_.translation_ = c;
					count = 0;
				}
				else if (num == 2) {
					worldTransform_.translation_ = d;
					count = 0;
				}
				else if (num == 3) {
					worldTransform_.translation_ = a;
					count = 0;
				}
				num += 1;
				if (num >= 4) {
					num = 0;
				}
			}
			break;
		case 1:
			if (input_->TriggerKey(DIK_A)) {
				if (num == 0) {
					worldTransform_.translation_ = d;
					count = 0;
				}
				else if (num == 1) {
					worldTransform_.translation_ = a;
					count = 0;
				}
				else if (num == 2) {
					worldTransform_.translation_ = b;
					count = 0;
				}
				else if (num == 3) {
					worldTransform_.translation_ = c;
					count = 0;
				}
				num -= 1;
				if (num < 0) {
					num = 3;
				}
			}
			break;
	}
	if (input_->PushKey(DIK_A)) {
		count++;
		if (count >= 100) {
			if (mode == 0) {
				mode += 1;
				count = 0;
			}
			else if (mode == 1) {
				mode -= 1;
				count = 0;
			}
		}

	}

	

	//�ړ����E����
	const float kMoveLimitX = 33.0f;
	const float kMoveLimitY = 17.0f;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//�s��̍X�V
	worldTransform_.matWorld_ = CreateIdentity();
	worldTransform_.matWorld_ *= CreateScale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= CreateRotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateRotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateRotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateTrans(worldTransform_.translation_);

	//�e�q�֌W
	//worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	worldTransform_.TransferMatrix();

	////�L�����N�^�[�̍��W����ʂɕ\������
	//debugText_->SetPos(50, 150);
	//debugText_->Printf("translation:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	//���L�����̉�]����
	Vector3 turn = { 0,0.01f,0 };

	if (input_->PushKey(DIK_J))
	{
		worldTransform_.rotation_ -= turn;
	}
	else if (input_->PushKey(DIK_U))
	{
		worldTransform_.rotation_ += turn;
	}

	/*debugText_->SetPos(50, 125);
	debugText_->Printf("rotation:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);*/

	////�L�����N�^�[�U������
	//Attack();

	////�e�X�V
	//for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	//{
	//	bullet->Update();
	//}
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw(ViewProjection& viewProjection)
{
	//3D���f����`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	////�e��`��
	//for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	//{
	//	bullet->Draw(viewProjection);
	//}
}

/// <summary>
/// �U��
/// </summary>
void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		//�e�̑��x
		const float kBulletSpeed = 0.4f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = Transform(velocity, worldTransform_);

		Vector3 position = GetWorldPosition();

		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, position, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

//���[���h���W���擾
Vector3 Player::GetWorldPosition()
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
void Player::OnCollision()
{
	//�������Ȃ�
}
