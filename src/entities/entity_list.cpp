/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/

#include "player.h"
#include "killer_bot.h"
#include "menu_camera.h"
#include "killer_drone.h"
#include "victim.h"
#include "saw.h"
#include "stick.h"
#include "checkpoint.h"

std::unordered_map<unsigned int, ENTITY_CREATOR_FUNC((*))> g_EntityCreatorList;

void InitEntityCreatorList()
{
    g_EntityCreatorList.insert({CPlayer::SGetEntityID(), &CPlayer::PlayerCreator});
    g_EntityCreatorList.insert({CKillerBot::SGetEntityID(), &CKillerBot::KillerBotCreator});
    g_EntityCreatorList.insert({CMenuCamera::SGetEntityID(), &CMenuCamera::MenuCameraCreator});
    g_EntityCreatorList.insert({CKillerDrone::SGetEntityID(), &CKillerDrone::KillerDroneCreator});
    g_EntityCreatorList.insert({CVictim::SGetEntityID(), &CVictim::VictimCreator});
    g_EntityCreatorList.insert({CSawBot::SGetEntityID(), &CSawBot::SawBotCreator});
    g_EntityCreatorList.insert({CShootBot::SGetEntityID(), &CShootBot::ShootBotCreator});
    g_EntityCreatorList.insert({CSaw::SGetEntityID(), &CSaw::SawCreator});
    g_EntityCreatorList.insert({CStick::SGetEntityID(), &CStick::StickCreator});
    g_EntityCreatorList.insert({CCheckpoint::SGetEntityID(), &CCheckpoint::CheckpointCreator});
}
