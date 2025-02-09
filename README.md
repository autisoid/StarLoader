# StarLoader
This is a .DLL file which adds .ASI file loading capability to (all) GoldSrc games.

# Why (all)?
Well, I'm unsure about ALL games support. So far I've tested Sven Co-op and the original game, it works there very well.

# HOW TO BUILD
Just copy the repository onto your drive, open up `StarLoader\StarLoader.sln` file with MSVS 2015/2017/2019/2022 and hit Ctrl+Shift+B.

The artifact will be in `StarLoader\Debug` or `StarLoader\Release` folder.

Rename `StarLoader.dll` into `hid.dll` and follow `HOW TO INSTALL`

# HOW TO INSTALL
Either follow `HOW TO BUILD` to build an artifact, or download one from Releases.

Drop the `hid.dll` file into game ROOT folder (e.g. `C:\Program Files (x86)\Steam\steamapps\common\Sven Co-op\`) and launch the game at least once.

A folder named `starloader` will appear in the game ROOT folder. Drop any needed .ASI plugins there, they'll be loaded upon game start/restart.