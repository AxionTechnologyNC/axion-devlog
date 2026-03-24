#include "pch.h"
#include "FlyCamera.h"

FlyCamera::FlyCamera(const glm::vec3& position, const glm::vec3& worldUp, float yaw, float pitch)
    : position_(position),
    worldUp_(worldUp),
    yaw_(yaw),
    pitch_(pitch)
{
    updateVectors();
}

glm::mat4 FlyCamera::getViewMatrix() const
{
    return glm::lookAt(position_, position_ + front_, up_);
}

void FlyCamera::move(CameraMoveDirection direction, float deltaTime)
{
    const float velocity = movementSpeed_ * deltaTime;

    switch (direction)
    {
    case CameraMoveDirection::Forward:
        position_ += front_ * velocity;
        break;
    case CameraMoveDirection::Backward:
        position_ -= front_ * velocity;
        break;
    case CameraMoveDirection::Left:
        position_ -= right_ * velocity;
        break;
    case CameraMoveDirection::Right:
        position_ += right_ * velocity;
        break;
    case CameraMoveDirection::Up:
        position_ += worldUp_ * velocity;
        break;
    case CameraMoveDirection::Down:
        position_ -= worldUp_ * velocity;
        break;
    }
}

void FlyCamera::rotate(float yawOffset, float pitchOffset, bool constrainPitch)
{
    yaw_ += yawOffset * lookSensitivity_;
    pitch_ += pitchOffset * lookSensitivity_;

    if (constrainPitch)
    {
        pitch_ = std::clamp(pitch_, -kMaxPitch, kMaxPitch);
    }

    updateVectors();
}

void FlyCamera::zoom(float yOffset)
{
    zoomDegrees_ -= yOffset;
    zoomDegrees_ = std::clamp(zoomDegrees_, kMinZoomDegrees, kMaxZoomDegrees);
}

void FlyCamera::updateVectors()
{
    glm::vec3 front;
    front.x = std::cos(glm::radians(yaw_)) * std::cos(glm::radians(pitch_));
    front.y = std::sin(glm::radians(pitch_));
    front.z = std::sin(glm::radians(yaw_)) * std::cos(glm::radians(pitch_));

    front_ = glm::normalize(front);
    right_ = glm::normalize(glm::cross(front_, worldUp_));
    up_ = glm::normalize(glm::cross(right_, front_));
}