# 🎡 CPT205 — Computer Graphics (Coursework 2)
## 3D Interactive Scene in OpenGL (freeglut): House + Amusement Park + Snake Mini-Game 🌧️⚡🐍

This repository contains my **CPT205 Assessment 2 (3D Modelling Project)** submission: a fully interactive 3D scene built with **OpenGL + freeglut only** (no extra OpenGL libraries), featuring an explorable **house interior**, an outdoor **amusement-park area** with multiple animated rides, a playable **Snake mini-game embedded in the scene**, and a **dynamic sunny→rainy weather transition** with rain + lightning effects. ✨

The goal was not just “render some objects”, but to build a small **interactive world** that demonstrates core graphics techniques:
**hierarchical modelling, transformations, viewing/projection, lighting/materials, texture mapping, animation, and interaction**.

---

## 🌟 What’s inside (Scene Overview)

### 🏠 1) House Interior (walkable)
A furnished indoor environment with multiple textured objects and lighting cues:
- **Sofa** built via hierarchical modelling (cushions/armrests/details)
- **Cupboard / desk / floor lamp** with realistic proportions and textures
- A framed **photo** using texture mapping (*StarryNight*) for the picture surface
- A **door** that can be opened/closed interactively

### 🎮 2) “Game Console” + Embedded Snake Mini-Game
Inside the house, there is a **3D console** that acts like an interactive portal:
- Toggleable **spotlights** focused on the console to enhance visibility
- A pulsing “cool circle” glow under the console (time-based transparency/brightness)
- **Press a key near the console to enter Snake mode**, where you play:
  - grid + snake + food rendered in 3D
  - **fireworks particle effect** when food is eaten 🎆
  - extruded **3D text prompts** such as GAME OVER

### 🎡 3) Amusement Park Rides (animated)
Outside the house is an amusement park area with multiple moving attractions:

- **Ferris Wheel**
  - support structure + spokes + rings
  - cabins distributed around the rim
  - optional **Ferris wheel lighting system** for nighttime vibes 💡

- **Flying Chair Ride**
  - rotating structure with multi-axis animation
  - chairs swing outward with chain simulation
  - character can **ride it** and switch to a more immersive view

- **Roller Coaster**
  - track generated from control points and **Catmull-Rom spline interpolation**
  - track rendered with lighting normals and texture mapping
  - track includes diverse geometry: climb, spiral descent, undulating section, spiral ascent, and S-curve return

- **Drop Tower**
  - animated free-fall motion with acceleration/deceleration phases
  - decorative **dynamic lights** (HSV-based color changes) + glow
  - synchronized **particle effects** for a dramatic “ride moment” ✨

### ☁️ 4) Sky & Weather Transition (Sunny → Rainy) 🌧️⚡
- A **camera-centered textured sky sphere** for a seamless background
- Clouds composed of blended spheres with randomized attributes
- A gradual weather transition that triggers:
  - rain particle system
  - lightning bolt + lightning flash with temporary brightness boost

---

## 🎮 Controls (Keyboard + Mouse)

### Core navigation & camera
- **W / A / S / D** — move & rotate the character  
- **Hold Shift** — run faster 🏃
- **Space** — jump (when not already mid-jump)
- **O / P** — zoom camera in / out
- **Left Click + Drag** — rotate camera view around the character

### Interactions in the world
- **H** — toggle spotlights focused on the game console 💡
- **M** — open/close the door 🚪
- **T** — *when close to the console*, switch between:
  - main 3D scene ↔ Snake game mode

### Snake game controls (when in Snake mode)
- **Arrow keys** — control snake direction (↑ ↓ ← →)
- **R** — restart Snake if you are in GAME OVER
- **T** — return to main scene

### Outdoor ride controls
- **Y** — toggle Ferris wheel lights (when close enough)
- **X** — ride the flying chair (when within range), switching to a more immersive view

### Weather
- **B** — gradually fade the sky from sunny to rainy; triggers rain + lightning 🌧️⚡

### Safety / boundary behavior
- If the character walks outside the ground boundary, the scene simulates a **cliff fall**:
  the camera briefly shakes and the character position is reset.

---

## 🧠 Graphics Techniques Demonstrated (what this project showcases)

- **Hierarchical modelling** for complex objects (sofa, furniture, rides)
- **Transformations** (translation/rotation/scale) and multi-stage animation
- **Viewing & camera control** (orbit camera, zoom, perspective setup)
- **Lighting & materials**
  - global lighting setup
  - spotlights with cutoff angles and directional vectors
  - emissive/glow-like effects via blending
- **Texture mapping** (floors, walls, furniture, character textures, sky textures, etc.)
- **Spline-based geometry**
  - Catmull-Rom spline for smooth roller coaster track continuity
  - computed normals + twist for orientation and lighting
- **Particle systems**
  - rain particles
  - fireworks on Snake food consumption
- **Weather system**
  - textured sky sphere + cloud adaptation
  - lightning bolt + flash overlay effect

---

## 🗂️ Repository Contents
- `Code_Wu_Tonghui.cpp` — full OpenGL/freeglut implementation
- `Report_Wu_Tonghui-1.pdf` — technical report (features + controls + screenshots)
- `CPT205Ass2_2024-25.pdf` — official coursework specification
- `texture/` or `Texture/` — required texture image files (BMP)

> Note: some builds are Windows-oriented. Folder name casing (`texture` vs `Texture`) matters on Linux/macOS.

---

## 🛠️ Build & Run

### Option A — Run the executable (Windows)
If a prebuilt `*.exe` is included, you can run it directly (ensure the texture folder is next to it).

### Option B — Build from source (recommended)

#### Requirements
- C++ compiler (Visual Studio recommended)
- **OpenGL + GLU**
- **freeglut** *(required; no other OpenGL libraries)*

#### Visual Studio steps (typical)
1. Create a new C++ project and add `Code_Wu_Tonghui.cpp`
2. Configure include/lib paths for freeglut
3. Link libraries (commonly):
   - `opengl32.lib`
   - `glu32.lib`
   - `freeglut.lib` (or `freeglut_static.lib` depending on your setup)
4. Place the `texture/` (or `Texture/`) folder in the same directory as the executable
5. Build & run ✅

---

## 📸 Suggested Add-Ons for a Portfolio-Ready Repo
If you want this repo to “wow” people on first glance, add:
- `assets/overview.gif` — walking from house → rides
- `assets/snake.gif` — Snake gameplay + fireworks
- `assets/rain.gif` — sunny→rainy transition + lightning

Then embed in README:
```md
![Overview](assets/overview.gif)
![Snake](assets/snake.gif)
![Rain](assets/rain.gif)
