#pragma once

#include <RmlUi/RmlUiDocument.h>

#include <Engine/Scripting/Script.h>

class CustomEventInstancer;

API_CLASS() class GAME_API AnimationExample : public Script
{
API_AUTO_SERIALIZATION();
DECLARE_SCRIPTING_TYPE(AnimationExample);

    // [Script]
    void OnEnable() override;
    void OnDisable() override;
    void OnUpdate() override;

private:
    CustomEventInstancer* event_listener_instancer = nullptr;
    String title;
    float t_prev_fade = 0.0f;
protected:
    RmlUiDocument* Document = nullptr;
};
