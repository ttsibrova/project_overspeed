#pragma once
#include <Input/InputLayer.h>
#include <raylib.h>

namespace input {

enum class Device : uint8_t
{
    None,
    Keyboard,
    Gamepad
};


class InputHandler
{
public:
    InputHandler():
        m_lastActiveDevice (input::Device::Keyboard)
    {}

    InputHandler (const InputHandler&)            = delete;
    InputHandler (InputHandler&&)                 = delete;
    InputHandler& operator= (const InputHandler&) = delete;
    InputHandler& operator= (InputHandler&&)      = delete;

    template<typename T>
    void registerInputLayer (const T& owner, const input::Layer& layer)
    {
        m_layers.emplace_back (reinterpret_cast<const void*> (&owner), std::ref (layer));
    }

    template<typename T>
    void unregisterInputLayers (const T& owner)
    {
        std::erase_if (m_layers, [ownerPtr = &owner] (const RegisteredLayer& rlayer) { return rlayer.owner == ownerPtr; });
    }

    void handleInput();
    void handleTargetedInput (const input::Layer& inputLayer);

    input::Device getActiveDevice() const { return m_lastActiveDevice; }

private:
    bool checkKeyboardInputs (const input::Layer& inputLayer) const;
    bool checkGamepadInputs (const input::Layer& inputLayer) const;

private:
    struct RegisteredLayer
    {
        const void* owner;
        std::reference_wrapper<const input::Layer> layer;
    };

    input::Device                m_lastActiveDevice;
    std::vector<RegisteredLayer> m_layers;
};

InputHandler& getInputHandler();

}


