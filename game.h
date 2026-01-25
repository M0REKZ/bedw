/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>
#include <unordered_map>

class IEntity;

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

    //these should be only read if m_NumVertices == 3, for slopes
    bool m_IsFloorSlope = false;
    int m_FloorSlopeVert = 0;
    float m_FloorSlopeAltitude = 0.f;
    bool m_IsCeilingSlope = false;
    int m_CeilingSlopeVert = 0;
    float m_CeilingSlopeAltitude = 0.f;
};

class CGame
{
    CSector * m_pSectors = nullptr;
    int m_NumSectors = 0;
    IEntity ** m_pEntities = nullptr;
    int m_NumEntities = 0;

    CSector * m_pCurrentSector = nullptr;

    void ValidateActiveSectors(CSector * pCurrentSector, int recursionamount = 15);
    void RenderSectors();

    //editor
    void RenderSectors2D(CSector * pSelectedSector);
    void RenderEntities2D();
    void RenderEditorInfo();
    void RenderGrid();
    void UpdateEditorCamera();
    void UpdateEditorInput();

    bool InitAssets();

    public:

    void SetNeededTexture(unsigned int id);
    void SetNeededSound(unsigned int id);

    std::vector<unsigned int> m_NeededTextures;
    std::unordered_map<unsigned int, Texture2D> m_Textures;
    std::vector<unsigned int> m_NeededSounds;
    std::unordered_map<unsigned int, Sound> m_Sounds;
    Music m_Music;
    bool m_HasMusic = false;
    unsigned int m_MusicID = 0;

    //editor
    bool m_EditorMode = false;
    enum EEditorState
    {
        EDITORSTATE_NONE,
        EDITORSTATE_CREATING_SECTOR,
        EDITORSTATE_EDITING_SECTOR,
        EDITORSTATE_SETTING_NEIGHBOR,
        EDITORSTATE_MOVING_FLOOR,
        EDITORSTATE_MOVING_CEILING,
        EDITORSTATE_MOVING_SLOPE_CEILING,
        EDITORSTATE_MOVING_SLOPE_FLOOR,
        EDITORSTATE_PLACING_ENTITY,
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
    unsigned int m_EditorSelectedEntity = 0;

    float m_DeathLineY = -20.f;

    bool Init();
    void Destroy();
    void Update();
    void Render();

    CSector * GetSector(int i) { return (i < 0 || i >= m_NumSectors) ? nullptr : &m_pSectors[i]; }
    CSector * GetCurrentSector() { return m_pCurrentSector; }
    void SetCurrentSector(CSector * pSec) { m_pCurrentSector = pSec; }
    int NumSectors() { return m_NumSectors; }

    IEntity * GetEntity(int i) { return (i < 0 || i >= m_NumEntities) ? nullptr : m_pEntities[i]; }
    int NumEntities() { return m_NumEntities; }

    unsigned long long SectorPointerToID(CSector *pSector);

    friend class CLevelHandler;

    bool m_Exit = false;
};

extern CGame g_Game;

#endif
