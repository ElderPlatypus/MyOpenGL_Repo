#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>


class Transform
{

    glm::vec3 mPosition;
    glm::vec3 mScale;
    glm::quat mOrientation;

public:
    Transform()
        : mPosition(glm::vec3(0.0f)), mScale(glm::vec3(1.0f)), mOrientation(glm::quat(1,0,0,0)) {}

   
    ///Rotation Yaw and Pitch
    void AddRotation(const glm::quat& rot)
    {
        mOrientation = mOrientation * rot; //rotation = current rot multilpied ny new rot
        mOrientation = glm::normalize(mOrientation); //Normalizes the vector to a unit-vector with length 1
    }
    void AddPitch(float angle_Degrees) //Rotation along Y-axis
    {
        float angleToRadians = glm::radians(angle_Degrees);
        glm::vec3 rightVector = glm::rotate(mOrientation, glm::vec3(1.0f, 0.f, 0.f));
        glm::quat pitchRotation = glm::angleAxis(angleToRadians, rightVector);
        AddRotation(pitchRotation);
    }
    void AddYaw(float angle_Degress) //Rotation along X-axis
    {
        float angleToRadians = glm::radians(angle_Degress);
        glm::quat yawRotation = glm::angleAxis(angleToRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        AddRotation(yawRotation);
    }

    ///Setters
    void SetRotation(const glm::quat& rot){ mOrientation = rot;} //Set rotation in form of glm::quat(angle,x,y,z)
    void SetPosition(const glm::vec3& pos){mPosition = pos;} //Set position glm::vec3(x,y,z)
    void SetScale(const glm::vec3& scl){mScale = scl;} //Set scale glm::vec3(x,y,z)
    void SetRotation(const glm::vec3& forwardDirection, const glm::vec3& upDirection = glm::vec3(0.0f, 1.0f, 0.0f))
    {
        glm::vec3 normalizedForward = glm::normalize(forwardDirection);
        glm::vec3 defaultForward(0.0f, 0.0f, -1.0f);
        glm::quat newRotation = glm::rotation(defaultForward, normalizedForward);
        mOrientation = newRotation;
    }
    void SetRotationFromPitch(float pitchDegrees)
    {
        float pitchRadians = glm::radians(pitchDegrees);
        glm::quat pitchRotation = glm::angleAxis(pitchRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        mOrientation = pitchRotation;
    }
    void SetRotationFromYaw(float yawDegrees)
    {
        float yawRadians = glm::radians(yawDegrees);
        glm::quat yawRotation = glm::angleAxis(yawRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        mOrientation = yawRotation;
    }
    void SetTransformMatrix(const glm::mat4 matrix) //Set matrix transformation: transforms a matrix consiting of all neccessary components
    {
        glm::vec3 scale; 
        glm::quat orientation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;

        glm::decompose(matrix, scale, orientation, translation, skew, perspective); //Decomposing the matrix to fit the trasnformation matrix
        //bool decompose(mat<4, 4, T, Q> const& ModelMatrix, vec<3, T, Q> & Scale, qua<T, Q> & Orientation, vec<3, T, Q> & Translation, vec<3, T, Q> & Skew, vec<4, T, Q> & Perspective)
        //Using LU decomposition, a faster alternative to Gauss Jordan elemination.
        // As said on https://glm.g-truc.net/0.9.6/api/a00204.html#ga0f1245817507156b337798a253577c8b it breaks down the matrix into parts: "translations, rotations adn scale components".
        //The user only need to insert these values which will be manpulated by the template.

        SetPosition(translation);
        SetRotation(orientation);
        SetScale(scale);
    }

    ///Getters
    //---------------------------------Matrix Components------------------------------------------------
    const glm::vec3& GetScale() const { return mScale; } //Returns scale as correct datatype
    const glm::quat& GetOrientation() const { return mOrientation; } //Returns orientation as correct datatype
    const glm::vec3& GetPosition() const { return mPosition; } //Returns position as correct datatype

    //---------------------------------Vector Components------------------------------------------------
    glm::vec3 GetRightVector() const { return glm::rotate(mOrientation, glm::vec3(1.f, 0.f, 0.f));} // Returns right vector: invert axis to get oposite direction
    glm::vec3 GetForwardVector() const { return glm::rotate(mOrientation, glm::vec3(0.f, 0.f, -1.f)); } //Returns forward vector inverted to match view directio: invert axis to get oposite direction
    glm::vec3 GetUpVector() const { return glm::rotate(mOrientation, glm::vec3(0.f, 1.f, 0.f)); } //Returns Up vector: invert axis to get oposite direction

    //---------------------------------Pitch & Yaw------------------------------------------------
    //Euler angles will be used and are vectors fixed to a rigid object and uses rotations in form of: yaw(x), pitch(y) and roll(z)
    float GetPitch() //Return current pitch in degrees
    {
        glm::vec3 eulerAngleX = glm::eulerAngles(mOrientation); //variable which stores the euler angles of the orientation: 3-vectors
        float pitch = eulerAngleX.x; //Extracting the EulerAngle corresponding to the axis i need for Pitch
        return glm::degrees(pitch); //Returns specific EulerAngle
    }

    float GetYaw() //Returns current yaw in degrees
    {
        glm::vec3 eulerAngleY = glm::eulerAngles(mOrientation); //variable which stores the euler angles of the orientation: 3-vectors
        float yaw = eulerAngleY.y;  //Extracting the EulerAngle corresponding to the axis i need for Pitch
        return glm::degrees(yaw); //Returns specific EulerAngle
    }
    
    glm::mat4 GetTransformMatrix() const //Returns the matrix as a const, menaing that it must beinitalized before used.
    {
        glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f),mScale); //Scaling the matrix
        glm::mat4 orientingMatrix = glm::toMat4(mOrientation);  //Converts the quaternions to 4x4 matrix
        glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), mPosition); //Translates a "4x4 matrix created from 3-scalars" 

        return translateMatrix * orientingMatrix * scalingMatrix; 
    }



}; 