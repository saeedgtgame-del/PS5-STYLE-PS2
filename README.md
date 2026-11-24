# PS5-STYLE-PS2

A PS5-inspired dashboard homebrew project for the original PlayStation 2.

## Current prototype
- Native PS2 ELF project using PS2SDK, gsKit and libpad.
- D-pad and left analog stick navigation.
- Cross (X) selects a tile.
- GTA VI tile opens a visual-only loading demo; it does not run GTA VI.
- Other game tiles are information/preview placeholders and do not launch commercial games.
- Original abstract tile artwork; no copyrighted game cover images are bundled.

## Build
GitHub Actions builds the ELF on every push to main and when manually dispatched.

1. Open the Actions tab.
2. Select Build PS2 ELF.
3. Open the completed run.
4. Download the PS5-STYLE-PS2-ELF artifact.
5. Extract PS5_STYLE_MENU.ELF and test it with your PS2 homebrew setup.

The build uses the community PS2DEV toolchain container, ps2dev/ps2dev:latest.

## Controls
- D-pad / left analog stick: move selection
- X (Cross): select
- Select: return from demo screen

## Important limitation
A PS2 cannot run PS5 games or GTA VI. The GTA VI screen is only a menu/loading demo. This project is a dashboard-style homebrew app, not a PS5 emulator.
