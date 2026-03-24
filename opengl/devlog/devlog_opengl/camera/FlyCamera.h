#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMoveDirection
{
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down
};

class FlyCamera
{
public:
    static constexpr float kDefaultYaw = -90.0f;
    static constexpr float kDefaultPitch = 0.0f;
    static constexpr float kDefaultSpeed = 2.5f;
    static constexpr float kDefaultLookSensitivity = 0.1f;
    static constexpr float kDefaultZoomDegrees = 45.0f;
    static constexpr float kMinZoomDegrees = 1.0f;
    static constexpr float kMaxZoomDegrees = 45.0f;
    static constexpr float kMaxPitch = 89.0f;

public:
    FlyCamera(
        const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f),
        const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = kDefaultYaw,
        float pitch = kDefaultPitch);

    glm::mat4 getViewMatrix() const;

    void move(CameraMoveDirection direction, float deltaTime);
    void rotate(float yawOffset, float pitchOffset, bool constrainPitch = true);
    void zoom(float yOffset);

    const glm::vec3& position() const { return position_; }
    const glm::vec3& front() const { return front_; }
    const glm::vec3& up() const { return up_; }
    const glm::vec3& right() const { return right_; }

    float yaw() const { return yaw_; }
    float pitch() const { return pitch_; }
    float zoomDegrees() const { return zoomDegrees_; }

    float movementSpeed() const { return movementSpeed_; }
    float lookSensitivity() const { return lookSensitivity_; }

    void setPosition(const glm::vec3& position) { position_ = position; }
    void setMovementSpeed(float speed) { movementSpeed_ = speed; }
    void setLookSensitivity(float sensitivity) { lookSensitivity_ = sensitivity; }

private:
    void updateVectors();

private:
    glm::vec3 position_{ 0.0f, 0.0f, 3.0f };
    glm::vec3 front_{ 0.0f, 0.0f, -1.0f };
    glm::vec3 up_{ 0.0f, 1.0f, 0.0f };
    glm::vec3 right_{ 1.0f, 0.0f, 0.0f };
    glm::vec3 worldUp_{ 0.0f, 1.0f, 0.0f };

    float yaw_ = kDefaultYaw;
    float pitch_ = kDefaultPitch;

    float movementSpeed_ = kDefaultSpeed;
    float lookSensitivity_ = kDefaultLookSensitivity;
    float zoomDegrees_ = kDefaultZoomDegrees;
};