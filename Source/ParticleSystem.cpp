#include"ParticleSystem.h"
#include"ParticleManager.h"
#include"Calculation.h"
#include"InstancingSpriteManager.h"
#include"EffectTexAll.h"
#include"Player.h"

//void ParticleSystem::SlashingEffect(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position) {
//    ParticleManager& particleManager = ParticleManager::Instance();
//    //for (int i = 0; i < 150; i++) {
//    //    Particle* particle = new Particle;
//    //    particle->Launch(direction,position,ParticleType::Slashing,i*0.002+1);
//    //    particleManager.Register(particle);
//    //}
//    for (int i = 0; i < 7; i++) {
//        float angleX = DirectX::XMConvertToRadians(rand()%5 - 2.5);
//        float angleY =  DirectX::XMConvertToRadians(rand() % 5 - 2.5);
//        DirectX::XMFLOAT3 Vec;
//        Vec.y = sin(NULL) - sin(angleY);
//        Vec.x = cosf(NULL) *sinf(NULL) - cosf(angleY)* sinf(angleX);
//        Vec.z = cosf(NULL) *cosf(NULL) - cosf(angleY) * cosf(angleX);
//        Vec = Vector3::add(direction, Vec);
//        float speed = rand() % 7 + 8;
//        Particle* particle = new Particle;
//        particle->Launch(Vec, position, ParticleType::Slashing, 0.3, speed);
//        particleManager.Register(particle);
//    }
//
//}
void ParticleSystem::LanceEffect(DirectX::XMFLOAT3 position) {
    ParticleManager& particleManager = ParticleManager::Instance();
    Particle::MoveConstants move;
    std::vector<Particle::MoveConstants> moveConst;
    for (int i = 0; i < 50; i++) {
        float angle = DirectX::XMConvertToRadians(rand() % 360);
        float length = rand() % 5 ;
        DirectX::XMFLOAT3 Vec,pos;
        Vec.x = sinf(angle);
        Vec.z = cosf(angle);
        pos.x = position.x + Vec.x * length;
        pos.y = position.y - 4;
        pos.z = position.z + Vec.z * length;
        //上向きランダムで
        float angleX = DirectX::XMConvertToRadians(rand()%360);
        float angleY =  DirectX::XMConvertToRadians( 90 + (rand() % 90-45)  );
        Vec.y = sin(angleY);
        Vec.x = cosf(angleY)* sinf(angleX);
        Vec.z = cosf(angleY) * cosf(angleX);
        //Vec = Vector3::add({0,1,0}, Vec);
        float speed = 10;
        move.position = pos;
        move.direction = Vec;
        move.speed = speed;
        moveConst.push_back(move);
    }
    Particle* particle = new Particle;
    particle->Launch(moveConst, ParticleType::Lance, Particle::Lance, static_cast<int>(ParticleShader::ModelPSType::Toon), static_cast<int>(EffectTexAll::EfTexAll::Distortion), 1.5, { 1,0,0,1 });
    particleManager.Register(particle);
}


void ParticleSystem::BarrierEffect(const DirectX::XMFLOAT3& position, float size, int textype, DirectX::XMFLOAT4 color) {
    ParticleManager& particleManager = ParticleManager::Instance();
    Particle::MoveConstants move;
    std::vector<Particle::MoveConstants> moveConst;
    float angle = DirectX::XMConvertToRadians(rand() % 360);
    DirectX::XMFLOAT3 Vec;
    Vec.y = 0;
    Vec.x = sinf(angle);
    Vec.z = cosf(angle);
    move.position = position;
    move.direction = Vec;
    move.directionUp = { NULL,1,NULL };
    move.speed = size;
    moveConst.push_back(move);
    Particle* particle = new Particle;
    particle->Launch(moveConst, ParticleType::Barrier, Particle::Ball, static_cast<int>(ParticleShader::ModelPSType::Toon), textype, 1.0, color);
    particleManager.Register(particle);

}

