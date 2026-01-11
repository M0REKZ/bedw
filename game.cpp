// Copyright (c) Benjamín Gajardo (also known as +KZ)

#include <raylib.h>
#include <rlgl.h>

#include "game.h"
#include "globals.h"
#include <entities/player.h>
#include <cmath>
#include <cstdio>

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
        BeginMode3D(g_Globals.m_RaylibCamera);
        rlBegin(RL_TRIANGLES);

        if(!m_pSectors[i].m_Active)
            continue;

        Color colorwall;
        colorwall.a = 255;
        colorwall.r = 255;
        colorwall.g = 0;
        colorwall.b = 0;

        // walls
        for(int vertid = 0; vertid < m_pSectors[i].m_NumVertices; vertid++)
        {
            Vector3 Verts[4];
            Verts[0] = {m_pSectors[i].m_pVertices[vertid].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[vertid].y};
            Verts[1] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};
            Verts[2] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, m_pSectors[i].m_Floor, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};
            Verts[3] = {m_pSectors[i].m_pVertices[vertid].x, m_pSectors[i].m_Floor, m_pSectors[i].m_pVertices[vertid].y};

            rlSetTexture(m_Textures[m_pSectors[i].m_pTexturesIDs[vertid]].id);
            rlColor4ub(WHITE.r, WHITE.g, WHITE.b, WHITE.a);

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

                    rlTexCoord2f(1.f, 0.f);
                    rlVertex3f(Verts[1].x, Verts[1].y, Verts[1].z);
                    rlTexCoord2f(0.f, 0.f);
                    rlVertex3f(Verts[0].x, Verts[0].y, Verts[0].z);
                    rlTexCoord2f(1.f, ceiluv);
                    rlVertex3f(VertsNeigh[1].x, VertsNeigh[1].y, VertsNeigh[1].z);

                    rlTexCoord2f(1.f, ceiluv);
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

                    rlTexCoord2f(1.f, flooruv);
                    rlVertex3f(VertsNeigh[2].x, VertsNeigh[2].y, VertsNeigh[2].z);
                    rlTexCoord2f(0.f, flooruv);
                    rlVertex3f(VertsNeigh[3].x, VertsNeigh[3].y, VertsNeigh[3].z);
                    rlTexCoord2f(0.f, 1.f);
                    rlVertex3f(Verts[3].x, Verts[3].y, Verts[3].z);

                    rlTexCoord2f(1.f, 1.f);
                    rlVertex3f(Verts[2].x, Verts[2].y, Verts[2].z);
                    rlTexCoord2f(1.f, flooruv);
                    rlVertex3f(VertsNeigh[2].x, VertsNeigh[2].y, VertsNeigh[2].z);
                    rlTexCoord2f(0.f, 1.f);
                    rlVertex3f(Verts[3].x, Verts[3].y, Verts[3].z);
                }

                continue;
            }

            //entire wall
            rlTexCoord2f(1.f, 0.f);
            rlVertex3f(Verts[1].x, Verts[1].y, Verts[1].z);
            rlTexCoord2f(0.f, 0.f);
            rlVertex3f(Verts[0].x, Verts[0].y, Verts[0].z);
            rlTexCoord2f(1.f, 1.f);
            rlVertex3f(Verts[2].x, Verts[2].y, Verts[2].z);

            rlTexCoord2f(1.f, 1.f);
            rlVertex3f(Verts[2].x, Verts[2].y, Verts[2].z);
            rlTexCoord2f(0.f, 0.f);
            rlVertex3f(Verts[0].x, Verts[0].y, Verts[0].z);
            rlTexCoord2f(0.f, 1.f);
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

        /*BeginMode3D(g_Globals.m_RaylibCamera);

        for(int vertid = 1; vertid < m_pSectors[i].m_NumVertices; vertid++)
        {
            Vector3 tri[3];

            tri[0] = {m_pSectors[i].m_pVertices[0].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[0].y};
            tri[1] = {m_pSectors[i].m_pVertices[vertid].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[vertid].y};
            tri[2] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, m_pSectors[i].m_Ceiling, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};

            DrawTriangle3D(tri[0], tri[1], tri[2], colorceil);

            tri[2] = {m_pSectors[i].m_pVertices[0].x, m_pSectors[i].m_Floor, m_pSectors[i].m_pVertices[0].y};
            tri[1] = {m_pSectors[i].m_pVertices[vertid].x, m_pSectors[i].m_Floor, m_pSectors[i].m_pVertices[vertid].y};
            tri[0] = {m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].x, m_pSectors[i].m_Floor, m_pSectors[i].m_pVertices[(vertid + 1) % (m_pSectors[i].m_NumVertices)].y};

            DrawTriangle3D(tri[0], tri[1], tri[2], colorfloor);
        }

        EndMode3D();*/
        
        
    }
}

bool CGame::InitTextures()
{
    Image temp = LoadImage("data/images/floor_dirt_bw.png");
    m_Textures[0] = LoadTextureFromImage(temp);
    UnloadImage(temp);
    //m_pTextures[1] = LoadTexture("data/images/floor_robot_dark.png");

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
    m_pEntities[0] = new CPlayer({0,0,0});

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
