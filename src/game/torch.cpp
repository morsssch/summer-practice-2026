#include "torch.h"
#include "room.h"
#include "tilemap.h"

constexpr float TORCH_FRAME_TIME = 0.08f;

void initTorches(TorchField& f, const Room& room) {
    for (int row = 0; row < room.h && f.count < MAX_TORCH; row++) {
        for (int col = 0; col < room.w && f.count < MAX_TORCH; col++) {
            uint8_t t = room.tiles[row][col];
            if (t != T_TORCH) continue;

            int hash = (col * 2347 + row * 1031 + col * row * 7) % 12;

            TorchSprite& torch = f.sprites[f.count++];
            torch.active = true;
            torch.x = col * TILE_SIZE + 4;
            torch.y = row * TILE_SIZE - 8;
            torch.animFrame = hash;
        }
    }
}

void updateTorches(TorchField& f, float dt) {
    f.time += dt;
    for (int i = 0; i < f.count; i++) {
        TorchSprite& torch = f.sprites[i];
        torch.animTimer += dt;
        if (torch.animTimer >= TORCH_FRAME_TIME) {
            torch.animTimer = 0;
            torch.animFrame = (torch.animFrame + 1) % 12;
        }
    }
}