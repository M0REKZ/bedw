// Copyright (c) Benjamín Gajardo (also known as +KZ)

#include "player.h"
#include "killer_bot.h"
#include "menu_camera.h"
#include "killer_drone.h"

std::unordered_map<unsigned int, ENTITY_CREATOR_FUNC((*))> g_EntityCreatorList;

void InitEntityCreatorList()
{
    g_EntityCreatorList.insert({CPlayer::SGetEntityID(), &CPlayer::PlayerCreator});
    g_EntityCreatorList.insert({CKillerBot::SGetEntityID(), &CKillerBot::KillerBotCreator});
    g_EntityCreatorList.insert({CMenuCamera::SGetEntityID(), &CMenuCamera::MenuCameraCreator});
    g_EntityCreatorList.insert({CKillerDrone::SGetEntityID(), &CKillerDrone::KillerDroneCreator});
}
