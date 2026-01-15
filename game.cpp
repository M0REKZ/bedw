// Copyright (c) Benjamín Gajardo (also known as +KZ)

#include <raylib.h>
#include <rlgl.h>

#include "game.h"
#include "globals.h"
#include <collision.h>
#include <entities/player.h>
#include <cmath>
#include <cstdio>
#include <input_handler.h>
#include <level_handler.h>

CGame g_Game;

void CGame::ValidateActiveSectors(CSector *pCurrentSector, int recursionamount)
{
    pCurrentSector->m_Active = true;

    recursionamount--;
    if(recursionamount <= 0) //limit reached
        return;

    for(int vertid = 0; vertid < pCurrentSector->m_NumVertices; vertid++)
    {
        if(pCurrentSector->m_pNeighbors[vertid] && !pCurrentSector->m_pNeighbors[vertid]->m_Active)
        {
            ValidateActiveSectors(pCurrentSector->m_pNeighbors[vertid], recursionamount);
        }
    }
}

void CGame::RenderSectors()
{
    for(int i = 0; i < m_NumSectors; i++)
    {

        if(!m_pSectors[i].m_Active)
        {
            continue;
        }

        BeginMode3D(g_Globals.m_RaylibCamera);
        rlBegin(RL_TRIANGLES);

        // walls

        float WallCeilingSlope = m_pSectors[i].m_IsCeilingSlope ? m_pSectors[i].m_CeilingSlopeAltitude : m_pSectors[i].m_Ceiling;
        float WallFloorSlope = m_pSectors[i].m_IsFloorSlope ? m_pSectors[i].m_FloorSlopeAltitude : m_pSectors[i].m_Floor;

        for(int vertid = 0; vertid < m_pSectors[i].m_NumVertices; vertid++)
        {
            Vector3 Verts[4];
            if(m_pSectors[i].m_IsCeilingSlope && vertid == m_pSectors[i].m_CeilingSlopeVert)
                Verts[0] = {m_pSectors[i].m_pVertices[vertid].x, WallCeilingSlope, m_pSectors[i].m_pVertices[vertid].y};
            else
                Verts[0] = {m_pSectors[i].m_pVertices[vertid].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[vertid].y};

            if(m_pSectors[i].m_IsCeilingSlope && (vertid + 1) % (m_pSectors[i].m_NumVertices) == m_pSectors[i].m_CeilingSlopeVert)
                Verts[1] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, WallCeilingSlope, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};
            else
                Verts[1] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};

            if(m_pSectors[i].m_IsFloorSlope && (vertid + 1) % (m_pSectors[i].m_NumVertices) == m_pSectors[i].m_FloorSlopeVert)
                Verts[2] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, WallFloorSlope, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};
            else
                Verts[2] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, m_pSectors[i].m_Floor, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};

            if(m_pSectors[i].m_IsFloorSlope && vertid == m_pSectors[i].m_FloorSlopeVert)
                Verts[3] = {m_pSectors[i].m_pVertices[vertid].x, WallFloorSlope, m_pSectors[i].m_pVertices[vertid].y};
            else
                Verts[3] = {m_pSectors[i].m_pVertices[vertid].x, m_pSectors[i].m_Floor, m_pSectors[i].m_pVertices[vertid].y};

            float vertdist = PointDistance(m_pSectors[i].m_pVertices[vertid], m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)]);
            float huv = vertdist/32; // horizontal uv

            float ceildist = (m_pSectors[i].m_Ceiling - m_pSectors[i].m_Floor);
            float vuv = ceildist/32; // vertical uv

            rlSetTexture(m_Textures[m_pSectors[i].m_pTexturesIDs[vertid]].id);
            rlColor4ub(WHITE.r, WHITE.g, WHITE.b, WHITE.a);

            if(m_pSectors[i].m_Floor <= m_pSectors[i].m_Ceiling &&
                m_pSectors[i].m_pNeighbors[vertid] &&
                m_pSectors[i].m_pNeighbors[vertid]->m_Floor <= m_pSectors[i].m_pNeighbors[vertid]->m_Ceiling)
            {
                CSector *pNeigh = m_pSectors[i].m_pNeighbors[vertid];
                Vector3 VertsNeigh[4];

                if(pNeigh->m_IsCeilingSlope && pNeigh->m_NumVertices == 3 &&
                    m_pSectors[i].m_pVertices[vertid].x == pNeigh->m_pVertices[pNeigh->m_CeilingSlopeVert].x &&
                    m_pSectors[i].m_pVertices[vertid].y == pNeigh->m_pVertices[pNeigh->m_CeilingSlopeVert].y)
                    VertsNeigh[0] = {m_pSectors[i].m_pVertices[vertid].x, pNeigh->m_CeilingSlopeAltitude, m_pSectors[i].m_pVertices[vertid].y};
                else
                    VertsNeigh[0] = {m_pSectors[i].m_pVertices[vertid].x, pNeigh->m_Ceiling, m_pSectors[i].m_pVertices[vertid].y};
                
                if(pNeigh->m_IsCeilingSlope && pNeigh->m_NumVertices == 3 &&
                    m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x == pNeigh->m_pVertices[pNeigh->m_CeilingSlopeVert].x &&
                    m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y == pNeigh->m_pVertices[pNeigh->m_CeilingSlopeVert].y)
                    VertsNeigh[1] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, pNeigh->m_CeilingSlopeAltitude, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};
                else
                    VertsNeigh[1] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, pNeigh->m_Ceiling, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};

                if(pNeigh->m_IsFloorSlope && pNeigh->m_NumVertices == 3 &&
                    m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x == pNeigh->m_pVertices[pNeigh->m_FloorSlopeVert].x &&
                    m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y == pNeigh->m_pVertices[pNeigh->m_FloorSlopeVert].y)
                    VertsNeigh[2] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, pNeigh->m_FloorSlopeAltitude, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};
                else
                    VertsNeigh[2] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, pNeigh->m_Floor, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};

                if(pNeigh->m_IsFloorSlope && pNeigh->m_NumVertices == 3 &&
                    m_pSectors[i].m_pVertices[vertid].x == pNeigh->m_pVertices[pNeigh->m_FloorSlopeVert].x &&
                    m_pSectors[i].m_pVertices[vertid].y == pNeigh->m_pVertices[pNeigh->m_FloorSlopeVert].y)
                    VertsNeigh[3] = {m_pSectors[i].m_pVertices[vertid].x, pNeigh->m_FloorSlopeAltitude, m_pSectors[i].m_pVertices[vertid].y};
                else
                    VertsNeigh[3] = {m_pSectors[i].m_pVertices[vertid].x, pNeigh->m_Floor, m_pSectors[i].m_pVertices[vertid].y};

                bool dontceil1 = false, dontceil2 = false;
                if(VertsNeigh[0].y > Verts[0].y)
                {
                    Verts[0].y = VertsNeigh[0].y;
                    dontceil1 = true;
                }

                if(VertsNeigh[1].y > Verts[1].y)
                {
                    Verts[1].y = VertsNeigh[1].y;
                    dontceil2 = true;
                }

                bool dontfloor1 = false, dontfloor2 = false;

                if(VertsNeigh[2].y < Verts[2].y)
                {
                    Verts[2].y = VertsNeigh[2].y;
                    dontfloor1 = true;
                }

                if(VertsNeigh[3].y < Verts[3].y)
                {
                    Verts[3].y = VertsNeigh[3].y;
                    dontfloor2 = true;
                }

                //top
                if(!dontceil1 || !dontceil2)
                {
                    rlTexCoord2f(huv, 0.f);
                    rlVertex3f(Verts[1].x, Verts[1].y, Verts[1].z);
                    rlTexCoord2f(0.f, 0.f);
                    rlVertex3f(Verts[0].x, Verts[0].y, Verts[0].z);
                    rlTexCoord2f(huv, vuv);
                    rlVertex3f(VertsNeigh[1].x, VertsNeigh[1].y, VertsNeigh[1].z);

                    rlTexCoord2f(huv, vuv);
                    rlVertex3f(VertsNeigh[1].x, VertsNeigh[1].y, VertsNeigh[1].z);
                    rlTexCoord2f(0.f, 0.f);
                    rlVertex3f(Verts[0].x, Verts[0].y, Verts[0].z);
                    rlTexCoord2f(0.f, vuv);
                    rlVertex3f(VertsNeigh[0].x, VertsNeigh[0].y, VertsNeigh[0].z);
                }
                

                //bottom
                if(!dontfloor1 || !dontfloor2)
                {                
                    rlTexCoord2f(huv, vuv);
                    rlVertex3f(VertsNeigh[2].x, VertsNeigh[2].y, VertsNeigh[2].z);
                    rlTexCoord2f(0.f, vuv);
                    rlVertex3f(VertsNeigh[3].x, VertsNeigh[3].y, VertsNeigh[3].z);
                    rlTexCoord2f(0.f, 1.f);
                    rlVertex3f(Verts[3].x, Verts[3].y, Verts[3].z);

                    rlTexCoord2f(huv, 1.f);
                    rlVertex3f(Verts[2].x, Verts[2].y, Verts[2].z);
                    rlTexCoord2f(huv, vuv);
                    rlVertex3f(VertsNeigh[2].x, VertsNeigh[2].y, VertsNeigh[2].z);
                    rlTexCoord2f(0.f, 1.f);
                    rlVertex3f(Verts[3].x, Verts[3].y, Verts[3].z);
                }
                

                continue;
            }

            //entire wall
            rlTexCoord2f(huv, 0.f);
            rlVertex3f(Verts[1].x, Verts[1].y, Verts[1].z);
            rlTexCoord2f(0.f, 0.f);
            rlVertex3f(Verts[0].x, Verts[0].y, Verts[0].z);
            rlTexCoord2f(huv, vuv);
            rlVertex3f(Verts[2].x, Verts[2].y, Verts[2].z);

            rlTexCoord2f(huv, vuv);
            rlVertex3f(Verts[2].x, Verts[2].y, Verts[2].z);
            rlTexCoord2f(0.f, 0.f);
            rlVertex3f(Verts[0].x, Verts[0].y, Verts[0].z);
            rlTexCoord2f(0.f, vuv);
            rlVertex3f(Verts[3].x, Verts[3].y, Verts[3].z);


        }

        rlEnd();
        EndMode3D();

        BeginMode3D(g_Globals.m_RaylibCamera);
        rlBegin(RL_TRIANGLES);

        rlSetTexture(m_Textures[m_pSectors[i].m_CeilingTextureID].id);
        rlColor4ub(WHITE.r, WHITE.g, WHITE.b, WHITE.a);

        for(int vertid = 1; vertid < m_pSectors[i].m_NumVertices; vertid++)
        {
            rlTexCoord2f((m_pSectors[i].m_pVertices[0].x)/32, (m_pSectors[i].m_pVertices[0].y)/32);
            if(m_pSectors[i].m_IsCeilingSlope && 0 == m_pSectors[i].m_CeilingSlopeVert)
                rlVertex3f(m_pSectors[i].m_pVertices[0].x, WallCeilingSlope, m_pSectors[i].m_pVertices[0].y);
            else
                rlVertex3f(m_pSectors[i].m_pVertices[0].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[0].y);
            rlTexCoord2f((m_pSectors[i].m_pVertices[vertid].x)/32, (m_pSectors[i].m_pVertices[vertid].y)/32);
            if(m_pSectors[i].m_IsCeilingSlope && vertid == m_pSectors[i].m_CeilingSlopeVert)
                rlVertex3f(m_pSectors[i].m_pVertices[vertid].x, WallCeilingSlope, m_pSectors[i].m_pVertices[vertid].y);
            else
                rlVertex3f(m_pSectors[i].m_pVertices[vertid].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[vertid].y);
            rlTexCoord2f((m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x)/32, (m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y)/32);
            if(m_pSectors[i].m_IsCeilingSlope && (vertid + 1) % (m_pSectors[i].m_NumVertices) == m_pSectors[i].m_CeilingSlopeVert)
                rlVertex3f(m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, WallCeilingSlope, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y);
            else
                rlVertex3f(m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y);
        }

        rlSetTexture(m_Textures[m_pSectors[i].m_FloorTextureID].id);
        rlColor4ub(WHITE.r, WHITE.g, WHITE.b, WHITE.a);

        for(int vertid = 1; vertid < m_pSectors[i].m_NumVertices; vertid++)
        {
            rlTexCoord2f((m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x)/32, (m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y)/32);
            if(m_pSectors[i].m_IsFloorSlope && (vertid + 1) % (m_pSectors[i].m_NumVertices) == m_pSectors[i].m_FloorSlopeVert)
                rlVertex3f(m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, WallFloorSlope, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y);
            else
                rlVertex3f(m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, m_pSectors[i].m_Floor, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y);
            rlTexCoord2f((m_pSectors[i].m_pVertices[vertid].x)/32, (m_pSectors[i].m_pVertices[vertid].y)/32);
            if(m_pSectors[i].m_IsFloorSlope && vertid == m_pSectors[i].m_FloorSlopeVert)
                rlVertex3f(m_pSectors[i].m_pVertices[vertid].x, WallFloorSlope, m_pSectors[i].m_pVertices[vertid].y);
            else
                rlVertex3f(m_pSectors[i].m_pVertices[vertid].x, m_pSectors[i].m_Floor, m_pSectors[i].m_pVertices[vertid].y);
            rlTexCoord2f((m_pSectors[i].m_pVertices[0].x)/32, (m_pSectors[i].m_pVertices[0].y)/32);
            if(m_pSectors[i].m_IsFloorSlope && 0 == m_pSectors[i].m_FloorSlopeVert)
                rlVertex3f(m_pSectors[i].m_pVertices[0].x, WallFloorSlope, m_pSectors[i].m_pVertices[0].y);
            else
                rlVertex3f(m_pSectors[i].m_pVertices[0].x, m_pSectors[i].m_Floor, m_pSectors[i].m_pVertices[0].y);
        }

        rlEnd();
        EndMode3D();
    }
}

