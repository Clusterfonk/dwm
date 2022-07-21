# Customized dwm build
Configuration of [dwm](https://dwm.suckless.org/)

## Patches and Features
- [actual fullscreen](https://dwm.suckless.org/patches/actualfullscreen/)
- [alwayscenter](https://dwm.suckless.org/patches/alwayscenter/)
- [swallow](https://dwm.suckless.org/patches/swallow/)
- [systray](https://dwm.suckless.org/patches/systray/)
- [anybar](https://dwm.suckless.org/patches/anybar/)
- [cyclelayouts](https://dwm.suckless.org/patches/anybar/)
- [fullgaps](https://dwm.suckless.org/patches/fullgaps/)
- [rotatestack](https://dwm.suckless.org/patches/rotatestack/)
- [restartsig](https://dwm.suckless.org/patches/restartsig/)
- [uselessgap](https://dwm.suckless.org/patches/uselessgap/)

## Requirements
?

## Install
```bash
git clone https://github.com/Clusterfonk/dwm
cd dwm
sudo make install
```

## Maintain
New updates are pulled from the upstream
```bash
git remote add upstream https://git.suckless.org/dwm
git pull upstream master
```
Then the branch custom is rebased on top of the master
```bash
git rebase master
```
