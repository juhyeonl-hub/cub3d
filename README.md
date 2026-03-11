# cub3D

![Language](https://img.shields.io/badge/Language-C-blue)
![School](https://img.shields.io/badge/School-Hive%20Helsinki-green)
![Type](https://img.shields.io/badge/Type-Graphics-red)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen)

A raycasting engine inspired by **Wolfenstein 3D**, built as a team project at [Hive Helsinki](https://www.hive.fi/) (42 Network). Renders a 2D map into a 3D first-person perspective using the **DDA algorithm** and the MiniLibX graphics library.

---

## Demo

```bash
./cub3D maps/test.cub
```

```
  ╔══════════════════════════════════════════════╗
  ║░░░░░░░░░░░░░░░ Sky (Ceiling) ░░░░░░░░░░░░░░░║
  ║░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░║
  ║                                              ║
  ║      ████  ████████████████████  ████        ║
  ║      ████  ██              ████  ████        ║
  ║  ████████  ██    ████      ████  ████████    ║
  ║  ████████  ██    ████      ████  ████████    ║
  ║  ████████  ██    ████      ████  ████████    ║
  ║  ████████  ██              ████  ████████    ║
  ║      ████  ████████████████████  ████        ║
  ║      ████  ████████████████████  ████        ║
  ║                                              ║
  ║▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ Floor ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓║
  ║▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓║
  ╚══════════════════════════════════════════════╝

  Controls:  W/A/S/D = Move  |  ←/→ = Rotate  |  ESC = Quit
```

---

## DDA Raycasting Algorithm

For each vertical column of the screen, a ray is cast from the player position. The **DDA (Digital Differential Analyzer)** steps through the grid, checking which wall the ray hits first:

```
    0   1   2   3   4   5
  ┌───┬───┬───┬───┬───┬───┐
0 │   │   │   │   │███│███│
  ├───┼───┼───┼───┼───┼───┤
1 │   │   │   │ / │   │███│  █ = Wall
  ├───┼───┼───┼/──┼───┼───┤  / = Ray path
2 │   │   │ / │   │   │   │  P = Player
  ├───┼───┼/──┼───┼───┼───┤  * = Hit point
3 │   │ / │   │   │   │   │
  ├───┼/──┼───┼───┼───┼───┤  The ray steps through grid
4 │ P │   │   │   │   │   │  cells, checking x-side and
  ├───┼───┼───┼───┼───┼───┤  y-side intersections at each
5 │   │   │   │   │   │   │  step until it hits a wall.
  └───┴───┴───┴───┴───┴───┘

  Step 1: Check next x-side vs next y-side distance
  Step 2: Advance to whichever is closer
  Step 3: Repeat until wall hit
  Step 4: Calculate perpendicular distance (fixes fisheye)
  Step 5: Draw wall column height = screen_height / distance
```

### Fisheye Correction

```
  Without correction:          With correction:
  ╭────────────────╮          ┌────────────────┐
  │  /          \  │          │  |          |  │
  │ /            \ │          │  |          |  │
  │|              |│          │  |          |  │
  │ \            / │          │  |          |  │
  │  \          /  │          │  |          |  │
  ╰────────────────╯          └────────────────┘
  Walls curve outward          Walls render straight

  Fix: Use perpendicular distance to camera plane,
       not Euclidean distance to player position.
```

---

## Map Format (.cub)

```
NO textures/north.png
SO textures/south.png
WE textures/west.png
EA textures/east.png

F 220,100,0
C 135,206,235

11111111111111111111111111111
10000000001100000000000000001
10110000011100000001111011111
11111111110000000000000000011111
        10110000011100000000000111111
        1000000000110000011111111111
11111111111111011100000010001
11110111111111011101010010001
11000000110101011100000010001
10000000000000001100000010001
10000000000000001100010000001
11000000110101000000000000001
11110111110101000000000000001
10000000001100000100000010001
10000000000000000000000001001
10000000111100010000N00001001
100000001  100110101110000001
10000000111110000001 10100001
11111111111111111111 11111111
```

| Element | Description |
|---------|-------------|
| `NO/SO/WE/EA` | Texture path for each wall direction |
| `F` | Floor color (R,G,B) |
| `C` | Ceiling color (R,G,B) |
| `0` | Empty walkable space |
| `1` | Wall |
| `N/S/E/W` | Player start position & facing direction |
| ` ` (space) | Void (outside map) |

The map must be fully enclosed by walls — validated via **flood fill** from each open space.

---

## Features

| Category | Details |
|---|---|
| **Raycasting** | DDA algorithm, one ray per screen column |
| **Fisheye Correction** | Perpendicular distance to camera plane |
| **Textures** | Direction-dependent wall textures (N/S/E/W) |
| **Floor & Ceiling** | Configurable RGB color rendering |
| **Movement** | `W`/`A`/`S`/`D` move, `←`/`→` rotate, `ESC` quit |
| **Map Parsing** | Full `.cub` file validation with flood fill |

---

## Build & Run

### Prerequisites

- GCC or Clang
- GNU Make
- MiniLibX (included or system-installed)
- X11 development libraries (`libx11-dev`, `libxext-dev`)

### Build

```bash
git clone https://github.com/Hyeon-coder/cub3d.git
cd cub3d
make
```

### Run

```bash
./cub3D maps/test.cub
```

### Clean

```bash
make clean    # Remove object files
make fclean   # Remove object files and binary
make re       # Rebuild from scratch
```

---

## Key Challenges & What I Learned

### 1. Understanding the DDA Algorithm
Translating the math into code was the hardest part — calculating `deltaDistX`/`deltaDistY` and stepping through the grid to find the exact wall hit point. Once the grid-stepping logic clicked, it became elegant: just compare two distances at each step and advance the shorter one.

### 2. Fisheye Distortion Correction
Using the raw Euclidean distance from the player to the wall makes edges of the screen appear curved. The fix: project the distance onto the **camera direction vector** to get the perpendicular distance. A single `cos()` multiplication eliminates the distortion entirely.

### 3. Map Validation with Flood Fill
The map can have irregular shapes and spaces. A flood fill starting from each `0` cell checks whether the map is fully enclosed — if the fill reaches the map boundary or a space character, the map is invalid. Edge cases like maps with internal voids and non-rectangular shapes required careful boundary checking.

### 4. Texture Coordinate Mapping
When a ray hits a wall, the exact x-coordinate on the texture depends on **which face** was hit (N/S vs E/W) and where along that face the ray landed. Getting this wrong produces stretched or misaligned textures — subtle bugs that required careful debugging with known test maps.

---

## License

This project was developed as part of the 42 curriculum at Hive Helsinki.
