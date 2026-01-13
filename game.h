// Copyright (c) Benjamín Gajardo (also known as +KZ)

#ifndef GAME_H
#define GAME_H

#include <entities/entity_base.h>
#include <vector>

#define MAX_TEXTURES 10

class CSector
{
    public:
    float m_Ceiling = 0.f;
    float m_Floor = 0.f;
    unsigned int m_CeilingTextureID = 0;
    unsigned int m_FloorTextureID = 0;

    Vector2 * m_pVertices = nullptr;
    CSector ** m_pNeighbors = nullptr;
    unsigned int * m_pTexturesIDs = nullptr;
    int m_NumVertices = 0;
    bool m_Active = false; // for ValidateActiveSector()
};

class CGame
{
    CSector * m_pSectors = nullptr;
    int m_NumSectors = 0;
    IEntity ** m_pEntities = nullptr;
    int m_NumEntities = 0;

    CSector * m_pCurrentSector = nullptr;

    Texture2D m_Textures[MAX_TEXTURES];

    void ValidateActiveSectors(CSector * pCurrentSector, int recursionamount = 15);
    void RenderSectors();

    //editor
    void RenderSectors2D(CSector * pSelectedSector);
    void RenderEditorInfo();
    void RenderGrid();
    void UpdateEditorCamera();
    void UpdateEditorInput();

    bool InitTextures();

    public:

    //editor
    bool m_EditorMode = false;
    enum EEditorState
    {
        EDITORSTATE_NONE,
        EDITORSTATE_CREATING_SECTOR,
        EDITORSTATE_EDITING_SECTOR,
        EDITORSTATE_SETTING_NEIGHBOR,
    } m_EditorState = EDITORSTATE_NONE;
    std::vector<Vector2> m_EditorVertices;
    enum EEditorSettingNeighborState
    {
        EDITORSETTINGNEIGHBOR_FIRST_VERT,
        EDITORSETTINGNEIGHBOR_NEIGH_SEC,
        EDITORSETTINGNEIGHBOR_SECOND_VERT,
    } m_EditorSettingNeighborState = EDITORSETTINGNEIGHBOR_FIRST_VERT;
    int m_EditorSelectedVert = 0;
    int m_EditorFirstVert = 0;
    int m_EditorNeighSec = 0;

    bool Init();
    void Destroy();
    void Update();
    void Render();

    CSector * GetSector(int i) { return (i < 0 || i >= m_NumSectors) ? nullptr : &m_pSectors[i]; }
    CSector * GetCurrentSector() { return m_pCurrentSector; }
    void SetCurrentSector(CSector * pSec) { m_pCurrentSector = pSec; }
    int NumSectors() { return m_NumSectors; }

    unsigned long long SectorPointerToID(CSector *pSector);

    friend class CLevelHandler;
};

extern CGame g_Game;

#endif