void CGame::RenderSectors2D(CSector *pSelectedSector)
{
    BeginMode2D(g_Globals.m_RaylibCamera2D);

    for(int i = 0; i < m_NumSectors; i++)
    {
        // walls
        for(int vertid = 0; vertid < m_pSectors[i].m_NumVertices; vertid++)
        {
            if(&m_pSectors[i] == pSelectedSector)
                continue;

            if(m_EditorState == EDITORSTATE_SETTING_NEIGHBOR && (m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_NEIGH_SEC || m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_SECOND_VERT) &&
                &m_pSectors[i] == &m_pSectors[m_EditorNeighSec])
                continue;

            if(m_EditorState == EDITORSTATE_SETTING_NEIGHBOR && m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_NEIGH_SEC && &m_pSectors[m_EditorNeighSec] == &m_pSectors[i])
                DrawLineV(m_pSectors[i].m_pVertices[vertid], m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)], {255,255,0,255});
            else if(m_EditorState == EDITORSTATE_SETTING_NEIGHBOR && vertid == m_EditorSelectedVert && (m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_SECOND_VERT ? &m_pSectors[m_EditorNeighSec] == &m_pSectors[i] : false))
                DrawLineV(m_pSectors[i].m_pVertices[vertid], m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)], {255,255,0,255});
            else if(m_pSectors[i].m_pNeighbors[vertid])
            {
                DrawLineV(m_pSectors[i].m_pVertices[vertid], m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)], {0,0,255,255});
            }
            else
                DrawLineV(m_pSectors[i].m_pVertices[vertid], m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)], {255,0,0,255});

            if(m_pSectors[i].m_IsCeilingSlope)
            {
                DrawCircleV(m_pSectors[i].m_pVertices[m_pSectors[i].m_CeilingSlopeVert],5.f,{255,200,100,255});
            }

            if(m_pSectors[i].m_IsFloorSlope)
            {
                DrawCircleV(m_pSectors[i].m_pVertices[m_pSectors[i].m_FloorSlopeVert],3.f,{200,0,255,255});
            }
        }
    }

    //draw selected sector
    for(int vertid = 0; vertid < pSelectedSector->m_NumVertices; vertid++)
    {
        if(m_EditorState == EDITORSTATE_SETTING_NEIGHBOR && m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_NEIGH_SEC && &m_pSectors[m_EditorNeighSec] == pSelectedSector)
            DrawLineV(pSelectedSector->m_pVertices[vertid], pSelectedSector->m_pVertices[(vertid + 1) % (pSelectedSector->m_NumVertices)], {255,255,0,255});
        else if(m_EditorState == EDITORSTATE_SETTING_NEIGHBOR && vertid == m_EditorSelectedVert && m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_FIRST_VERT)
            DrawLineV(pSelectedSector->m_pVertices[vertid], pSelectedSector->m_pVertices[(vertid + 1) % (pSelectedSector->m_NumVertices)], {255, 255, 0, 255});
        else if(pSelectedSector->m_pNeighbors[vertid])
        {
            DrawLineV(pSelectedSector->m_pVertices[vertid], pSelectedSector->m_pVertices[(vertid + 1) % (pSelectedSector->m_NumVertices)], {0, 255, 255, 255});
        }
        else
            DrawLineV(pSelectedSector->m_pVertices[vertid], pSelectedSector->m_pVertices[(vertid + 1) % (pSelectedSector->m_NumVertices)], {0, 255, 0, 255});

        if(pSelectedSector->m_IsCeilingSlope)
        {
            DrawCircleV(pSelectedSector->m_pVertices[pSelectedSector->m_CeilingSlopeVert], 5.f, {255, 200, 100, 255});
        }

        if(pSelectedSector->m_IsFloorSlope)
        {
            DrawCircleV(pSelectedSector->m_pVertices[pSelectedSector->m_FloorSlopeVert], 3.f, {200, 0, 255, 255});
        }
    }

    //for setting neighbor
    if(m_EditorState == EDITORSTATE_SETTING_NEIGHBOR && (m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_NEIGH_SEC || m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_SECOND_VERT))
    {
        for(int vertid = 0; vertid < m_pSectors[m_EditorNeighSec].m_NumVertices; vertid++)
        {
            if(m_EditorState == EDITORSTATE_SETTING_NEIGHBOR && m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_NEIGH_SEC && &m_pSectors[m_EditorNeighSec] == &m_pSectors[m_EditorNeighSec])
                DrawLineV(m_pSectors[m_EditorNeighSec].m_pVertices[vertid], m_pSectors[m_EditorNeighSec].m_pVertices[(vertid + 1) % (m_pSectors[m_EditorNeighSec].m_NumVertices)], {255,255,0,255});
            else if(m_EditorState == EDITORSTATE_SETTING_NEIGHBOR && vertid == m_EditorSelectedVert && m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_SECOND_VERT)
                DrawLineV(m_pSectors[m_EditorNeighSec].m_pVertices[vertid], m_pSectors[m_EditorNeighSec].m_pVertices[(vertid + 1) % (m_pSectors[m_EditorNeighSec].m_NumVertices)], {255, 255, 0, 255});
            else if(m_pSectors[m_EditorNeighSec].m_pNeighbors[vertid])
            {
                DrawLineV(m_pSectors[m_EditorNeighSec].m_pVertices[vertid], m_pSectors[m_EditorNeighSec].m_pVertices[(vertid + 1) % (m_pSectors[m_EditorNeighSec].m_NumVertices)], {0, 255, 255, 255});
            }
            else
                DrawLineV(m_pSectors[m_EditorNeighSec].m_pVertices[vertid], m_pSectors[m_EditorNeighSec].m_pVertices[(vertid + 1) % (m_pSectors[m_EditorNeighSec].m_NumVertices)], {0, 255, 0, 255});

            if(m_pSectors[m_EditorNeighSec].m_IsCeilingSlope)
            {
                DrawCircleV(m_pSectors[m_EditorNeighSec].m_pVertices[m_pSectors[m_EditorNeighSec].m_CeilingSlopeVert], 5.f, {255, 200, 100, 255});
            }

            if(m_pSectors[m_EditorNeighSec].m_IsFloorSlope)
            {
                DrawCircleV(m_pSectors[m_EditorNeighSec].m_pVertices[m_pSectors[m_EditorNeighSec].m_FloorSlopeVert], 3.f, {200, 0, 255, 255});
            }
        }
    }

    //editor drawing
    if(m_EditorState == EDITORSTATE_CREATING_SECTOR || m_EditorState == EDITORSTATE_EDITING_SECTOR)
    {
        for(int vertid = 0; vertid < m_EditorVertices.size(); vertid++)
        {
            DrawLineV(m_EditorVertices[vertid], m_EditorVertices[(vertid + 1) % (m_EditorVertices.size())], {255,255,255,255});
        }
    }

    EndMode2D();
}

