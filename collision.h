// Copyright (c) Benjamín Gajardo (also known as +KZ)

#ifndef COLLISION_H
#define COLLISION_H

#include <raylib.h>
#include <game.h>

bool IntersectLines(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, Vector2 &intersection);
float GetAngleBetweenPoints(Vector2 p1, Vector2 p2);
float PointDistance(Vector2 p1, Vector2 p2);

void DoMovement(Vector3 Pos, Vector3 &InOutVel, float Radius, CSector ** ppInOutSector = nullptr);

#endif