void ParticleSystem::BarrierEnemyEffect(float enemyID, int textype, DirectX::XMFLOAT4 color) {
    ParticleManager& particleManager = ParticleManager::Instance();
    Particle::MoveConstants move;
    std::vector<Particle::MoveConstants> moveConst;
    float angle = DirectX::XMConvertToRadians(rand() % 360);
    DirectX::XMFLOAT3 Vec;
    Vec.y = 0;
    Vec.x = sinf(angle);
    Vec.z = cosf(angle);
    move.position = {};
    move.direction = Vec;
    move.directionUp = { NULL,1,NULL };
    move.speed = enemyID;
    moveConst.push_back(move);
    Particle* particle = new Particle;
    particle->Launch(moveConst, ParticleType::BarrierEnemy, Particle::Ball, static_cast<int>(ParticleShader::ModelPSType::Toon), textype, 1.0, color);
    particleManager.Register(particle);

}

void ParticleSystem::RubbleEffect(DirectX::XMFLOAT3 position, int Max, int SpeedMax) {
    ParticleManager& particleManager = ParticleManager::Instance();
    Particle::MoveConstants move;
    std::vector<Particle::MoveConstants> moveConst;
    for (int i = 0; i < Max; i++) {
        float angle = DirectX::XMConvertToRadians(rand() % 360);
        float length = rand() % 5;
        DirectX::XMFLOAT3 Vec, pos;
        Vec.x = sinf(angle);
        Vec.z = cosf(angle);
        pos.x = position.x ;
        pos.y = position.y ;
        pos.z = position.z ;
        //上向きランダムで
        float angleX = DirectX::XMConvertToRadians(rand() % 360);
        float angleY = DirectX::XMConvertToRadians(90 + (rand() % 90 - 45));
        Vec.y = sin(angleY);
        Vec.x = cosf(angleY) * sinf(angleX);
        Vec.z = cosf(angleY) * cosf(angleX);
        //Vec = Vector3::add({0,1,0}, Vec);
        float speed = rand() % SpeedMax + 10;
        move.position = pos;
        move.direction = Vec;
        move.speed = speed;
        moveConst.push_back(move);
        
    }
    Particle* particle = new Particle;
    //ランダムの岩モデル
    particle->Launch(moveConst, ParticleType::Rubble, rockType[rand() % Particle::Rock3 + Particle::Lance], static_cast<int>(ParticleShader::ModelPSType::Default),NULL ,1.5,{ 1,0,0,1 });
    particleManager.Register(particle);
}

void ParticleSystem::Rubble2Effect(DirectX::XMFLOAT3 position, int Max, int SpeedMax) {
    ParticleManager& particleManager = ParticleManager::Instance();
    Particle::MoveConstants move;
    std::vector<Particle::MoveConstants> moveConst;
    for (int i = 0; i < Max; i++) {
        float angle = DirectX::XMConvertToRadians(rand() % 360);
        float length = rand() % 5;
        DirectX::XMFLOAT3 Vec, pos;
        Vec.x = sinf(angle);
        Vec.z = cosf(angle);
        pos.x = position.x;
        pos.y = position.y - 6;
        pos.z = position.z;
        //上向きランダムで
        float angleX = DirectX::XMConvertToRadians(90) + Player::Instance().GetAngle().y;
        float angleY = 0;
        if (i % 2) {
            angleY = DirectX::XMConvertToRadians(90 - 35);
        }
        else
        {
            angleY = DirectX::XMConvertToRadians(90 + 35);
        }
        Vec.y = sin(angleY);
        Vec.x = cosf(angleY) * sinf(angleX);
        Vec.z = cosf(angleY) * cosf(angleX);
        //Vec = Vector3::add({0,1,0}, Vec);
        float speed = rand() % SpeedMax + 100;
        move.position = pos;
        move.direction = Vec;
        move.speed = speed;
        moveConst.push_back(move);

    }
    Particle* particle = new Particle;
    //ランダムの岩モデル
    particle->Launch(moveConst, ParticleType::Rubble2, rockType[rand() % Particle::Rock3], static_cast<int>(ParticleShader::ModelPSType::Default), NULL, 1.5, { 1,0,0,1 });
    particleManager.Register(particle);
}

