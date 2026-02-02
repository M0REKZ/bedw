/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/

#ifndef COLLISION_H
#define COLLISION_H

#include <raylib.h>
#include <game.h>
#include <cstddef>

bool IntersectLines(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, Vector2 &intersection);
float GetAngleBetweenPoints(Vector2 p1, Vector2 p2);
float PointDistance(Vector2 p1, Vector2 p2);
float PointDistance3D(Vector3 p1, Vector3 p2);
float GetClockwise(Vector2 * pVecArr, size_t vecnum);
Vector2 ClosestPointOnLine(Vector2 Start, Vector2 End, Vector2 Point);
float GetSlopeAltitude(Vector2 Point1, Vector2 Point2, Vector2 HighestPoint, float HighestPointAltitude, Vector2 CheckingPoint);
bool IsPointInsideTriangle(Vector2 p, Vector2 p0, Vector2 p1, Vector2 p2);
bool IsPointInsideSector(CSector * pSector, Vector2 Point);

bool DoRaycast(Vector2 From, Vector2 &InOutTo, float &InOutAltitude, float Step, CSector ** ppInOutSector, CSector * pIgnoreSector = nullptr);
bool DoMovement(Vector3 Pos, Vector3 &InOutVel, float Radius, CSector ** ppInOutSector = nullptr, bool * pGroundedState = nullptr);
bool DoCeilingCollision(Vector3 Pos, Vector3 &InOutVel, float Radius, CSector *pSector);
bool DoFloorCollision(Vector3 Pos, Vector3 &InOutVel, float Radius, CSector *pSector, bool * pGroundedState = nullptr);
bool DoEntityCollision(Vector3 MyPos, Vector3 &InOutVel, float MyRadius, Vector3 HisPos, float HisRadius);

#endif