// Copyright (c) Benjamín Gajardo (also known as +KZ)

#include "collision.h"
#include <cmath>

bool IntersectLines(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, Vector2 &intersection)
{
    float denom = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
    if (denom == 0.0f)
        return false;

    float t = ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / denom;
    float u = -((p1.x - p2.x) * (p1.y - p3.y) - (p1.y - p2.y) * (p1.x - p3.x)) / denom;

    if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f)
    {
        intersection.x = p1.x + t * (p2.x - p1.x);
        intersection.y = p1.y + t * (p2.y - p1.y);
        return true;
    }

    return false;
}

float GetAngleBetweenPoints(Vector2 p1, Vector2 p2)
{
    return atan2f(p2.y - p1.y, p2.x - p1.x);
}

float PointDistance(Vector2 p1, Vector2 p2)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return sqrtf(dx * dx + dy * dy);
}

// This function should return negative for valid sectors
float GetClockwise(Vector2 *pVecArr, size_t vecnum)
{
    float res = 0;
    for(size_t i = 0; i < vecnum; i++)
    {
        res += (pVecArr[(i + 1) % vecnum].x - pVecArr[i].x) * (pVecArr[(i + 1) % vecnum].y + pVecArr[i].y);
    }
    return res;
}

void DoMovement(Vector3 Pos, Vector3 &InOutVel, float Radius, CSector ** ppInOutSector)
{
    CSector *pPrevSector = nullptr;
    CSector *pNextSector = nullptr;
    CSector *pCurrentSector = ppInOutSector ? *ppInOutSector : g_Game.GetCurrentSector();

    if((Pos.y - Radius) + InOutVel.y < pCurrentSector->m_Floor)
        InOutVel.y = pCurrentSector->m_Floor - (Pos.y - Radius);
    
    if((Pos.y + Radius) + InOutVel.y > pCurrentSector->m_Ceiling)
        InOutVel.y = pCurrentSector->m_Ceiling - (Pos.y + Radius);

    do
    {
        pNextSector = nullptr;
        Vector2 Intersection = {Pos.x, Pos.z};
        Vector2 RadiusColDir;

        for(int vertid = 0; vertid < pCurrentSector->m_NumVertices; vertid++)
        {
            //ignore the sector i was previously in
            if(pPrevSector && pCurrentSector->m_pNeighbors[vertid] && pCurrentSector->m_pNeighbors[vertid] == pPrevSector)
                continue;

            float wallangle = GetAngleBetweenPoints(pCurrentSector->m_pVertices[vertid], pCurrentSector->m_pVertices[(vertid + 1) % pCurrentSector->m_NumVertices]);

            RadiusColDir = {Radius * cosf(wallangle + M_PI_2), Radius * sinf(wallangle + M_PI_2)};

            //i touched a wall
            if(IntersectLines({Pos.x, Pos.z}, {Pos.x + RadiusColDir.x, Pos.z + RadiusColDir.y},
                pCurrentSector->m_pVertices[vertid], pCurrentSector->m_pVertices[(vertid + 1) % pCurrentSector->m_NumVertices], Intersection) ||
            IntersectLines({Pos.x, Pos.z}, {Pos.x + InOutVel.x, Pos.z + InOutVel.z},
                pCurrentSector->m_pVertices[vertid], pCurrentSector->m_pVertices[(vertid + 1) % pCurrentSector->m_NumVertices], Intersection) ||
            IntersectLines({RadiusColDir.x + Pos.x + InOutVel.x, RadiusColDir.y + Pos.z + InOutVel.z}, {Pos.x + InOutVel.x, Pos.z + InOutVel.z},
                pCurrentSector->m_pVertices[vertid], pCurrentSector->m_pVertices[(vertid + 1) % pCurrentSector->m_NumVertices], Intersection))
            {
                //evil: ignore if intersection is the same as position, to avoid going back to previous sector next update
                if(Pos.x + InOutVel.x == Intersection.x && Pos.z + InOutVel.z == Intersection.y)
                    continue;

                //it is a neighbor sector portal
                if(pCurrentSector->m_pNeighbors[vertid] && Pos.y >= pCurrentSector->m_pNeighbors[vertid]->m_Floor && Pos.y <= pCurrentSector->m_pNeighbors[vertid]->m_Ceiling)
                {
                    pPrevSector = pCurrentSector;
                    pNextSector = pCurrentSector->m_pNeighbors[vertid];
                }
                else //it is not
                {
                    InOutVel.x = Intersection.x - Pos.x;
                    InOutVel.z = Intersection.y - Pos.z;
                    InOutVel.x += RadiusColDir.x;
                    InOutVel.z += RadiusColDir.y;
                }
            }
        }

        if(pNextSector)
        {
            pCurrentSector = pNextSector;
        }
    }
    while(pNextSector);

    if(ppInOutSector)
        *ppInOutSector = pCurrentSector;
}
