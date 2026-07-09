# duarte’s dwm build

This is my personal build of dwm with several patches and customizations applied, was forked from Luke Smith's build.

You probably should make your own builds, since it's in the "suckless" mindset to know your programs and have only exactly what you want in them.

## Patches/Changes Included

- **Vanitygaps** - per-side gap control with toggle (`Mod+Alt+0`), individual inner/outer/horizontal/vertical gap adjustments.
- **Scratchpad** - a persistent floating terminal on a hidden tag (`Mod+n` to toggle, `Mod+Shift+n` to send window to scratchpad).
- **Adjacent tag navigation** - `Mod+Left/Right` cycles through occupied tags; `Mod+Shift+Left/Right` moves the focused window to the adjacent tag.
- **Hide vacant tags** - only tags with windows or the current tag are drawn in the bar, saving space.
- **Color schemes** - extended status bar colors: `SchemeStatus`, `SchemeTagsSel/Norm`, `SchemeInfoSel/Norm` for granular bar styling.
- **9 tags** - expanded from the default 5.
- **Custom `config.mk`** - install prefix set to `$(HOME)/.local` instead of `/usr/local`.
- **Disable resize hints** - `resizehints = 0` for more predictable tiling.
- **Shorter setmfact steps** - `0.02` instead of `0.05`.
- **Custom scripts** - bindings for dmenu-drun, dmenu-scripts, power menu, VM selector, recording script, wallpaper switcher, music (ncmpcpp), file manager (ranger), and browser.
- **Screenshot to clipboard** - `Print` captures selection, `Alt+Print` captures fullscreen, both copy to clipboard via `xclip`.
- **Hardware keybindings** - volume/brightness via `Mod+F1-F6` and `XF86XK_*` keys, `PowerOff` key launches shutdown menu.

*(See the source and config.h for details.)*
## Compilation

To build and install dwm, run:

```sh
make clean install
```

You might need to use `sudo` for installation.
