#include "libpch.h"
#include "Core/Input.h"

#include "Core/Application.h"
#include <GLFW/glfw3.h>

namespace Lib
{
    bool Lib::Input::IsKeyPressed(KeyCode key)
    {
        auto* window = static_cast<GLFWwindow*>(Application::Get()->GetNativeWindow());
        auto state = glfwGetKey(window, static_cast<int32_t>(key));
        return state == GLFW_PRESS;
    }

    bool Lib::Input::IsMouseButtonPressed(MouseCode button)
    {
        auto* window = static_cast<GLFWwindow*>(Application::Get()->GetNativeWindow());
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
        return state == GLFW_PRESS;
    }

    glm::vec2 Lib::Input::GetMousePosition()
    {
        auto* window = static_cast<GLFWwindow*>(Application::Get()->GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { (float)xpos, (float)ypos };
    }

    float Lib::Input::GetMouseX()
    {
        return GetMousePosition().x;
    }

    float Lib::Input::GetMouseY()
    {
        return GetMousePosition().y;
    }
}