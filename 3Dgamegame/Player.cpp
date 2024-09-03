#include "Player.h"
#include"Pad.h"
#include"MyLib/Physics/ColliderSphere.h"
#include"Camera.h"

namespace
{
	constexpr int kNetralRadius = 20;//�ʏ펞�̓����蔼�a

	//�A�j���[�V�����ԍ�
	constexpr int kIdleAnimIndex = 1;
	//constexpr int kIdleAnimIndex = 2;//�ҋ@�e�X�g
	constexpr int kAttackAnimIndex = 30;

	constexpr float kAnimFrameSpeed = 30.0f;//�A�j���[�V�����i�s���x

	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//�A�i���O�X�e�B�b�N�ɂ��ړ��֘A
	constexpr float kMaxSpeed = 5.0f;//�v���C���[�̍ő呬�x
	constexpr float kAnalogRangeMin = 0.1f;//�A�i���O�X�e�B�b�N�̓��͔���͈�
	constexpr float kAnalogRangeMax = 0.8f;
	constexpr float kAnalogInputMax = 1000.0f;//�A�i���O�X�e�B�b�N������͂����x�N�g���̍ő�l

	constexpr float kFrameParSecond = 60.0f;

	constexpr int kAvoidFrame = 60;

	constexpr int kJumpPower = 50;
}


Player::Player(int modelhandle) : Collidable(Priority::High,ObjectTag::Player),
	m_modelHandle(MV1DuplicateModel(modelhandle)),
	m_anim_move(),
	m_radius(kNetralRadius),
	m_Hp(50),
	m_playerUpdate(&Player::StartUpdate),
	m_cameraUpdate(&Player::Planet1Update),
	m_regeneRange(0),
	m_angle(0),
	m_spinAngle(0),
	m_animBlendRate(0),
	m_currentAnimNo(0),
	m_prevAnimNo(0)
{
	m_rigid.SetPos(Vec3(0, 0, 0));
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = m_radius;
}

Player::~Player()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Init()
{
	

}

void Player::Update()
{
	m_rigid.SetVelocity(Vec3(0, 0, 0));
	(this->*m_playerUpdate)();

	Pad::Update();
	//(this->*m_cameraUpdate)();
	//m_camera->SetUpVec(GetCameraUpVector());
	//m_camera->Update(m_rigid.GetPos()+Vec3(GetCameraUpVector())*300);

	if (m_visibleCount > 200)
	{
		m_isVisibleFlag = false;
		m_visibleCount = 0;
	}
	if (m_isVisibleFlag)
	{
		m_visibleCount++;
	}
	else
	{
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.front());
		item->radius = m_radius;
	}
}

void Player::SetMatrix()
{
	//�J�����̂���p�x����
	//�R���g���[���[�ɂ��ړ����������肷��
	MATRIX mtx;
	MATRIX scale = MGetScale(VGet(0.1f, 0.1f, 0.1f));
	

	MATRIX moveDir = MGetRotY((m_angle)+DX_PI_F / 2);
	mtx = MMult(scale, moveDir);

	MATRIX moving = MGetTranslate(m_rigid.GetPos().VGet());

	mtx = MMult(mtx, moving);

	MV1SetMatrix(m_modelHandle, mtx);
}

void Player::Draw()
{
	if (m_visibleCount % 5 == 0)
	{
		MV1DrawModel(m_modelHandle);
	}

#if _DEBUG
	DrawSphere3D(m_rigid.GetPos().VGet(), m_radius, 10, 0x000000, 0x00ffff, false);
	
	//printfDx("%d", HitCount);
#endif

}

void Player::SetCameraToPlayer(Vec3 cameraToPlayer)
{
	m_cameraToPlayer = cameraToPlayer;
}

void Player::OnCollideEnter(const Collidable& colider)
{
	if (colider.GetTag() == ObjectTag::Takobo)
	{
		m_Hp -= 10;
	}
}

void Player::SetCameraAngle(float cameraAngle)
{
	m_cameraAngle = cameraAngle;
}

void Player::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::EnemyAttack)
	{
		m_Hp -= 20;
	}
}

Vec3 Player::GetCameraToPlayer() const
{
	return m_cameraToPlayer;
}

bool Player::UpdateAnim(int attachNo)
{
	//�A�j���[�V�������ݒ肳��Ă��Ȃ��̂ŏI��
	if (attachNo == -1) return false;

	//�A�j���[�V������i�s������
	float now = MV1GetAttachAnimTime(m_modelHandle, attachNo);//���݂̍Đ��J�E���g
	now += kAnimFrameSpeed / kFrameParSecond;//�A�j���[�V�����J�E���g��i�߂�

	//���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾����
	float total = MV1GetAttachAnimTotalTime(m_modelHandle, attachNo);
	bool isLoop = false;
	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	MV1SetAttachAnimTime(m_modelHandle, attachNo, now);

	return isLoop;
}

