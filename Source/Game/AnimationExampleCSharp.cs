using System;
using System.Collections.Generic;
using FlaxEngine;
using RmlUi;

namespace Game;

/// <summary>
/// AnimationExampleCSharp Script.
/// </summary>
//[ExecuteInEditMode]
public class AnimationExampleCSharp : Script, IEquatable<AnimationExampleCSharp>
{
    private RmlUiDocument Document;
    private float t_prev_fade = 0.0f;
    private float t_prev = 0.0f;
    private int count_frames = 0;
    private bool run_loop = true;
    private bool single_loop = false;
    private int nudge = 0;
    private float ff = 0.0f;
    private string title = "Animation sample C#";

    /// <inheritdoc/>
    public override void OnStart()
    {
    }

    /// <inheritdoc/>
    public override void OnEnable()
    {
        Document = Actor.As<RmlUiDocument>();
        if (Document.LoadDocument())
        {
            Document.Focus();

            Document.GetElementById("title").SetInnerRML(title);

            // Button fun
            {
                var el = Document.GetElementById("start_game");
                var p1 = RmlUiTransform.MakeProperty(RmlUiTransforms.Rotate2D(10.0f), RmlUiTransforms.TranslateX(100.0f));
                var p2 = RmlUiTransform.MakeProperty(RmlUiTransforms.Scale2D(3.0f));
                el.Animate("transform", p1, 1.8f, new RmlUiTween(RmlUiTweenType.Elastic, RmlUiTweenDirection.InOut), -1, true);
                el.AddAnimationKey("transform", p2, 1.3f, new RmlUiTween(RmlUiTweenType.Elastic, RmlUiTweenDirection.InOut));
            }
            {
                var el = Document.GetElementById("high_scores");
                el.Animate("margin-left", new RmlUiProperty(0.0f, RmlUiUnit.Px), 0.3f, new RmlUiTween(RmlUiTweenType.Sine, RmlUiTweenDirection.In), 10, true, 1.0f);
                el.AddAnimationKey("margin-left", new RmlUiProperty(100.0f, RmlUiUnit.Px), 3.0f, new RmlUiTween(RmlUiTweenType.Circular, RmlUiTweenDirection.Out));
            }
            {
                var el = Document.GetElementById("options");
                el.Animate("image-color", new RmlUiProperty(new Color(128, 255, 255, 255), RmlUiUnit.Colour), 0.3f, new RmlUiTween(), -1, false);
                el.AddAnimationKey("image-color", new RmlUiProperty(new Color(128, 128, 255, 255), RmlUiUnit.Colour), 0.3f);
                el.AddAnimationKey("image-color", new RmlUiProperty(new Color(0, 128, 128, 255), RmlUiUnit.Colour), 0.3f);
                el.AddAnimationKey("image-color", new RmlUiProperty(new Color(64, 128, 255, 0), RmlUiUnit.Colour), 0.9f);
                el.AddAnimationKey("image-color", new RmlUiProperty(new Color(255, 255, 255, 255), RmlUiUnit.Colour), 0.3f);
            }
            {
                var el = Document.GetElementById("exit");
                RmlUiStyleSheetSpecification.TryParsePropertyDeclaration(out var pd, "transform", "translate(200px, 200px) rotate(1215deg)");
                el.Animate("transform", pd[RmlUiPropertyId.Transform], 3.0f, new RmlUiTween(RmlUiTweenType.Bounce, RmlUiTweenDirection.Out), -1);
            }

            // Transform tests

            {
                var el = Document.GetElementById("generic");
                var p = RmlUiTransform.MakeProperty(RmlUiTransforms.TranslateY(50, RmlUiUnit.Px), RmlUiTransforms.Rotate3D(0, 0, 1, -90, RmlUiUnit.Deg), RmlUiTransforms.ScaleY(0.8f));
                el.Animate("transform", p, 1.5f, new RmlUiTween(RmlUiTweenType.Sine, RmlUiTweenDirection.InOut), -1, true);
            }
            {
                var el = Document.GetElementById("combine");
                var p = RmlUiTransform.MakeProperty(RmlUiTransforms.Translate2D(50, 50, RmlUiUnit.Px), RmlUiTransforms.Rotate2D(1215));
                el.Animate("transform", p, 8.0f, new RmlUiTween(), -1, true);
            }
            {
                var el = Document.GetElementById("decomposition");
                var p = RmlUiTransform.MakeProperty(RmlUiTransforms.TranslateY(50, RmlUiUnit.Px), RmlUiTransforms.Rotate3D(0.8f, 0, 1, 110, RmlUiUnit.Deg));
                el.Animate("transform", p, 1.3f, new RmlUiTween(RmlUiTweenType.Quadratic, RmlUiTweenDirection.InOut), -1, true);
            }

            // Mixed units tests
            {
                var el = Document.GetElementById("abs_rel");
                el.Animate("margin-left", new RmlUiProperty(50.0f, RmlUiUnit.Percent), 1.5f, new RmlUiTween(), -1, true);
            }
            {
                var el = Document.GetElementById("abs_rel_transform");
                var p = RmlUiTransform.MakeProperty(RmlUiTransforms.TranslateX(0, RmlUiUnit.Px));
                el.Animate("transform", p, 1.5f, new RmlUiTween(), -1, true);
            }
            {
                var el = Document.GetElementById("animation_event");
                el.Animate("top", new RmlUiProperty(Random.Shared.NextSingle()*250.0f/*Rml::Math::RandomReal(250.0f)*/, RmlUiUnit.Px), 1.5f, new RmlUiTween(RmlUiTweenType.Cubic, RmlUiTweenDirection.InOut));
                el.Animate("left", new RmlUiProperty(Random.Shared.NextSingle()*250.0f/*Rml::Math::RandomReal(250.0f)*/, RmlUiUnit.Px), 1.5f, new RmlUiTween(RmlUiTweenType.Cubic, RmlUiTweenDirection.InOut));
            }

            Document.Show();
        }
    }

    /// <inheritdoc/>
    public override void OnDisable()
    {
    }

    /// <inheritdoc/>
    public override void OnUpdate()
    {
        if (!Document.IsLoaded())
            return;

        if (Time.TimeSinceStartup - t_prev_fade >= 1.4)
        {
            var el = Document.GetElementById("help");
            if (el.IsClassSet("fadeout"))
            {
                el.SetClass("fadeout", false);
                el.SetClass("fadein", true);
            }
            else if (el.IsClassSet("fadein"))
            {
                el.SetClass("fadein", false);
                el.SetClass("textalign", true);
            }
            else
            {
                el.SetClass("textalign", false);
                el.SetClass("fadeout", true);
            }

            t_prev_fade = Time.TimeSinceStartup;
        }

        float t = Time.TimeSinceStartup;
        float dt = t - t_prev;
        count_frames += 1;

        if (nudge != 0)
        {
            t_prev = t;
            ff += nudge * 0.3f;
            var el = Document.GetElementById("exit");
            el.SetProperty(RmlUiPropertyId.MarginLeft, new RmlUiProperty(ff, RmlUiUnit.Px));
            float f_left = el.GetAbsoluteLeft();
            //Rml::Log::Message(Rml::Log::LT_INFO, "margin-left: '%f'   abs: %f.", ff, f_left);
            nudge = 0;
        }

        if (dt > 0.2f)
        {
            t_prev = t;
            var el = Document.GetElementById("fps");
            float fps = count_frames / dt;
            count_frames = 0;
            el.SetInnerRML(String.Format("FPS: {0}", fps));
        }
    }

    public bool Equals(AnimationExampleCSharp other)
    {
        throw new NotImplementedException();
    }
}