void CGame::RenderEditorInfo()
{
    BeginMode2D(g_Globals.m_RaylibCamera2D);
    unsigned long long sectorid = SectorPointerToID(m_pCurrentSector);
    char tempchar[256] = {0};

    if(m_EditorState == EDITORSTATE_CREATING_SECTOR)
    {
        strncpy(tempchar, "Creating Sector...", sizeof(tempchar));
    }
    else if(m_EditorState == EDITORSTATE_SETTING_NEIGHBOR)
    {
        if(m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_FIRST_VERT)
            snprintf(tempchar, sizeof(tempchar), "Adding Neighbor... \nWall: %d\n", m_EditorSelectedVert);
        else if(m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_NEIGH_SEC)
            snprintf(tempchar, sizeof(tempchar), "Adding Neighbor... \nSector: %d\n", m_EditorNeighSec);
        else if(m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_SECOND_VERT)
            snprintf(tempchar, sizeof(tempchar), "Adding Neighbor... \nOther Wall: %d\n", m_EditorSelectedVert);
    }
    else if(m_EditorState == EDITORSTATE_EDITING_SECTOR)
    {
        snprintf(tempchar, sizeof(tempchar), "Remaking Sector: %llu\n", sectorid);
    }
    else
    {
        snprintf(tempchar, sizeof(tempchar), "Current Sector: %llu\nCeiling: %f\nFloor: %f\n", sectorid, m_pCurrentSector->m_Ceiling, m_pCurrentSector->m_Floor);
        if(m_pCurrentSector->m_IsCeilingSlope)
        {
            char temp2[256] = {0};
            snprintf(temp2, sizeof(temp2), "%sCeiling Slope Vertex: %d, Altitude: %f\n", tempchar, m_pCurrentSector->m_CeilingSlopeVert, m_pCurrentSector->m_CeilingSlopeAltitude);
            strncpy(tempchar, temp2, sizeof(tempchar));
        }
        if(m_pCurrentSector->m_IsFloorSlope)
        {
            char temp2[256] = {0};
            snprintf(temp2, sizeof(temp2), "%sFloor Slope Vertex: %d, Altitude: %f\n", tempchar, m_pCurrentSector->m_FloorSlopeVert, m_pCurrentSector->m_FloorSlopeAltitude);
            strncpy(tempchar, temp2, sizeof(tempchar));
        }
    }

    DrawText(tempchar, 5 + g_Globals.m_RaylibCamera2D.target.x, 5 + g_Globals.m_RaylibCamera2D.target.y, 1, {255,255,255,255});
    EndMode2D();
}