void Player::ChangeAnim(int animIndex)
{
	//����ɌÂ��A�j���[�V�������A�^�b�`����Ă���ꍇ�͂��̎��_�ō폜���Ă���
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_prevAnimNo);
	}

	//���ݍĐ����̑ҋ@�A�j���[�V�����͕ύX�O�̃A�j���[�V����������
	m_prevAnimNo = m_currentAnimNo;

	//�ύX��̃A�j���[�V�����Ƃ��čU���A�j���[�V���������߂Đݒ肷��
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, animIndex, -1, false);

	//�؂�ւ��̏u�Ԃ͕ύX�O�̃A�j���[�V�������Đ�������Ԃɂ���
	m_animBlendRate = 0.0f;

	//�ύX�O�̃A�j���[�V����100%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//�ύX��̃A�j���[�V����0%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);

}

void Player::StartUpdate()
{
	m_regeneRange += 0.01f;
	if (m_regeneRange > 2)
	{
		m_playerUpdate = &Player::NeutralUpdate;
	}
}

void Player::NeutralUpdate()
{
	//�A�i���O�X�e�B�b�N���g���Ĉړ�

	int analogX = 0, analogY = 0;

	GetJoypadAnalogInput(&analogX, &analogY, DX_INPUT_PAD1);
	analogY = -analogY;
	if (analogX * analogY != 0)
	{
		int a = 0;
	}
	//�A�i���O�X�e�B�b�N�̓���10%~80%���g�p����
	//�x�N�g���̒������ő�1000�ɂȂ�
	//�x�N�g���̒������擾
	Vec3 move;

	float len = move.Length();
	//�x�N�g���̒�����0.0~1.0�̊����ɕϊ�����
	float rate = len / kAnalogInputMax;
	Vec3 front=GetCameraFrontVector();
	Vec3 right = GetCameraRightVector();
	move=m_frontVec* analogY;//���͂��傫���قǗ������傫��,0�̎���0
	move+=m_sideVec* analogX;
	

	//�A�i���O�X�e�B�b�N�����Ȕ͈͂����O����
	rate = (rate - kAnalogRangeMin / (kAnalogRangeMax - kAnalogRangeMin));
	rate = std::min(rate, 1.0f);
	rate = std::max(rate, 0.0f);

	//���x������ł���̂ňړ��x�N�g���ɔ��f
	move = move.GetNormalized();
	float speed = kMaxSpeed;

	//m_angle = fmodf(m_cameraAngle, 360);//mod:�]��@
	//MATRIX rotate = MGetRotY((m_angle)-DX_PI_F / 2);//�{���̓J�������s��Ő��䂵�A���̍s���Y����]
	m_moveDir = move;
	move = move * speed;
	//�v���C���[�̍ő�ړ����x��0.01f/frame
	if (Pad::IsTrigger(PAD_INPUT_1))//XBox��A�{�^��
	{
		
	}
	if (Pad::IsTrigger(PAD_INPUT_2))//XBox��
	{
		m_playerUpdate=&Player::SpiningUpdate;
	}
	/*auto v = VTransform(VGet(move.x, 0, move.z), rotate);
	move = Vec3(v);*/
	m_rigid.SetVelocity(move);

	//�v���C���[�̍ő�ړ����x��0.01f/frame
	//if (Pad::IsTrigger(PAD_INPUT_1))//XBox��A�{�^��
	//{
	//	m_radius = 0;
	//	m_playerUpdate = &Player::AvoidUpdate;
	//}
	
}

void Player::WalkingUpdate()
{
}

void Player::JumpingUpdate()
{
}

void Player::SpiningUpdate()
{
	m_spinAngle+=DX_PI_F/60;
	m_angle += DX_PI_F / 60;
	if (m_spinAngle >= DX_PI_F * 2)
	{
		m_playerUpdate = &Player::NeutralUpdate;
		m_spinAngle = 0;
	}
}

void Player::HitUpdate()
{

	//�_���[�W�A�j���[�V�����̂�
	m_playerUpdate = &Player::NeutralUpdate;


}

void Player::AvoidUpdate()
{
	actionFrame++;

	if (actionFrame > kAvoidFrame)
	{
		actionFrame = 0;
		m_radius = kNetralRadius;
		m_playerUpdate = &Player::NeutralUpdate;
	}
}

void Player::Planet1Update()
{

	
}
