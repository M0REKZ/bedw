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
        for(int vertid = 0; vertid < m_pSectors[i].m_NumVertices; vertid++)
        {
            Vector3 Verts[4];
            Verts[0] = {m_pSectors[i].m_pVertices[vertid].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[vertid].y};
            Verts[1] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};
            Verts[2] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, m_pSectors[i].m_Floor, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};
            Verts[3] = {m_pSectors[i].m_pVertices[vertid].x, m_pSectors[i].m_Floor, m_pSectors[i].m_pVertices[vertid].y};

            float vertdist = PointDistance(m_pSectors[i].m_pVertices[vertid], m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)]);
            float huv = vertdist/32; // horizontal uv

            float ceildist = (m_pSectors[i].m_Ceiling - m_pSectors[i].m_Floor);
            float vuv = ceildist/32; // vertical uv

            float dist = PointDistance({g_Globals.m_Camera.m_Pos.x, g_Globals.m_Camera.m_Pos.z}, m_pSectors[i].m_pVertices[vertid]);
            float distalt = PointDistance({g_Globals.m_Camera.m_Pos.x, g_Globals.m_Camera.m_Pos.z}, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)]);

            dist = (dist + distalt)/2.f;

            dist = 1 - dist/100.f;

            rlSetTexture(m_Textures[m_pSectors[i].m_pTexturesIDs[vertid]].id);
            rlColor4ub(WHITE.r * dist, WHITE.g * dist, WHITE.b * dist, WHITE.a);

            if(m_pSectors[i].m_pNeighbors[vertid])
            {
                CSector *pNeigh = m_pSectors[i].m_pNeighbors[vertid];
                Vector3 VertsNeigh[4];
                VertsNeigh[0] = {m_pSectors[i].m_pVertices[vertid].x, pNeigh->m_Ceiling, m_pSectors[i].m_pVertices[vertid].y};
                VertsNeigh[1] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, pNeigh->m_Ceiling, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};
                VertsNeigh[2] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, pNeigh->m_Floor, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};
                VertsNeigh[3] = {m_pSectors[i].m_pVertices[vertid].x, pNeigh->m_Floor, m_pSectors[i].m_pVertices[vertid].y};


                //top
                if(pNeigh->m_Ceiling < m_pSectors[i].m_Ceiling)
                {
                    float ceiluv = (((m_pSectors[i].m_Ceiling - m_pSectors[i].m_Floor) - (pNeigh->m_Ceiling - m_pSectors[i].m_Floor)) / (m_pSectors[i].m_Ceiling - m_pSectors[i].m_Floor));

                    rlTexCoord2f(huv, 0.f);
                    rlVertex3f(Verts[1].x, Verts[1].y, Verts[1].z);
                    rlTexCoord2f(0.f, 0.f);
                    rlVertex3f(Verts[0].x, Verts[0].y, Verts[0].z);
                    rlTexCoord2f(huv, ceiluv);
                    rlVertex3f(VertsNeigh[1].x, VertsNeigh[1].y, VertsNeigh[1].z);

                    rlTexCoord2f(huv, ceiluv);
                    rlVertex3f(VertsNeigh[1].x, VertsNeigh[1].y, VertsNeigh[1].z);
                    rlTexCoord2f(0.f, 0.f);
                    rlVertex3f(Verts[0].x, Verts[0].y, Verts[0].z);
                    rlTexCoord2f(0.f, ceiluv);
                    rlVertex3f(VertsNeigh[0].x, VertsNeigh[0].y, VertsNeigh[0].z);
                }

                //bottom

                if(pNeigh->m_Floor > m_pSectors[i].m_Floor)
                {
                    float flooruv = (((m_pSectors[i].m_Ceiling - m_pSectors[i].m_Floor) - (pNeigh->m_Floor - m_pSectors[i].m_Floor)) / (m_pSectors[i].m_Ceiling - m_pSectors[i].m_Floor));

                    rlTexCoord2f(huv, flooruv);
                    rlVertex3f(VertsNeigh[2].x, VertsNeigh[2].y, VertsNeigh[2].z);
                    rlTexCoord2f(0.f, flooruv);
                    rlVertex3f(VertsNeigh[3].x, VertsNeigh[3].y, VertsNeigh[3].z);
                    rlTexCoord2f(0.f, 1.f);
                    rlVertex3f(Verts[3].x, Verts[3].y, Verts[3].z);

                    rlTexCoord2f(huv, 1.f);
                    rlVertex3f(Verts[2].x, Verts[2].y, Verts[2].z);
                    rlTexCoord2f(huv, flooruv);
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
            rlVertex3f(m_pSectors[i].m_pVertices[0].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[0].y);
            rlTexCoord2f((m_pSectors[i].m_pVertices[vertid].x)/32, (m_pSectors[i].m_pVertices[vertid].y)/32);
            rlVertex3f(m_pSectors[i].m_pVertices[vertid].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[vertid].y);
            rlTexCoord2f((m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x)/32, (m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y)/32);
            rlVertex3f(m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y);
        }

        rlSetTexture(m_Textures[m_pSectors[i].m_FloorTextureID].id);
        rlColor4ub(WHITE.r, WHITE.g, WHITE.b, WHITE.a);

        for(int vertid = 1; vertid < m_pSectors[i].m_NumVertices; vertid++)
        {
            rlTexCoord2f((m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x)/32, (m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y)/32);
            rlVertex3f(m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, m_pSectors[i].m_Floor, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y);
            rlTexCoord2f((m_pSectors[i].m_pVertices[vertid].x)/32, (m_pSectors[i].m_pVertices[vertid].y)/32);
            rlVertex3f(m_pSectors[i].m_pVertices[vertid].x, m_pSectors[i].m_Floor, m_pSectors[i].m_pVertices[vertid].y);
            rlTexCoord2f((m_pSectors[i].m_pVertices[0].x)/32, (m_pSectors[i].m_pVertices[0].y)/32);
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
            if(m_EditorState == EDITORSTATE_SETTING_NEIGHBOR && m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_NEIGH_SEC && &m_pSectors[m_EditorNeighSec] == &m_pSectors[i])
                DrawLineV(m_pSectors[i].m_pVertices[vertid], m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)], {255,255,0,255});
            else if(m_EditorState == EDITORSTATE_SETTING_NEIGHBOR && vertid == m_EditorSelectedVert && (pSelectedSector == &m_pSectors[i] || &m_pSectors[m_EditorNeighSec] == &m_pSectors[i]))
                DrawLineV(m_pSectors[i].m_pVertices[vertid], m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)], {255,255,0,255});
            else if(m_pSectors[i].m_pNeighbors[vertid])
            {
                if(pSelectedSector == &m_pSectors[i])
                    DrawLineV(m_pSectors[i].m_pVertices[vertid], m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)], {0,255,255,255});
                else
                    DrawLineV(m_pSectors[i].m_pVertices[vertid], m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)], {0,0,255,255});
            }
            else if(pSelectedSector == &m_pSectors[i])
                DrawLineV(m_pSectors[i].m_pVertices[vertid], m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)], {0,255,0,255});
            else
                DrawLineV(m_pSectors[i].m_pVertices[vertid], m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)], {255,0,0,255});
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
        snprintf(tempchar, sizeof(tempchar), "Current Sector: %llu\n", sectorid);
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
                for(int i = 0; i < m_NumSectors; i++)
                {
                    pTempSec[i] = m_pSectors[i];
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

                if(m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_FIRST_VERT || m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_SECOND_VERT)
                {
                    m_EditorSelectedVert--;
                    if(m_EditorSelectedVert < 0)
                        m_EditorSelectedVert = m_pCurrentSector->m_NumVertices - 1;
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
                
                if(m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_FIRST_VERT || m_EditorSettingNeighborState == EDITORSETTINGNEIGHBOR_SECOND_VERT)
                {
                    m_EditorSelectedVert++;
                    if(m_EditorSelectedVert >= m_pCurrentSector->m_NumVertices)
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
    m_NumSectors = 2;
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
    m_pSectors[0].m_pNeighbors[2] = &m_pSectors[1];

    m_pSectors[0].m_pVertices[3].x = 15.f;
    m_pSectors[0].m_pVertices[3].y = 10.f;

    m_pSectors[0].m_pVertices[4].x = 15.f;
    m_pSectors[0].m_pVertices[4].y = 12.f;

    m_pSectors[0].m_pVertices[5].x = -20.f;
    m_pSectors[0].m_pVertices[5].y = 10.f;

    m_pSectors[0].m_Ceiling = 10.f;
    m_pSectors[0].m_Floor = -10.f;

    m_pSectors[1].m_NumVertices = 5;
    m_pSectors[1].m_pVertices = new Vector2[m_pSectors[1].m_NumVertices];
    m_pSectors[1].m_pNeighbors = new CSector*[m_pSectors[1].m_NumVertices];
    m_pSectors[1].m_pTexturesIDs = new unsigned int[m_pSectors[1].m_NumVertices];
    for(int i = 0; i < m_pSectors[1].m_NumVertices; i++)
    {
        m_pSectors[1].m_pNeighbors[i] = nullptr;
        m_pSectors[1].m_pTexturesIDs[i] = 0;
    }

    m_pSectors[1].m_pVertices[0].x = 15.f;
    m_pSectors[1].m_pVertices[0].y = -10.f;

    m_pSectors[1].m_pVertices[1].x = 35.f;
    m_pSectors[1].m_pVertices[1].y = -10.f;

    m_pSectors[1].m_pVertices[2].x = 40.f;
    m_pSectors[1].m_pVertices[2].y = 0.f;

    m_pSectors[1].m_pVertices[3].x = 35.f;
    m_pSectors[1].m_pVertices[3].y = 10.f;

    m_pSectors[1].m_pVertices[4].x = 15.f;
    m_pSectors[1].m_pVertices[4].y = 10.f;
    m_pSectors[1].m_pNeighbors[4] = &m_pSectors[0];

    m_pSectors[1].m_Ceiling = 9.f;
    m_pSectors[1].m_Floor = -6.f;

    m_NumEntities = 1;
    m_pEntities = new IEntity*[m_NumEntities];
    m_pEntities[0] = new CPlayer({0,1,0});

    m_pCurrentSector = &m_pSectors[0];

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
        m_pSectors->m_Active = false;
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
