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


    // ’e‚Ì”­ËˆÊ’u‚ğİ’è
    pos_ = pos.ToVector2F();

    // ’e‚Ì”­Ë•ûŒü‚Ìİ’è
    dir_ = dir;

    // ’e‚Ì¶‘¶ŠÔ‚Ìİ’è
    stepAlive_ = SHOT_ALIVE;

    // ’e‚Ì‘¬“x
    speed_ = 5.0f;

}

void Shot::Update(void)
{

    // ”š”­‚ÌXV
    blast_->Update();

    // ’e‚Ì¶‘¶ŠÔ‚ğŒ¸‚ç‚·
    stepAlive_ -= 1.0f / SceneManager::DEFAULT_FPS;
    if (stepAlive_ < 0.0f)
    {
        stepAlive_ = 0.0f;
        // ŠÖ”‚Ìˆ—’†’f
        return;
    }

    // ’e‚ğˆÚ“®‚³‚¹‚é
    pos_.x += dir_.x * speed_;
    pos_.y += dir_.y * speed_;

}

void Shot::Draw(void)
{

    // ”š”­‚Ì•`‰æ
    blast_->Draw();

    if (!IsAlive())
    {
        // ¶‘¶‚µ‚Ä‚¢‚È‚¯‚ê‚Îˆ—’†’f
        return;
    }

    // ’e‚ğ•`‰æ
    Vector2 pos = pos_.ToVector2();

    DrawRotaGraph(pos.x, pos.y, 1.0, 0.0, img_, true);


}

void Shot::Release(void)
{
    // ”š”­‚ÌŠJ•ú
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
