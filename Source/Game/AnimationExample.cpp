#include "AnimationExample.h"

#include <RmlUi/RmlUiDocument.h>
#include <RmlUi/IncludeRmlUiCore.h>

#include <Engine/Engine/Time.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Threading/Threading.h>

#if USE_EDITOR
#include "Editor/Editor.h"
#endif

// This script is copied and adapted from the RmlUi animation sample.

bool run_loop = true;
bool single_loop = false;
int nudge = 0;

class CustomEvent : public Rml::EventListener
{
private:
	Rml::Context* context;
public:
	CustomEvent(const Rml::String& value, Rml::Context* context)
		: value(value)
		, context(context)
	{
	}

	void ProcessEvent(Rml::Event& event) override
	{
		using namespace Rml;

		if (value == "exit")
			Engine::RequestExit();

		switch (event.GetId())
		{
		case EventId::Keydown:
		{
			Rml::Input::KeyIdentifier key_identifier = (Rml::Input::KeyIdentifier)event.GetParameter< int >("key_identifier", 0);

			if (key_identifier == Rml::Input::KI_SPACE)
			{
				run_loop = !run_loop;
			}
			else if (key_identifier == Rml::Input::KI_RETURN)
			{
				run_loop = false;
				single_loop = true;
			}
			else if (key_identifier == Rml::Input::KI_OEM_PLUS)
			{
				nudge = 1;
			}
			else if (key_identifier == Rml::Input::KI_OEM_MINUS)
			{
				nudge = -1;
			}
			else if (key_identifier == Rml::Input::KI_ESCAPE)
			{
				Engine::RequestExit();
			}
			else if (key_identifier == Rml::Input::KI_LEFT)
			{
				auto el = context->GetRootElement()->GetElementById("keyevent_response");
				if (el) el->Animate("left", Property{ -200.f, Property::DP }, 0.5, Tween{ Tween::Cubic });
			}
			else if (key_identifier == Rml::Input::KI_RIGHT)
			{
				auto el = context->GetRootElement()->GetElementById("keyevent_response");
				if (el) el->Animate("left", Property{ 200.f, Property::DP }, 0.5, Tween{ Tween::Cubic });
			}
			else if (key_identifier == Rml::Input::KI_UP)
			{
				auto el = context->GetRootElement()->GetElementById("keyevent_response");
				auto offset_right = Property{ 200.f, Property::DP };
				if (el) el->Animate("left", Property{ 0.f, Property::PX }, 0.5, Tween{ Tween::Cubic }, 1, true, 0, &offset_right);
			}
			else if (key_identifier == Rml::Input::KI_DOWN)
			{
				auto el = context->GetRootElement()->GetElementById("keyevent_response");
				if (el) el->Animate("left", Property{ 0.f, Property::PX }, 0.5, Tween{ Tween::Cubic });
			}
		}
		break;

		case EventId::Click:
		{
			auto el = event.GetTargetElement();
			if (el->GetId() == "transition_class")
			{
				el->SetClass("move_me", !el->IsClassSet("move_me"));
			}
		}
		break;

		case EventId::Animationend:
		{
			auto el = event.GetTargetElement();
			if (el->GetId() == "animation_event")
			{
				el->Animate("top", Property(Rml::Math::RandomReal(200.f), Property::PX), 1.2f, Tween{ Tween::Cubic, Tween::InOut });
				el->Animate("left", Property(Rml::Math::RandomReal(100.f), Property::PERCENT), 0.8f, Tween{ Tween::Cubic, Tween::InOut });
			}
		}
		break;

		default:
			break;
		}
	}

	void OnDetach(Rml::Element* /*element*/) override { delete this; }

private:
	Rml::String value;
};

class CustomEventInstancer : public Rml::EventListenerInstancer
{
private:
	Rml::Context* context;

public:
	CustomEventInstancer(Rml::Context* context)
		: Rml::EventListenerInstancer()
		, context(context)
	{
	}

	Rml::EventListener* InstanceEventListener(const Rml::String& value, Rml::Element* /*element*/) override { return new CustomEvent(value, context); }
};

AnimationExample::AnimationExample(const SpawnParams& params)
    : Script(params)
{
    _tickUpdate = true;
    _executeInEditor = true;
	title = "Animation sample";
}

