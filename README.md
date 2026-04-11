# 🔦 Ray Optics Simulator (SFML → Raylib → Web)

> ⚠️ This README was generated with the help of AI.

---

## 📌 Overview

This project is a **2D Ray Optics Simulator** built using C++.
It simulates how light rays interact with mirrors using basic physics concepts like:

* Ray direction & movement
* Line intersection
* Surface normals
* Reflection (vector math)

The project started with **SFML**, then transitioned to **raylib**, and is now being prepared for **web deployment using Emscripten**.

---

## 🚀 Evolution of the Project

### 🧩 Phase 1 — SFML (Initial Build)

* Implemented ray movement using `sf::Vector2f`
* Learned:

  * Vector normalization
  * Direction × speed = movement
  * Rendering lines using `sf::VertexArray`
* Issues faced:

  * Frame updates tied to events
  * Limited control over performance

---

### 🔄 Phase 2 — Raylib (Rewrite)

Switched to raylib for:

* Simpler API
* Better real-time rendering loop
* Easier input handling

#### Improvements:

* Continuous rendering (`BeginDrawing()` loop)
* Cleaner input system (`IsMouseButtonDown`, `IsKeyPressed`)
* Lightweight and faster than SFML

---

### 💡 Core Concepts Implemented

#### 1. Ray Movement

```text
position += direction × speed
```

#### 2. Line Intersection

Used parametric line equations with `t` and `u`:

* Detects if a ray intersects a mirror segment
* Ensures intersection happens within segment bounds

#### 3. Surface Normal

For a mirror edge:

```text
normal = (-edge.y, edge.x)
```

#### 4. Reflection Formula

```text
R = D - 2(D · N)N
```

Where:

* `D` = incoming direction
* `N` = normalized normal
* `R` = reflected direction

---

### ⚠️ Problems Solved During Development

* ❌ Rays passing through mirrors (tunneling)
* ❌ Incorrect reflections (wrong mirror detection)
* ❌ Double movement per frame
* ❌ Unstable collision detection

#### ✅ Final Fixes:

* Predict next position before moving
* Use **current → next** segment for intersection
* Choose **closest intersection (smallest t)**
* Offset hit point slightly to prevent sticking

---

## 🖥️ Controls

| Action     | Input            |
| ---------- | ---------------- |
| Add mirror | Left Click       |
| Emit rays  | Hold Right Click |

---

## 🌐 Phase 3 — Web (Emscripten)

The project is being prepared to run in the browser using:

* **Emscripten**
* **WebAssembly (WASM)**
* **raylib WebGL backend**

### Why Web?

* Shareable demo
* Interactive learning tool
* Can be embedded in educational platforms

---

## 🧠 What This Project Teaches

* Vector mathematics in graphics
* Collision detection (line-line intersection)
* Reflection physics
* Game/render loops
* Transitioning between frameworks (SFML → raylib)
* Preparing native C++ apps for the web

---

## 📁 Project Structure

```text
project/
│
├── main.cpp
│
├── index.html
├── index.js
├── index.wasm
│
└── README.md
```

---

## ⚙️ Build (Raylib - Desktop)

Example (MinGW):

```bash
g++ main.cpp -o main.exe -lraylib -lopengl32 -lgdi32 -lwinmm
```

---

## 🌍 Build (Web - Emscripten)

```bash
emcc src/main.c -o build/index.html \
    -I path/to/raylib/src \
    -L path/to/raylib/src \
    -lraylib \
    -s USE_GLFW=3 \
    -s ASYNCIFY
```

---

## 🔮 Future Improvements

* 🔁 Laser mode (single ray with multiple reflections)
* 🧊 Refraction (glass simulation)
* 🎯 Mirror rotation
* 🎮 Interactive UI controls
* 📱 Mobile compatibility (web)

---

## 🙌 Credits

* Built using **raylib**
* Inspired by basic ray tracing and optics simulations
* README generated with AI assistance

---
