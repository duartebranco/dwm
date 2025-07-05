# duarte’s dwm build

This is my personal build of dwm with several patches and customizations applied, was forked from Luke Smith's build.

You probably should make your own builds, since it's in the "suckless" mindset to know your programs and have only exactly what you want in them.

## Patches/Changes Included


- **Vanitygaps**: Adjustable gaps between windows for a modern tiling look.
- **Multiple layouts**: Includes tile, bstack, monocle, spiral, centered master, grid, nrowgrid, and floating.
- **Xresources support**: Colors and some settings can be loaded at startup from Xresources.
- **Simple tagging**: Uses 5 default tags (workspaces) for window organization.
- **Streamlined keybindings**: Focus on essential tiling, launching terminal, browser, and dmenu.
- **Status bar**: Minimal setup, compatible with dwmblocks (but less interactive than Luke's).
- **No scratchpads, swallow, or sticky windows**: Only core tiling window management features.

*(See the source and config.h for details.)*
## Compilation

To build and install dwm, run:

```sh
make clean install
```

You might need to use `sudo` for installation.
