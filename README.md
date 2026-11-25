# ORBIT GAME STORE (PS2)

A native PlayStation 2 homebrew store interface, developed inside the existing PS5-STYLE-PS2 repository.

## Branding
The in-console store name is **ORBIT GAME STORE**. It is intentionally independent of the developer's personal name.

## Current milestone
- Native PS2 ELF built with PS2SDK, gsKit and libpad.
- Controller navigation and a game-details page.
- Dark store interface with red accents.
- Catalog entries are clearly marked as placeholders.
- No fake download progress: network and downloads remain disabled until the network client is integrated and tested.

## Planned milestones
1. Integrate the PS2DEV network stack (DEV9/SMAP, NETMAN, PS2IP) and verify DHCP/DNS on the target console/network adapter.
2. Add a catalog/update service and versioned manifest.
3. Add PS2-friendly cover thumbnails and a versioned catalog.
4. Add downloads only for homebrew, public-domain, or otherwise legally distributable files. Show real byte counts, speed, percentage, and errors.
5. Add safe resume/retry and storage-space checks.

## Build
GitHub Actions builds the ELF on pushes to main and on manual dispatch. Open Actions, select Build PS2 ELF, then download the ORBIT-GAME-STORE-PS2-ELF artifact.

## Install in OPL
The repository includes install/OPL/conf_apps.cfg and install/OPL/APPS/ORBITSTORE/title.cfg examples. Copy the built ELF to mass:/APPS/ORBITSTORE/ORBIT_STORE.ELF. Place title.cfg beside the ELF. Enable Applications start mode in OPL. A Free McBoot shortcut can point to the same ELF path.

## Important
- This is a PS2 homebrew application, not a PS5 emulator.
- Network support is not yet active in this milestone.
- The catalog must not point to unauthorized copies of commercial games. Downloads are intended for homebrew and legally distributable content.