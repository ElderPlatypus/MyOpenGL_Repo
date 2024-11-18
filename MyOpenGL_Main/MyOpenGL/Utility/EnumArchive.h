#pragma once

enum Extrude
{
    //For extrude
    increase, decrease,
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
    Triangle_Fan
};