void ParticleSystem::Rubble3Effect(DirectX::XMFLOAT3 position, int Max, int SpeedMax,int RockType) {
    ParticleManager& particleManager = ParticleManager::Instance();
    Particle::MoveConstants move;
    std::vector<Particle::MoveConstants> moveConst;
    for (int i = 0; i < Max; i++) {
        float angle = DirectX::XMConvertToRadians(rand() % 360);
        float length = rand() % 5;
        DirectX::XMFLOAT3 Vec, pos;
        Vec.x = sinf(angle);
        Vec.z = cosf(angle);
        pos.x = position.x;
        pos.y = position.y;
        pos.z = position.z;
        //上向きランダムで
        float angleX = DirectX::XMConvertToRadians(rand() % 360);
        float angleY = DirectX::XMConvertToRadians(60);
        Vec.y = sin(angleY);
        Vec.x = cosf(angleY) * sinf(angleX);
        Vec.z = cosf(angleY) * cosf(angleX);
        //Vec = Vector3::add({0,1,0}, Vec);
        float speed = rand() % SpeedMax + 10;
        move.position = pos;
        move.direction = Vec;
        move.speed = speed;
        moveConst.push_back(move);

    }
    Particle* particle = new Particle;
    //ランダムの岩モデル
    particle->Launch(moveConst, ParticleType::Rubble2, rockType[RockType], static_cast<int>(ParticleShader::ModelPSType::Default), NULL, 1.5, { 1,0,0,1 });
    particleManager.Register(particle);
}



void ParticleSystem::BoomEffect(DirectX::XMFLOAT3 position, int max, int textype, float size, float liftimer, DirectX::XMFLOAT4 color) {
    ParticleManager& particleManager = ParticleManager::Instance();
    Particle::MoveConstants move;
    std::vector<Particle::MoveConstants> moveConst;
    for (int i = 0; i < max; i++) {
        float pitch = DirectX::XMConvertToRadians(rand() % 360);
        float yaw = DirectX::XMConvertToRadians(rand() % 180);
        DirectX::XMFLOAT3 Vec;
        Vec.x = cosf(pitch) * sinf(yaw);
        Vec.y = sin(pitch);
        Vec.z = cosf(pitch) * cosf(yaw);
        move.position =position;
        move.directionUp = {NULL,1,NULL};
        move.speed = size;
        if (i >= 1) {
            move.position.x = position.x + (Vec.x * (rand() % 2 + 1));
            move.position.y = position.y + (Vec.y * (rand() % 2 + 1));
            move.position.z = position.z + (Vec.z * (rand() % 2 + 1));
            //move.speed = 0.01;
        }
        Vec.y = 0;
        move.direction = Vec;
        moveConst.push_back(move);

    }
    Particle* particle = new Particle;
    particle->Launch(moveConst, ParticleType::Boom, Particle::Ball, static_cast<int>(ParticleShader::ModelPSType::Toon),textype, liftimer, color);
    particleManager.Register(particle);
}

void ParticleSystem::ImpactEffect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 dir, int textype, float size, DirectX::XMFLOAT4 color) {
    ParticleManager& particleManager = ParticleManager::Instance();
    Particle::MoveConstants move;
    std::vector<Particle::MoveConstants> moveConst;
    move.position = position;
    move.directionUp = { NULL,1,NULL };
    move.speed = size;
    move.direction = dir;
    moveConst.push_back(move);
    Particle* particle = new Particle;
    particle->Launch(moveConst, ParticleType::Impact, Particle::Ball, static_cast<int>(ParticleShader::ModelPSType::Distortion), textype, 0.2, color);
    particleManager.Register(particle);
}

void ParticleSystem::FlameBreathBoomEffect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 dir, int max, int textype, float size, DirectX::XMFLOAT4 color) {
    ParticleManager& particleManager = ParticleManager::Instance();
    Particle::MoveConstants move;
    std::vector<Particle::MoveConstants> moveConst;
    for (int i = 0; i < max; i++) {
        float angle = DirectX::XMConvertToRadians(rand() % 360);
        DirectX::XMFLOAT3 Vec;
        Vec.y = 0;
        Vec.x = sinf(angle);
        Vec.z = cosf(angle);
        move.position = position;
        move.direction = Vec;
        move.directionUp = { NULL,1,NULL };
        move.speed = size;
        moveConst.push_back(move);

    }
    Particle* particle = new Particle;
    particle->Launch(moveConst, ParticleType::Boom, Particle::Ball, static_cast<int>(ParticleShader::ModelPSType::Toon), textype, 1.0, color);
    particleManager.Register(particle);
}

