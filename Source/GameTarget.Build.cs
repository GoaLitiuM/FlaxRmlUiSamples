using Flax.Build;

public class GameTarget : GameProjectTarget
{
    /// <inheritdoc />
    public override void Init()
    {
        base.Init();
		
		Architectures = new[]
        {
            TargetArchitecture.x64,
        };

        // Reference the modules for game
        Modules.Add("Game");
    }
}
