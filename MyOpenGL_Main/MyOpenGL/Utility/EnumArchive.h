#pragma once

enum Extrude
{
    //For extrude
    Increase, Decrease,
};

enum Mouse
{
    //Mouse buttons
    RightMouseButton,
    MouseMotionX, MouseMotionY,
    LeftMouseButton
};

enum Direction
{
    //Directions
    Forward, Backwards,
    Right, Left,
    Up, Down,
    IncreaseSpeed, NormalSpeed,
};

enum CameraState
{
    //Other buttons
    CameraFreeMovement_1,
    CameraStatic_CharacterMovement_2,
    CameraStatic_FollowPlayer_3
};

enum GLDrawType
{
    Triangle,
    Line_Strip,
    Points,
    Triangle_Fan,
    PolygonMode
};

enum MeshShape
{
   Cube,
   Sphere,
   Pyramid
};

enum TextureType
{
    Wall = 0,
    Container = 1
};

enum LightType
{
    Default = 0,
    Slope = 1
};

enum CollisionTypes
{
    AABB,
    AABBInverse,
    TrackPlayer,
    ProjectileHit,
    AABBConvex
};