void AnimationExample::OnEnable()
{
    Document = Actor::Cast<RmlUiDocument>(GetParent());
	if (Document->LoadDocument())
	{
		Document->Focus();
		using namespace Rml;
		using Rml::Transform;

#if USE_EDITOR
        if (!Editor::IsPlayMode && _executeInEditor)
        {
            Document->Show();
            return; // Show the document but do not update it in editor
        }
#endif

		Rml::ElementDocument* document = Document != nullptr ? Document->GetDocument() : nullptr;
		document->GetElementById("title")->SetInnerRML(title.ToStringAnsi().Get());

		// Button fun
		{
			auto el = document->GetElementById("start_game");
			auto p1 = Transform::MakeProperty({ Transforms::Rotate2D{10.f}, Transforms::TranslateX{100.f} });
			auto p2 = Transform::MakeProperty({ Transforms::Scale2D{3.f} });
			el->Animate("transform", p1, 1.8f, Tween{ Tween::Elastic, Tween::InOut }, -1, true);
			el->AddAnimationKey("transform", p2, 1.3f, Tween{ Tween::Elastic, Tween::InOut });
		}
		{
			auto el = document->GetElementById("high_scores");
			el->Animate("margin-left", Property(0.f, Property::PX), 0.3f, Tween{ Tween::Sine, Tween::In }, 10, true, 1.f);
			el->AddAnimationKey("margin-left", Property(100.f, Property::PX), 3.0f, Tween{ Tween::Circular, Tween::Out });
		}
		{
			auto el = document->GetElementById("options");
			el->Animate("image-color", Property(Colourb(128, 255, 255, 255), Property::COLOUR), 0.3f, Tween{}, -1, false);
			el->AddAnimationKey("image-color", Property(Colourb(128, 128, 255, 255), Property::COLOUR), 0.3f);
			el->AddAnimationKey("image-color", Property(Colourb(0, 128, 128, 255), Property::COLOUR), 0.3f);
			el->AddAnimationKey("image-color", Property(Colourb(64, 128, 255, 0), Property::COLOUR), 0.9f);
			el->AddAnimationKey("image-color", Property(Colourb(255, 255, 255, 255), Property::COLOUR), 0.3f);
		}
		{
			auto el = document->GetElementById("exit");
			PropertyDictionary pd;
			StyleSheetSpecification::ParsePropertyDeclaration(pd, "transform", "translate(200px, 200px) rotate(1215deg)");
			el->Animate("transform", *pd.GetProperty(PropertyId::Transform), 3.f, Tween{ Tween::Bounce, Tween::Out }, -1);
		}

		// Transform tests
		{
			auto el = document->GetElementById("generic");
			auto p = Transform::MakeProperty({ Transforms::TranslateY{50, Property::PX}, Transforms::Rotate3D{0, 0, 1, -90, Property::DEG}, Transforms::ScaleY{0.8f} });
			el->Animate("transform", p, 1.5f, Tween{ Tween::Sine, Tween::InOut }, -1, true);
		}
		{
			auto el = document->GetElementById("combine");
			auto p = Transform::MakeProperty({ Transforms::Translate2D{50, 50, Property::PX}, Transforms::Rotate2D(1215) });
			el->Animate("transform", p, 8.0f, Tween{}, -1, true);
		}
		{
			auto el = document->GetElementById("decomposition");
			auto p = Transform::MakeProperty({ Transforms::TranslateY{50, Property::PX}, Transforms::Rotate3D{0.8f, 0, 1, 110, Property::DEG} });
			el->Animate("transform", p, 1.3f, Tween{ Tween::Quadratic, Tween::InOut }, -1, true);
		}

		// Mixed units tests
		{
			auto el = document->GetElementById("abs_rel");
			el->Animate("margin-left", Property(50.f, Property::PERCENT), 1.5f, Tween{}, -1, true);
		}
		{
			auto el = document->GetElementById("abs_rel_transform");
			auto p = Transform::MakeProperty({ Transforms::TranslateX{0, Property::PX} });
			el->Animate("transform", p, 1.5f, Tween{}, -1, true);
		}
		{
			auto el = document->GetElementById("animation_event");
			el->Animate("top", Property(Rml::Math::RandomReal(250.f), Property::PX), 1.5f, Tween{ Tween::Cubic, Tween::InOut });
			el->Animate("left", Property(Rml::Math::RandomReal(250.f), Property::PX), 1.5f, Tween{ Tween::Cubic, Tween::InOut });
		}

		Document->Show();

		Rml::Context* context = document->GetContext();

		event_listener_instancer = New<CustomEventInstancer>(context);
		Rml::Factory::RegisterEventListenerInstancer((Rml::EventListenerInstancer*)event_listener_instancer);

		document->AddEventListener(Rml::EventId::Keydown, new CustomEvent("hello", context));
		document->AddEventListener(Rml::EventId::Keyup, new CustomEvent("hello", context));
		document->AddEventListener(Rml::EventId::Animationend, new CustomEvent("hello", context));
	}
}

void AnimationExample::OnDisable()
{
	if (event_listener_instancer)
	{
		Delete(event_listener_instancer);
		event_listener_instancer = nullptr;
		Rml::Factory::RegisterEventListenerInstancer(nullptr);
	}

	if (Document)
		Document->Close();
}

void AnimationExample::OnUpdate()
{
#if USE_EDITOR
    if (!Editor::IsPlayMode && _executeInEditor)
        return; // Show the document but do not update it in editor
#endif

	Rml::ElementDocument* document = Document != nullptr ? Document->GetDocument() : nullptr;
	if (document == nullptr)
		return;

	if (Time::GetTimeSinceStartup() - t_prev_fade >= 1.4)
	{
		auto el = document->GetElementById("help");
		if (el->IsClassSet("fadeout"))
		{
			el->SetClass("fadeout", false);
			el->SetClass("fadein", true);
		}
		else if (el->IsClassSet("fadein"))
		{
			el->SetClass("fadein", false);
			el->SetClass("textalign", true);
		}
		else
		{
			el->SetClass("textalign", false);
			el->SetClass("fadeout", true);
		}

		t_prev_fade = Time::GetTimeSinceStartup();
	}
	
	float t = Time::GetTimeSinceStartup();
	static double t_prev = 0.0f;
	float dt = float(t - t_prev);
	static int count_frames = 0;
	count_frames += 1;

	if (nudge)
	{
		t_prev = t;
		static float ff = 0.0f;
		ff += float(nudge) * 0.3f;
		auto el = document->GetElementById("exit");
		el->SetProperty(Rml::PropertyId::MarginLeft, Rml::Property(ff, Rml::Property::PX));
		float f_left = el->GetAbsoluteLeft();
		Rml::Log::Message(Rml::Log::LT_INFO, "margin-left: '%f'   abs: %f.", ff, f_left);
		nudge = 0;
	}

	if (dt > 0.2f)
	{
		t_prev = t;
		auto el = document->GetElementById("fps");
		float fps = float(count_frames) / dt;
		count_frames = 0;
		el->SetInnerRML(Rml::CreateString(20, "FPS: %f", fps));
	}
}