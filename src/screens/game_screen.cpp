#include "game_screen.h"
#include "render/renderer.h"
#include "render/sprites.h"
#include "game/player.h"

void drawPlayer(Renderer& r, const Player& p)
{
    // атака — спрайт 32px, при flip сдвигаем влево чтобы тело оставалось на месте
    if (p.anim == AnimState::ATTACK) {
        float drawX = p.facingRight ? p.pos.x : p.pos.x - 16;
        drawSprite(r, HERO_ATTACK[p.animFrame], drawX, p.pos.y, !p.facingRight);
        return;
    }

    const SpriteFrame* anim;
    switch (p.anim) {
        case AnimState::RUN:  anim = HERO_RUN;  break;
        case AnimState::JUMP: anim = HERO_JUMP; break;
        case AnimState::FALL: anim = HERO_FALL; break;
        case AnimState::LAND: anim = HERO_LAND; break;
        case AnimState::ROLL: anim = HERO_ROLL; break;
        default:              anim = HERO_IDLE; break;
    }
    drawSprite(r, anim[p.animFrame], p.pos.x, p.pos.y, !p.facingRight);
}
