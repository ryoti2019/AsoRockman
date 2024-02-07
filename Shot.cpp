#include <DxLib.h>
#include "SceneManager.h"
#include "Blast.h"
#include "Shot.h"

Shot::Shot(int img)
{

    img_ = img;
    blast_ = new Blast();
    blast_->Init(Blast::COLOR::RED);

}

Shot::~Shot(void)
{
}

void Shot::CreateShot(Vector2 pos, Vector2F dir)
{


    // �e�̔��ˈʒu��ݒ�
    pos_ = pos.ToVector2F();

    // �e�̔��˕����̐ݒ�
    dir_ = dir;

    // �e�̐������Ԃ̐ݒ�
    stepAlive_ = SHOT_ALIVE;

    // �e�̑��x
    speed_ = 5.0f;

}

void Shot::Update(void)
{

    // �����̍X�V
    blast_->Update();

    // �e�̐������Ԃ����炷
    stepAlive_ -= 1.0f / SceneManager::DEFAULT_FPS;
    if (stepAlive_ < 0.0f)
    {
        stepAlive_ = 0.0f;
        // �֐��̏������f
        return;
    }

    // �e���ړ�������
    pos_.x += dir_.x * speed_;
    pos_.y += dir_.y * speed_;

}

void Shot::Draw(void)
{

    // �����̕`��
    blast_->Draw();

    if (!IsAlive())
    {
        // �������Ă��Ȃ���Ώ������f
        return;
    }

    // �e��`��
    Vector2 pos = pos_.ToVector2();

    DrawRotaGraph(pos.x, pos.y, 1.0, 0.0, img_, true);


}

void Shot::Release(void)
{
    // �����̊J��
    blast_->Release();
    delete blast_;
}

bool Shot::IsAlive(void)
{
    return stepAlive_ > 0.0f;
}

void Shot::SetAlive(float time)
{
    stepAlive_ = time;
}

Vector2F Shot::GetPos(void)
{
   return pos_;
}

void Shot::ShotBlast(void)
{
    stepAlive_ = 0.0f;
    blast_->CreateEffect(pos_);
}
