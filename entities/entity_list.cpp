
#include "player.h"

std::unordered_map<unsigned int, ENTITY_CREATOR_FUNC((*))> g_EntityCreatorList;

void InitEntityCreatorList()
{
    g_EntityCreatorList.insert({CPlayer::SGetEntityID(), &CPlayer::PlayerCreator});
}