void ParticleSystem::FlameBreathEffect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 dir) {
    InstancingSpriteManager& particleManager = InstancingSpriteManager::Instance();

    //前方向に何度か掛けて
    float yaw = (rand() % 100 - 50) * 0.01;
    DirectX::XMFLOAT3 Vec;
    Vec.x = dir.z * yaw;
    Vec.y = -0.1;
    Vec.z = dir.x * -yaw;

    Vec = Vector3::Add(dir, Vec);
    float speed = 12.0;
    InstancingSprite* particle = new InstancingSprite;
    particle->Launch(Vec, position, InstancingSprite::FlameBreath, InstancingSprite::FlameBreath, 1.5, speed,0,0, { 1,1,1,1 });
    particleManager.Register(particle);
    

}

void ParticleSystem::SeirlConvergenceEffect(DirectX::XMFLOAT3 position, float angle,float len) {
    InstancingSpriteManager& particleManager = InstancingSpriteManager::Instance();
    
     DirectX::XMFLOAT3 pos;
     DirectX::XMFLOAT3 Vec;
     float angleX = DirectX::XMConvertToRadians(90) + Player::Instance().GetAngle().y;
     float Y = angle;
     float angleY = DirectX::XMConvertToRadians(Y);
     Vec.y = sin(angleY);
     Vec.x = cosf(angleY) * sinf(angleX);
     Vec.z = cosf(angleY) * cosf(angleX);

     pos.x = position.x + Vec.x * len;
     pos.y = position.y + Vec.y * len;
     pos.z = position.z + Vec.z * len;
    
    float speed = 15.0;
    InstancingSprite* particle = new InstancingSprite;
    particle->Launch(position, pos, InstancingSprite::SeirlConvergence, InstancingSprite::SeirlConvergence, 1.5, speed,angleX,Y, { 1,1,1,1 });
    particleManager.Register(particle);


}
void ParticleSystem::VortexDiffusionEffect(DirectX::XMFLOAT3 position, float angle) {
    InstancingSpriteManager& particleManager = InstancingSpriteManager::Instance();

    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT3 Vec;
    float angleX = DirectX::XMConvertToRadians(90) + Player::Instance().GetAngle().y;
    float Y = angle;
    float angleY = DirectX::XMConvertToRadians(Y);

    float speed = 15.0;
    InstancingSprite* particle = new InstancingSprite;
    particle->Launch(position,position, InstancingSprite::VortexDiffusion, InstancingSprite::VortexDiffusion, 0.5, speed, angleX, Y, { 1,1,1,1 });
    particleManager.Register(particle);


}

void ParticleSystem::CorruptionEffect(DirectX::XMFLOAT3 position,int enemyNo){
    InstancingSpriteManager& particleManager = InstancingSpriteManager::Instance();

    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT3 Vec;
    float angleX = DirectX::XMConvertToRadians(rand() % 360);
    float angleY = DirectX::XMConvertToRadians(90 + (rand() % 90 - 22));
   // Vec.y = sin(angleY);
    Vec.x = sinf(angleX);
    Vec.z = cosf(angleX);

    pos.x = position.x + Vec.x * 6;
    pos.y = position.y -0.5;
    pos.z = position.z + Vec.z * 6;

    float speed = 3.5;
    InstancingSprite* particle = new InstancingSprite;
    particle->Launch(Vec, pos, InstancingSprite::Corruption, InstancingSprite::Corruption, 0.3, speed, enemyNo, 0, { 1,1,0,1 });
    particleManager.Register(particle);


}

void ParticleSystem::Corruption2Effect(DirectX::XMFLOAT3 position) {
    InstancingSpriteManager& particleManager = InstancingSpriteManager::Instance();

    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT3 Vec;
    float angleX = DirectX::XMConvertToRadians(rand() % 360);
    float angleY = DirectX::XMConvertToRadians(90 + (rand() % 90 - 22));
    // Vec.y = sin(angleY);
    Vec.x = sinf(angleX);
    Vec.z = cosf(angleX);

    pos.x = position.x + Vec.x * 10;
    pos.y = position.y - 0.5;
    pos.z = position.z + Vec.z * 10;

    float speed = 3.5;
    InstancingSprite* particle = new InstancingSprite;
    particle->Launch({0,1,0}, pos, InstancingSprite::Corruption2, InstancingSprite::Corruption2, 0.00, speed, 0, 0, { 1,1,0,1 });
    particleManager.Register(particle);


}