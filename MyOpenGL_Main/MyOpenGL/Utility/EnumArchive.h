#pragma once

enum class Extrude
{
    //For extrude
    Increase, Decrease,
};

enum class Mouse
{
    //Mouse buttons
    RightMouseButton,
    MouseMotionX, MouseMotionY,
    LeftMouseButton
};

enum class Direction
{
    //Directions
    Forward, Backwards,
    Right, Left,
    Up, Down,
    IncreaseSpeed, NormalSpeed,
};

enum class CameraState
{
    //Other buttons
    CameraFreeMovement_1,
    CameraStatic_CharacterMovement_2,
    CameraStatic_FollowPlayer_3
};

enum class GLDrawType
{
    Triangle,
    Line_Strip,
    Points,
    Triangle_Fan,
    PolygonMode
};

enum class MeshShape
{
   Cube,
   Sphere,
   Pyramid
};

enum class TextureType
{
    Wall = 0,
    Container = 1,
    SnowFlake = 2
};

enum class LightType
{
    Default = 0,
    Slope = 1
};

enum class CollisionType
{
    AABB,
    AABBInverse,
    BallBall,
    BallBallInverse

};

enum class BoundTypes
{
    AABB = 0,
    Sphere = 1
};

enum class ParticleType
{
    None,
    Snow,
    Rain,
    Vortex
};