void CGame::RenderGrid()
{
    //vertical lines
    for(int x = ((int)g_Globals.m_RaylibCamera2D.target.x % 10); x < GAME_WIDTH; x+=10)
    {
        DrawLine(x,0,x,GAME_HEIGHT,{128,128,128,255});
    }

    for(int y = ((int)g_Globals.m_RaylibCamera2D.target.y % 10); y < GAME_WIDTH; y+=10)
    {
        DrawLine(0,y,GAME_WIDTH,y,{128,128,128,255});
    }
}

void CGame::UpdateEditorCamera()
{
    const SInput * pInput = g_InputHandler.GetInputs();
    if(pInput->m_Front)
    {
        g_Globals.m_RaylibCamera2D.target.y -=10;
    }

    if(pInput->m_Back)
    {
        g_Globals.m_RaylibCamera2D.target.y +=10;
    }

    if(pInput->m_Left)
    {
        g_Globals.m_RaylibCamera2D.target.x -=10;
    }

    if(pInput->m_Right)
    {
        g_Globals.m_RaylibCamera2D.target.x +=10;
    }
}

void CGame::UpdateEditorInput()
{
    unsigned long long sectorid = SectorPointerToID(m_pCurrentSector);
    const SInput *pInput = g_InputHandler.GetInputs();

    if(m_EditorState == EDITORSTATE_NONE)
    {
        static bool waiting_for_click_release = false;
        if(pInput->m_MouseClick)
        {
            if(!waiting_for_click_release)
            {
                waiting_for_click_release = true;

                float x = pInput->m_MousePos.x + g_Globals.m_RaylibCamera2D.target.x;
                float y = pInput->m_MousePos.y + g_Globals.m_RaylibCamera2D.target.y;

                for(int i = 0; i < m_NumSectors; i++)
                {
                    if(IsPointInsideSector(&m_pSectors[i], {x,y}))
                    {
                        m_pCurrentSector = &m_pSectors[i];
                        break;
                    }
                }
            }
        }
        else
        {
            waiting_for_click_release = false;
        }

        if(pInput->m_Jump)
        {
            m_EditorState = EDITORSTATE_CREATING_SECTOR;
            m_EditorVertices.clear();
        }

        if(pInput->m_EditorNeighborKey)
        {
            m_EditorState = EDITORSTATE_SETTING_NEIGHBOR;
            m_EditorSettingNeighborState = EDITORSETTINGNEIGHBOR_FIRST_VERT;
            m_EditorSelectedVert = 0;
        }

        if(pInput->m_EditorEditSectorKey)
        {
            m_EditorState = EDITORSTATE_EDITING_SECTOR;
            m_EditorVertices.clear();
        }

        if(pInput->m_EditorFloorKey)
        {
            m_EditorState = EDITORSTATE_MOVING_FLOOR;
        }

        if(pInput->m_EditorCeilingKey)
        {
            m_EditorState = EDITORSTATE_MOVING_CEILING;
        }

        if(pInput->m_EditorCeilingSlopeKey && m_pCurrentSector->m_NumVertices == 3)
        {
            m_EditorState = EDITORSTATE_MOVING_SLOPE_CEILING;
            m_pCurrentSector->m_CeilingSlopeVert = 0;
            m_pCurrentSector->m_CeilingSlopeAltitude = m_pCurrentSector->m_Ceiling;
            m_pCurrentSector->m_IsCeilingSlope = true;
        }

        if(pInput->m_EditorFloorSlopeKey && m_pCurrentSector->m_NumVertices == 3)
        {
            m_EditorState = EDITORSTATE_MOVING_SLOPE_FLOOR;
            m_pCurrentSector->m_FloorSlopeVert = 0;
            m_pCurrentSector->m_FloorSlopeAltitude = m_pCurrentSector->m_Floor;
            m_pCurrentSector->m_IsFloorSlope = true;
        }

        static bool waiting_for_saveload_release = false;
        if(pInput->m_EditorSaveLevelKey)
        {
            if(!waiting_for_saveload_release)
            {
                g_LevelHandler.SaveLevel("TESTLEVEL.txt");
            }
        }
        else if(pInput->m_EditorLoadLevelKey)
        {
            if(!waiting_for_saveload_release)
            {
                g_LevelHandler.ReadLevel("TESTLEVEL.txt");
            }
        }
        else
        {
            waiting_for_saveload_release = false;
        }

        static bool waiting_for_arrow_release = false;
        if(pInput->m_ArrowLeft)
        {
            if(!waiting_for_arrow_release && sectorid > 0)
            {
                waiting_for_arrow_release = true;

                sectorid--;
                m_pCurrentSector = &m_pSectors[sectorid];
            }
        }
        else if(pInput->m_ArrowRight)
        {
            if(!waiting_for_arrow_release && sectorid < m_NumSectors - 1)
            {
                waiting_for_arrow_release = true;

                sectorid++;
                m_pCurrentSector = &m_pSectors[sectorid];
            }
        }
        else
        {
            waiting_for_arrow_release = false;
        }
    }
    else if(m_EditorState == EDITORSTATE_CREATING_SECTOR || m_EditorState == EDITORSTATE_EDITING_SECTOR)
    {
        static bool waiting_for_click_release = false;
        if(pInput->m_MouseClick)
        {
            if(!waiting_for_click_release)
            {
                waiting_for_click_release = true;

                int x = pInput->m_MousePos.x + g_Globals.m_RaylibCamera2D.target.x;
                int y = pInput->m_MousePos.y + g_Globals.m_RaylibCamera2D.target.y;

                int modx = x%10;
                int mody = y%10;

                if(modx < 5)
                {
                    x-=modx;
                }
                else
                {
                    x+=(10-modx);
                }

                if(mody < 5)
                {
                    y-=mody;
                }
                else
                {
                    y+=(10-mody);
                }

                m_EditorVertices.push_back({(float)x, (float)y});
            }
        }
        else
        {
            waiting_for_click_release = false;
        }

        if(pInput->m_Enter)
        {
            if(m_EditorState == EDITORSTATE_CREATING_SECTOR)
            {
                //resize array for new sector
                CSector * pTempSec = new CSector[m_NumSectors+1];
                
                //copy data
                for(int i = 0; i < m_NumSectors; i++)
                {
                    pTempSec[i] = m_pSectors[i];
                    // remake neighbor pointers
                    for(int j = 0; j < m_pSectors[i].m_NumVertices; j++)
                    {
                        if(m_pSectors[i].m_pNeighbors[j])
                            pTempSec[i].m_pNeighbors[j] = &pTempSec[SectorPointerToID(m_pSectors[i].m_pNeighbors[j])];
                        else
                            pTempSec[i].m_pNeighbors[j] = nullptr;
                    }
                }
                delete[] m_pSectors; //just delete the old array, but not the data
                m_pSectors = pTempSec;

                //add new sector
                m_NumSectors++;
                int newsecid = m_NumSectors - 1;

                m_pSectors[newsecid].m_NumVertices = m_EditorVertices.size();
                m_pSectors[newsecid].m_pVertices = new Vector2[m_pSectors[newsecid].m_NumVertices];
                m_pSectors[newsecid].m_pNeighbors = new CSector*[m_pSectors[newsecid].m_NumVertices];
                m_pSectors[newsecid].m_pTexturesIDs = new unsigned int[m_pSectors[newsecid].m_NumVertices];
                for(int i = 0; i < m_pSectors[newsecid].m_NumVertices; i++)
                {
                    m_pSectors[newsecid].m_pVertices[i] = m_EditorVertices[i];
                    m_pSectors[newsecid].m_pNeighbors[i] = nullptr;
                    m_pSectors[newsecid].m_pTexturesIDs[i] = 0;
                }

                //fix vertices in wrong order
                if(GetClockwise(m_pSectors[newsecid].m_pVertices, m_pSectors[newsecid].m_NumVertices) > 0.f)
                {
                    int start = 0;
                    int end = m_pSectors[newsecid].m_NumVertices - 1;

                    while(start < end)
                    {
                        Vector2 temp = m_pSectors[newsecid].m_pVertices[start];
                        m_pSectors[newsecid].m_pVertices[start] = m_pSectors[newsecid].m_pVertices[end];
                        m_pSectors[newsecid].m_pVertices[end] = temp;

                        start++;
                        end--;
                    }
                }

                m_pSectors[newsecid].m_Floor = -10.f;
                m_pSectors[newsecid].m_Ceiling = 50.f;

                m_EditorVertices.clear();
                m_EditorState = EDITORSTATE_NONE;

                m_pCurrentSector = &m_pSectors[newsecid];
            }
            else if(m_EditorState == EDITORSTATE_EDITING_SECTOR)
            {
                //need to delete everything
                delete[] m_pCurrentSector->m_pVertices;
                delete[] m_pCurrentSector->m_pNeighbors;
                delete[] m_pCurrentSector->m_pTexturesIDs;
                m_pCurrentSector->m_NumVertices = m_EditorVertices.size();
                m_pCurrentSector->m_pVertices = new Vector2[m_pCurrentSector->m_NumVertices];
                m_pCurrentSector->m_pNeighbors = new CSector*[m_pCurrentSector->m_NumVertices];
                m_pCurrentSector->m_pTexturesIDs = new unsigned int[m_pCurrentSector->m_NumVertices];
                for(int i = 0; i < m_pCurrentSector->m_NumVertices; i++)
                {
                    m_pCurrentSector->m_pVertices[i] = m_EditorVertices[i];
                    m_pCurrentSector->m_pNeighbors[i] = nullptr;
                    m_pCurrentSector->m_pTexturesIDs[i] = 0;
                }

                //fix vertices in wrong order
                if(GetClockwise(m_pCurrentSector->m_pVertices, m_pCurrentSector->m_NumVertices) > 0.f)
                {
                    int start = 0;
                    int end = m_pCurrentSector->m_NumVertices - 1;

                    while(start < end)
                    {
                        Vector2 temp = m_pCurrentSector->m_pVertices[start];
                        m_pCurrentSector->m_pVertices[start] = m_pCurrentSector->m_pVertices[end];
                        m_pCurrentSector->m_pVertices[end] = temp;

                        start++;
                        end--;
                    }
                }

                m_EditorVertices.clear();
                m_EditorState = EDITORSTATE_NONE;
            }
        }
    }
    else if(m_EditorState == EDITORSTATE_SETTING_NEIGHBOR)
    {
        static bool waiting_for_key_release = false;
        if(pInput->m_ArrowLeft)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;

                if(m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_FIRST_VERT)
                {
                    m_EditorSelectedVert--;
                    if(m_EditorSelectedVert < 0)
                        m_EditorSelectedVert = m_pCurrentSector->m_NumVertices - 1;
                }
                else if(m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_SECOND_VERT)
                {
                    m_EditorSelectedVert--;
                    if(m_EditorSelectedVert < 0)
                        m_EditorSelectedVert = m_pSectors[m_EditorNeighSec].m_NumVertices - 1;
                }
                else if(m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_NEIGH_SEC)
                {
                    if(m_EditorNeighSec > 0)
                    {
                        m_EditorNeighSec--;
                    }
                }
            }
        }
        else if(pInput->m_ArrowRight)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;
                
                if(m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_FIRST_VERT)
                {
                    m_EditorSelectedVert++;
                    if(m_EditorSelectedVert >= m_pCurrentSector->m_NumVertices)
                        m_EditorSelectedVert = 0;
                }
                else if(m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_SECOND_VERT)
                {
                    m_EditorSelectedVert++;
                    if(m_EditorSelectedVert >= m_pSectors[m_EditorNeighSec].m_NumVertices)
                        m_EditorSelectedVert = 0;
                }
                else if(m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_NEIGH_SEC)
                {
                    if(m_EditorNeighSec < m_NumSectors - 1)
                    {
                        m_EditorNeighSec++;
                    }
                }

            }
        }
        else if(pInput->m_Enter)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;

                if(m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_FIRST_VERT)
                {
                    m_EditorFirstVert = m_EditorSelectedVert;
                    m_EditorSelectedVert = 0;
                    m_EditorSettingNeighborState = EDITORSETTINGNEIGHBOR_NEIGH_SEC;
                }
                else if(m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_NEIGH_SEC)
                {
                    m_EditorSettingNeighborState = EDITORSETTINGNEIGHBOR_SECOND_VERT;
                }
                else if(m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_SECOND_VERT)
                {
                    m_EditorSettingNeighborState = EDITORSETTINGNEIGHBOR_FIRST_VERT;
                    m_EditorState = EDITORSTATE_NONE;

                    m_pCurrentSector->m_pNeighbors[m_EditorFirstVert] = &m_pSectors[m_EditorNeighSec];
                    m_pSectors[m_EditorNeighSec].m_pNeighbors[m_EditorSelectedVert] = m_pCurrentSector;
                }
            }

        }
        else
        {
            waiting_for_key_release = false;
        }
    }
    else if(m_EditorState == EDITORSTATE_MOVING_CEILING)
    {
        static bool waiting_for_key_release = false;
        if(pInput->m_ArrowUp)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;
                m_pCurrentSector->m_Ceiling++;
            }
        }
        else if(pInput->m_ArrowDown)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;
                m_pCurrentSector->m_Ceiling--;
            }
        }
        else if(pInput->m_Enter)
        {
            m_EditorState = EDITORSTATE_NONE;
        }
        else
        {
            waiting_for_key_release = false;
        }
    }
    else if(m_EditorState == EDITORSTATE_MOVING_FLOOR)
    {
        static bool waiting_for_key_release = false;
        if(pInput->m_ArrowUp)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;
                m_pCurrentSector->m_Floor++;
            }
        }
        else if(pInput->m_ArrowDown)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;
                m_pCurrentSector->m_Floor--;
            }
        }
        else if(pInput->m_Enter)
        {
            m_EditorState = EDITORSTATE_NONE;
        }
        else
        {
            waiting_for_key_release = false;
        }
    }
    else if(m_EditorState == EDITORSTATE_MOVING_SLOPE_CEILING)
    {
        static bool waiting_for_key_release = false;
        if(pInput->m_ArrowUp)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;
                m_pCurrentSector->m_IsCeilingSlope = true;
                m_pCurrentSector->m_CeilingSlopeAltitude++;
            }
        }
        else if(pInput->m_ArrowDown)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;
                m_pCurrentSector->m_IsCeilingSlope = true;
                m_pCurrentSector->m_CeilingSlopeAltitude--;
            }
        }
        if(pInput->m_ArrowLeft)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;
                m_pCurrentSector->m_CeilingSlopeVert--;
                if(m_pCurrentSector->m_CeilingSlopeVert < 0)
                {
                    m_pCurrentSector->m_CeilingSlopeVert = 2;
                }
            }
        }
        else if(pInput->m_ArrowRight)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;
                m_pCurrentSector->m_CeilingSlopeVert++;
                if(m_pCurrentSector->m_CeilingSlopeVert >= 3)
                {
                    m_pCurrentSector->m_CeilingSlopeVert = 0;
                }
            }
        }
        else if(pInput->m_Enter)
        {
            m_EditorState = EDITORSTATE_NONE;
        }
        else
        {
            waiting_for_key_release = false;
        }
    }
    else if(m_EditorState == EDITORSTATE_MOVING_SLOPE_FLOOR)
    {
        static bool waiting_for_key_release = false;
        if(pInput->m_ArrowUp)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;
                m_pCurrentSector->m_IsFloorSlope = true;
                m_pCurrentSector->m_FloorSlopeAltitude++;
            }
        }
        else if(pInput->m_ArrowDown)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;
                m_pCurrentSector->m_IsFloorSlope = true;
                m_pCurrentSector->m_FloorSlopeAltitude--;
            }
        }
        if(pInput->m_ArrowLeft)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;
                m_pCurrentSector->m_FloorSlopeVert--;
                if(m_pCurrentSector->m_FloorSlopeVert < 0)
                {
                    m_pCurrentSector->m_FloorSlopeVert = 2;
                }
            }
        }
        else if(pInput->m_ArrowRight)
        {
            if(!waiting_for_key_release)
            {
                waiting_for_key_release = true;
                m_pCurrentSector->m_FloorSlopeVert++;
                if(m_pCurrentSector->m_FloorSlopeVert >= 3)
                {
                    m_pCurrentSector->m_FloorSlopeVert = 0;
                }
            }
        }
        else if(pInput->m_Enter)
        {
            m_EditorState = EDITORSTATE_NONE;
        }
        else
        {
            waiting_for_key_release = false;
        }
    }
}

