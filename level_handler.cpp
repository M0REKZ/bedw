// Copyright (c) Benjamín Gajardo (also known as +KZ)

#include "level_handler.h"

#include <raylib.h>
#include <fstream>
#include <game.h>


CLevelHandler g_LevelHandler;

bool CLevelHandler::ReadLevel(const char *filename)
{
    g_Game.Destroy();

    char filepath[256] = {0};
    snprintf(filepath, sizeof(filepath), "data/levels/%s", filename);
    std::ifstream file(filepath);

    std::string line;

    unsigned long long cursector = 0;
    unsigned long long curvert = 0;
    bool found_numsectors = false;
    bool found_numvertices = false;

    unsigned long long currentgamesector = 0;

    while(std::getline(file, line))
    {
        if(strstr(line.c_str(), "CURSEC"))
        {
            sscanf(line.c_str(), "CURSEC %llu", &currentgamesector);
        }

        if(!found_numsectors) //cant do anything without knowing sector number
        {
            if(strstr(line.c_str(), "NUMSECTORS"))
            {
                sscanf(line.c_str(), "NUMSECTORS %d", &g_Game.m_NumSectors);
                g_Game.m_pSectors = new CSector[g_Game.m_NumSectors];
                found_numsectors = true;
            }
        }
        else
        {
            if(strstr(line.c_str(), "SECTOR"))
            {
                sscanf(line.c_str(), "SECTOR %llu", &cursector);
                found_numvertices = false; //need to find this now
            }
            else if(strstr(line.c_str(), "FLOOR_TEXTURE"))
            {
                sscanf(line.c_str(), "FLOOR_TEXTURE %u", &g_Game.GetSector(cursector)->m_FloorTextureID);
            }
            else if(strstr(line.c_str(), "CEILING_TEXTURE"))
            {
                sscanf(line.c_str(), "CEILING_TEXTURE %u", &g_Game.GetSector(cursector)->m_CeilingTextureID);
            }
            else if(strstr(line.c_str(), "FLOOR"))
            {
                sscanf(line.c_str(), "FLOOR %f", &g_Game.GetSector(cursector)->m_Floor);
            }
            else if(strstr(line.c_str(), "CEILING"))
            {
                sscanf(line.c_str(), "CEILING %f", &g_Game.GetSector(cursector)->m_Ceiling);
            }
            
            if(!found_numvertices)
            {
                if(strstr(line.c_str(), "NUMVERTICES"))
                {
                    CSector * pSec = g_Game.GetSector(cursector);

                    sscanf(line.c_str(), "NUMVERTICES %d", &pSec->m_NumVertices);
                    
                    pSec->m_pVertices = new Vector2[pSec->m_NumVertices];
                    pSec->m_pNeighbors = new CSector*[pSec->m_NumVertices];
                    pSec->m_pTexturesIDs = new unsigned int[pSec->m_NumVertices];
                    for(int i = 0; i < pSec->m_NumVertices; i++)
                    {
                        pSec->m_pNeighbors[i] = nullptr;
                        pSec->m_pTexturesIDs[i] = 0;
                    }

                    found_numvertices = true;
                }
            }
            else
            {
                if(strstr(line.c_str(), "CUR_VERT"))
                {
                    sscanf(line.c_str(), "CUR_VERT %llu", &curvert);
                }
                else if(strstr(line.c_str(), "VERT"))
                {
                    sscanf(line.c_str(), "VERT %f %f", &g_Game.GetSector(cursector)->m_pVertices[curvert].x, &g_Game.GetSector(cursector)->m_pVertices[curvert].y);
                }
                else if(strstr(line.c_str(), "NEIGHBOR"))
                {
                    unsigned long long neighid;
                    sscanf(line.c_str(), "NEIGHBOR %llu", &neighid);
                    g_Game.GetSector(cursector)->m_pNeighbors[curvert] = g_Game.GetSector(neighid);
                }
                else if(strstr(line.c_str(), "WALL_TEXTURE"))
                {
                    sscanf(line.c_str(), "WALL_TEXTURE %u", &g_Game.GetSector(cursector)->m_pTexturesIDs[curvert]);
                }
                //only read if sector has 3 vertices
                else if(g_Game.GetSector(cursector)->m_NumVertices == 3 && strstr(line.c_str(), "CEILSLOPEID"))
                {
                    sscanf(line.c_str(), "CEILSLOPEID %d", &g_Game.GetSector(cursector)->m_CeilingSlopeVert);
                    g_Game.GetSector(cursector)->m_IsCeilingSlope = true;
                }
                else if(g_Game.GetSector(cursector)->m_NumVertices == 3 && strstr(line.c_str(), "FLOSLOPEID"))
                {
                    sscanf(line.c_str(), "FLOSLOPEID %d", &g_Game.GetSector(cursector)->m_FloorSlopeVert);
                    g_Game.GetSector(cursector)->m_IsCeilingSlope = true;
                }
                else if(g_Game.GetSector(cursector)->m_NumVertices == 3 && strstr(line.c_str(), "CEILSLOPEALT"))
                {
                    sscanf(line.c_str(), "CEILSLOPEALT %f", &g_Game.GetSector(cursector)->m_CeilingSlopeAltitude);
                    g_Game.GetSector(cursector)->m_IsCeilingSlope = true;
                }
                else if(g_Game.GetSector(cursector)->m_NumVertices == 3 && strstr(line.c_str(), "FLOSLOPEALT"))
                {
                    sscanf(line.c_str(), "FLOSLOPEALT %f", &g_Game.GetSector(cursector)->m_FloorSlopeAltitude);
                    g_Game.GetSector(cursector)->m_IsCeilingSlope = true;
                }
            }
        }
    }

    g_Game.m_pCurrentSector = g_Game.GetSector(currentgamesector);

    file.close();
    return false;
}

bool CLevelHandler::SaveLevel(const char *filename)
{
    char filepath[256] = {0};
    snprintf(filepath, sizeof(filepath), "data/levels/%s", filename);
    std::ofstream file(filepath);

    file << "CURSEC " << g_Game.SectorPointerToID(g_Game.GetCurrentSector()) << std::endl;

    file << "NUMSECTORS " << g_Game.NumSectors() << std::endl;

    for(int secid = 0; secid < g_Game.NumSectors(); secid++)
    {
        file << "SECTOR " << secid << std::endl;

        CSector * pSector = g_Game.GetSector(secid);

        file << "FLOOR " << pSector->m_Floor << std::endl;
        file << "CEILING " << pSector->m_Ceiling << std::endl;
        file << "FLOOR_TEXTURE " << pSector->m_FloorTextureID << std::endl;
        file << "CEILING_TEXTURE " << pSector->m_CeilingTextureID << std::endl;

        file << "NUMVERTICES " << pSector->m_NumVertices << std::endl;

        for(int vertid = 0; vertid < pSector->m_NumVertices; vertid++)
        {
            file << "CUR_VERT " << vertid << std::endl;
            file << "VERT " << pSector->m_pVertices[vertid].x << " " << pSector->m_pVertices[vertid].y << std::endl;
            if(pSector->m_pNeighbors[vertid])
            {
                unsigned long long neighid = g_Game.SectorPointerToID(pSector->m_pNeighbors[vertid]);
                file << "NEIGHBOR " << neighid << std::endl;
            }
            file << "WALL_TEXTURE " << pSector->m_pTexturesIDs[vertid] << std::endl;
        }
    }

    file.close();
    return true;
}
