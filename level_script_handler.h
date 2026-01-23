// Copyright (c) Benjamín Gajardo (also known as +KZ)
#ifndef LEVEL_SCRIPT_HANDLER_H
#define LEVEL_SCRIPT_HANDLER_H

#include <entities/entity_base.h>

class CLevelScriptHandler
{
    IEntity ** m_pTargets = nullptr;
    unsigned int m_NumTargets;

    public:

    void PostLoad();
    void PreUpdate();
    void PostUpdate();
    void Unload();

    void OnExitTouch(IEntity * pEnt);
};

extern CLevelScriptHandler g_LevelScriptHandler;

#endif