bool CGame::InitTextures()
{
    char filename[256] = {0};
    for(int i = 0; i < MAX_TEXTURES; i++)
    {
        snprintf(filename, sizeof(filename), "data/images/%d.png", i);
        if(FileExists(filename))
        {
            m_Textures[i] = LoadTexture(filename);
        }
    }

    return true;
}

bool CGame::Init()
{
    if(!m_EditorMode)
    {
        g_LevelHandler.ReadLevel("TESTLEVEL.txt");
    }
    else
    {
        m_NumSectors = 1;
        m_pSectors = new CSector[m_NumSectors];

        m_pSectors[0].m_NumVertices = 6;
        m_pSectors[0].m_pVertices = new Vector2[m_pSectors[0].m_NumVertices];
        m_pSectors[0].m_pNeighbors = new CSector*[m_pSectors[0].m_NumVertices];
        m_pSectors[0].m_pTexturesIDs = new unsigned int[m_pSectors[0].m_NumVertices];
        for(int i = 0; i < m_pSectors[0].m_NumVertices; i++)
        {
            m_pSectors[0].m_pNeighbors[i] = nullptr;
            m_pSectors[0].m_pTexturesIDs[i] = 0;
        }

        m_pSectors[0].m_pVertices[0].x = -20.f;
        m_pSectors[0].m_pVertices[0].y = -10.f;

        m_pSectors[0].m_pVertices[1].x = 15.f;
        m_pSectors[0].m_pVertices[1].y = -12.f;

        m_pSectors[0].m_pVertices[2].x = 15.f;
        m_pSectors[0].m_pVertices[2].y = -10.f;

        m_pSectors[0].m_pVertices[3].x = 15.f;
        m_pSectors[0].m_pVertices[3].y = 10.f;

        m_pSectors[0].m_pVertices[4].x = 15.f;
        m_pSectors[0].m_pVertices[4].y = 12.f;

        m_pSectors[0].m_pVertices[5].x = -20.f;
        m_pSectors[0].m_pVertices[5].y = 10.f;

        m_pSectors[0].m_Ceiling = 10.f;
        m_pSectors[0].m_Floor = -10.f;

        m_pCurrentSector = &m_pSectors[0];
    }
    m_NumEntities = 1;
    m_pEntities = new IEntity*[m_NumEntities];
    m_pEntities[0] = new CPlayer({0,1,0});
    return InitTextures();
}

