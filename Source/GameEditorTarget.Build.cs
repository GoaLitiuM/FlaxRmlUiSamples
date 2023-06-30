using Flax.Build;

public class GameEditorTarget : GameProjectEditorTarget
{
    /// <inheritdoc />
    public override void Init()
    {
        base.Init();
		
		Architectures = new[]
        {
            TargetArchitecture.x64,
        };

        // Reference the modules for editor
        Modules.Add("Game");
    }
}
