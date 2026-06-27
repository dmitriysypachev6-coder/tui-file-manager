[🇬🇧 English](./README.md) | [🇷🇺 Русский](./README_ru.md)
# TUI File Manager

A simple terminal-based file manager with keyboard control.  
Written in C++ using the **ncurses** library.

---

## Features

- Folder navigation (arrow keys up/down, enter/exit)
- Create files and folders
- Delete files and folders (with confirmation)
- One-handed control (keys within reach)
- UTF‑8 support (Cyrillic in filenames)

---

## Installation

### Dependencies

| Distribution | Command |
|--------------|---------|
| Debian / Ubuntu | `sudo apt install libncurses-dev` |
| Fedora | `sudo dnf install ncurses-devel` |
| Arch / CachyOS | `sudo pacman -S ncurses` |

### Build

```bash
g++ fm_beta_v0.2.cc -o filemanager -lncursesw
```

### Run

```
./filemanager
```

## Controls

| Key | Action |
|---|---|
| `↑` / `↓` | Move through the list |
| `→` | Enter folder |
| `←` | Go to parent folder |
| `Ins` | Create a new file |
| `/` | Create a new folder |
| `Del` | Delete selected item |
| `Ctrl+C` | Exit program |

> Exit via `END` command is not yet implemented – use `Ctrl+C`.

## Project Structure

```
tui-file-manager/
├── fm_beta_v0.1.cc      # First version (view only)
├── fm_beta_v0.2.cc      # Current version (create/delete)
├── LICENSE              # GPL-3.0 License
└── README.md            # This file
```

## Roadmap

- Open `.txt` in `nano` on `Enter`
- Support `END` command to exit
- Error handling for inaccessible folders
- Interface localization (RU/EN)
- Color scheme

## License

**GNU General Public License v3.0**
This is free software. You can redistribute and/or modify it under the terms of GPL-3.0.
The full license text is in the `LICENSE` file.

