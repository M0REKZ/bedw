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

bool IsPointInsideTriangle(Vector2 p, Vector2 p0, Vector2 p1, Vector2 p2)
{
    float s = (p0.x - p2.x) * (p.y - p2.y) - (p0.y - p2.y) * (p.x - p2.x);
    float t = (p1.x - p0.x) * (p.y - p0.y) - (p1.y - p0.y) * (p.x - p0.x);

    if ((s < 0) != (t < 0) && s != 0 && t != 0)
        return false;

    float d = (p2.x - p1.x) * (p.y - p1.y) - (p2.y - p1.y) * (p.x - p1.x);
    return d == 0 || (d < 0) == (s + t <= 0);
}

bool IsPointInsideSector(CSector * pSector, Vector2 Point)
{
    for(int i = 1; i < pSector->m_NumVertices; i++)
    {
        if(IsPointInsideTriangle(Point ,pSector->m_pVertices[0], pSector->m_pVertices[i], pSector->m_pVertices[(i + 1) % pSector->m_NumVertices]))
        {
            return true;
        }
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

float PointDistance3D(Vector3 p1, Vector3 p2)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float dz = p2.z - p1.z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
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

Vector2 ClosestPointOnLine(Vector2 Start, Vector2 End, Vector2 Point)
{
    Vector2 temp = {End.x - Start.x, End.y - Start.y};
    float temp2 = temp.x * temp.x + temp.y * temp.y;
    temp2 = (temp.y * (Point.y - Start.y) + temp.x * (Point.x - Start.x))/temp2;
    return {Start.x + temp2 * temp.x, Start.y + temp2 * temp.y};
}

float GetSlopeAltitude(Vector2 Point1, Vector2 Point2, Vector2 HighestPoint, float HighestPointAltitude, Vector2 CheckingPoint)
{
    Vector2 MiddlePoint = {(Point1.x + Point2.x)/2, (Point1.y + Point2.y)/2};
    float SlopeBase = PointDistance(MiddlePoint, HighestPoint);
    float Multiplier = (HighestPointAltitude + (HighestPointAltitude < 0.f ? -1.f : 1.f))/SlopeBase;
    Vector2 ClosestPoint = ClosestPointOnLine(Point1, Point2, CheckingPoint);
    return Multiplier * PointDistance(ClosestPoint, CheckingPoint);
}

void DoMovement(Vector3 Pos, Vector3 &InOutVel, float Radius, CSector ** ppInOutSector, bool * pGroundedState)
{
    CSector *pPrevSector = nullptr;
    CSector *pNextSector = nullptr;
    CSector *pCurrentSector = ppInOutSector ? *ppInOutSector : g_Game.GetCurrentSector();

    DoFloorCollision(Pos, InOutVel,Radius, pCurrentSector, pGroundedState);    
    DoCeilingCollision(Pos, InOutVel, Radius, pCurrentSector);

    do
    {
        pNextSector = nullptr;
        Vector2 Intersection = {Pos.x, Pos.z};
        Vector2 RadiusColDir;
        Vector2 PlayerRadiusColDir;

        for(int secid = -1; secid < pCurrentSector->m_NumVertices; secid++)
        {
            CSector * pSector;
            if(secid == -1)
                pSector = pCurrentSector;
            else
                pSector = pCurrentSector->m_pNeighbors[secid];

            if(!pSector)
                continue;

            for(int vertid = 0; vertid < pSector->m_NumVertices; vertid++)
            {
                bool notportal = false;
                if(pSector->m_Floor > pSector->m_Ceiling)
                    notportal = true;

                if(!notportal)
                {
                    //ignore the sector i was previously in
                    if(pPrevSector && pSector->m_pNeighbors[vertid] && pSector->m_pNeighbors[vertid] == pPrevSector)
                        continue;
                    
                    if(pSector->m_pNeighbors[vertid] && pSector->m_pNeighbors[vertid] == pCurrentSector)
                        continue;
                }

                float wallangle = GetAngleBetweenPoints(pSector->m_pVertices[vertid], pSector->m_pVertices[(vertid + 1) % pSector->m_NumVertices]);
                RadiusColDir = {Radius * cosf(wallangle + M_PI_2), Radius * sinf(wallangle + M_PI_2)};
                if(notportal)
                {
                    RadiusColDir.x *= -1.f;
                    RadiusColDir.y *= -1.f;
                    if(Pos.y <= pSector->m_Ceiling || Pos.y >= pSector->m_Floor)
                    {
                        if(IsPointInsideSector(pSector, {Pos.x, Pos.z}))
                        {
                            if(Pos.y > pSector->m_Floor)
                            {
                                DoFloorCollision(Pos, InOutVel, Radius, pSector, (pGroundedState && *pGroundedState) ? nullptr : pGroundedState);
                            }
                            else if(Pos.y < pSector->m_Ceiling)
                            {
                                DoCeilingCollision(Pos, InOutVel, Radius, pSector);
                            }
                            
                        }
                    }
                }

                float floor, ceiling;
                
                //it is a neighbor sector portal
                if(!notportal && pSector->m_pNeighbors[vertid])
                {
                    floor = pSector->m_pNeighbors[vertid]->m_Floor;
                    ceiling = pSector->m_pNeighbors[vertid]->m_Ceiling;

                    if(pSector->m_pNeighbors[vertid]->m_IsCeilingSlope && pSector->m_pNeighbors[vertid]->m_NumVertices == 3)
                    {
                        int ids[2] = {0,1};

                        if(pSector->m_pNeighbors[vertid]->m_CeilingSlopeVert == 0)
                        {
                            ids[0] = 1;
                            ids[1] = 2;
                        }
                        else if(pSector->m_pNeighbors[vertid]->m_CeilingSlopeVert == 1)
                        {
                            ids[0] = 2;
                            ids[1] = 0;
                        }

                        ceiling = GetSlopeAltitude(pSector->m_pNeighbors[vertid]->m_pVertices[ids[0]], pSector->m_pNeighbors[vertid]->m_pVertices[ids[1]],
                            pSector->m_pNeighbors[vertid]->m_pVertices[pSector->m_pNeighbors[vertid]->m_CeilingSlopeVert], pSector->m_pNeighbors[vertid]->m_CeilingSlopeAltitude - pSector->m_pNeighbors[vertid]->m_Ceiling, {Pos.x, Pos.z});
                        ceiling += pSector->m_pNeighbors[vertid]->m_Ceiling;
                    }

                    if(pSector->m_pNeighbors[vertid]->m_IsFloorSlope && pSector->m_pNeighbors[vertid]->m_NumVertices == 3)
                    {
                        int ids[2] = {0,1};

                        if(pSector->m_pNeighbors[vertid]->m_FloorSlopeVert == 0)
                        {
                            ids[0] = 1;
                            ids[1] = 2;
                        }
                        else if(pSector->m_pNeighbors[vertid]->m_FloorSlopeVert == 1)
                        {
                            ids[0] = 2;
                            ids[1] = 0;
                        }

                        floor = GetSlopeAltitude(pSector->m_pNeighbors[vertid]->m_pVertices[ids[0]], pSector->m_pNeighbors[vertid]->m_pVertices[ids[1]],
                            pSector->m_pNeighbors[vertid]->m_pVertices[pSector->m_pNeighbors[vertid]->m_FloorSlopeVert], pSector->m_pNeighbors[vertid]->m_FloorSlopeAltitude - pSector->m_pNeighbors[vertid]->m_Floor, {Pos.x, Pos.z});
                        floor += pSector->m_pNeighbors[vertid]->m_Floor;
                    }
                }
                //i touched a wall
                if(IntersectLines({Pos.x, Pos.z}, {Pos.x + InOutVel.x, Pos.z + InOutVel.z},
                    pSector->m_pVertices[vertid], pSector->m_pVertices[(vertid + 1) % pSector->m_NumVertices], Intersection) ||
                IntersectLines({-RadiusColDir.x + Pos.x + InOutVel.x, -RadiusColDir.y + Pos.z + InOutVel.z}, {Pos.x + InOutVel.x, Pos.z + InOutVel.z},
                    pSector->m_pVertices[vertid], pSector->m_pVertices[(vertid + 1) % pSector->m_NumVertices], Intersection))
                {
                    if(pSector->m_pNeighbors[vertid] && 
                        !notportal && //dont do for inverted sectors
                        Pos.y >= floor && Pos.y <= ceiling)
                    {
                        if(IsPointInsideSector(pSector->m_pNeighbors[vertid], {Pos.x + InOutVel.x, Pos.z + InOutVel.z}))
                        { 
                            pPrevSector = pSector;
                            pNextSector = pSector->m_pNeighbors[vertid];
                        }
                    }
                    else if(!(notportal && (Pos.y <= pSector->m_Ceiling || Pos.y >= pSector->m_Floor)))
                    {
                        InOutVel.x = Intersection.x - Pos.x;
                        InOutVel.z = Intersection.y - Pos.z;
                        InOutVel.x += RadiusColDir.x;
                        InOutVel.z += RadiusColDir.y;
                    }
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

void DoCeilingCollision(Vector3 Pos, Vector3 &InOutVel, float Radius, CSector *pSector)
{
    if(!pSector->m_IsCeilingSlope)
    {
        if((Pos.y + Radius) + InOutVel.y > pSector->m_Ceiling)
            InOutVel.y = pSector->m_Ceiling - (Pos.y + Radius);
    }
    else if(pSector->m_NumVertices == 3)
    {
        int ids[2] = {0,1};

        if(pSector->m_CeilingSlopeVert == 0)
        {
            ids[0] = 1;
            ids[1] = 2;
        }
        else if(pSector->m_CeilingSlopeVert == 1)
        {
            ids[0] = 2;
            ids[1] = 0;
        }

        float altitude = GetSlopeAltitude(pSector->m_pVertices[ids[0]], pSector->m_pVertices[ids[1]],
            pSector->m_pVertices[pSector->m_CeilingSlopeVert], pSector->m_CeilingSlopeAltitude - pSector->m_Ceiling, {Pos.x, Pos.z});
        altitude += pSector->m_Ceiling;
        if((Pos.y + Radius) + InOutVel.y > altitude)
            InOutVel.y = altitude - (Pos.y + Radius);
    }
}

void DoFloorCollision(Vector3 Pos, Vector3 &InOutVel, float Radius, CSector *pSector, bool * pGroundedState)
{
    bool grounded = false;
    if(!pSector->m_IsFloorSlope)
    {
        if((Pos.y - Radius) + InOutVel.y < pSector->m_Floor)
        {
            InOutVel.y = pSector->m_Floor - (Pos.y - Radius);
            grounded = true;
        }
    }
    else if(pSector->m_NumVertices == 3)
    {
        int ids[2] = {0,1};

        if(pSector->m_FloorSlopeVert == 0)
        {
            ids[0] = 1;
            ids[1] = 2;
        }
        else if(pSector->m_FloorSlopeVert == 1)
        {
            ids[0] = 2;
            ids[1] = 0;
        }

        float altitude = GetSlopeAltitude(pSector->m_pVertices[ids[0]], pSector->m_pVertices[ids[1]],
            pSector->m_pVertices[pSector->m_FloorSlopeVert], pSector->m_FloorSlopeAltitude - pSector->m_Floor, {Pos.x, Pos.z});
        altitude += pSector->m_Floor;
        if((Pos.y - Radius) + InOutVel.y < altitude)
        {
            InOutVel.y = altitude - (Pos.y - Radius);
            grounded = true;
        }
    }

    if(pGroundedState)
        *pGroundedState = grounded;
}

void DoEntityCollision(Vector3 MyPos, Vector3 &InOutVel, float MyRadius, Vector3 HisPos, float HisRadius)
{
    Vector3 FuturePoint = {InOutVel.x + MyPos.x, InOutVel.y + MyPos.y, InOutVel.z + MyPos.z};
    float dist = PointDistance3D(MyPos, HisPos);
    float futuredist = PointDistance3D(FuturePoint, HisPos);
    
    float BothRadius = (MyRadius + HisRadius);
    if(futuredist <= dist && dist < BothRadius/2)
    {
        float angle = GetAngleBetweenPoints({HisPos.x, HisPos.z}, {MyPos.x, MyPos.z});
        Vector2 RadiusColDir = {MyRadius * cosf(angle), MyRadius * sinf(angle)};
        InOutVel.x = HisPos.x - MyPos.x;
        InOutVel.z = HisPos.z - MyPos.z;
        InOutVel.x += RadiusColDir.x;
        InOutVel.z += RadiusColDir.y;
        InOutVel.x /= 2.f;
        InOutVel.z /= 2.f;
    }
}
