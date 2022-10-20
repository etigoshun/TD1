#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include <cassert>
#include "Affine.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>


/// <summary>
/// ���L����
/// </summary>
class Player
{
	//�����o�֐�
public:
		
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="texturehandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//�e�q�֌W�����ԃZ�b�^�[
	void SetParent(WorldTransform* worldTransform)
	{
		worldTransform_.parent_ = worldTransform;
	}

public:
	//���a
	const float radius = 1.0f;

	//�����o�ϐ�
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	
	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���͏������邽��
	Input* input_ = nullptr;

	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	int num = 0;

	int mode = 0;

	int count = 0;

	Vector3 a = { 20,10,50 };
	Vector3 b = { 20,-10,50 };
	Vector3 c = { -20,-10,50 };
	Vector3 d = { -20,10,50 };
};