void CGame::Destroy()
{
    if(m_pEntities)
    {
        for(int i = 0; i < m_NumEntities; i++)
        {
            delete m_pEntities[i];
            m_pEntities[i] = nullptr;
        }
        delete[] m_pEntities;
        m_pEntities = nullptr;
    }

    if(m_pSectors)
    {
        for(int i = 0; i < m_NumSectors; i++)
        {
            if(m_pSectors[i].m_pVertices)
            {
                delete[] m_pSectors[i].m_pVertices;
                m_pSectors[i].m_pVertices = nullptr;
                delete[] m_pSectors[i].m_pNeighbors;
                m_pSectors[i].m_pNeighbors = nullptr;
                delete[] m_pSectors[i].m_pTexturesIDs;
                m_pSectors[i].m_pTexturesIDs = nullptr;
            }
        }
        delete[] m_pSectors;
        m_pSectors = nullptr;
    }
}

void CGame::Update()
{

    if(m_EditorMode)
    {
        UpdateEditorInput();
        UpdateEditorCamera();
        return;
    }

    for(int i = 0; i < m_NumSectors; i++)
    {
        m_pSectors[i].m_Active = false;
    }

    ValidateActiveSectors(m_pCurrentSector);

    if(m_pEntities)
    {
        for(int i = 0; i < m_NumEntities; i++)
        {
            if(m_pEntities[i])
            {
                m_pEntities[i]->Update();
            }
        }
    }
}

void CGame::Render()
{
    if(m_EditorMode)
    {
        RenderGrid();
        RenderSectors2D(m_pCurrentSector);
        RenderEditorInfo();
        return;
    }

    RenderSectors();

    if(m_pEntities)
    {
        for(int i = 0; i < m_NumEntities; i++)
        {
            if(m_pEntities[i])
            {
                m_pEntities[i]->Render();
            }
        }
    }
}

unsigned long long CGame::SectorPointerToID(CSector *pSector)
{
    //i dont know if this will work on all systems, may need to remake...
    return ((unsigned long long)pSector - (unsigned long long)&m_pSectors[0]) / sizeof(CSector);
}
