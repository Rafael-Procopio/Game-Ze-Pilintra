# Zé Pilintra

A 2D RPG developed in C using the Raylib graphics library. The project combines classic RPG mechanics such as combat, character progression, inventory management, equipment systems, enemy waves, and educational Mathematical Logic challenges.

The main objective is to create an engaging gameplay experience while reinforcing logical reasoning through interactive challenges integrated into the progression system.

---

## Features

### Character Progression

* Experience (XP) system
* Leveling system
* Skill points
* Character stat upgrades

  * Attack
  * Defense
  * Maximum Health

### Combat System

* Melee combat
* Enemy collision damage
* Damage calculation based on player stats
* Attack hitbox system
* Debug hitbox visualization

### Inventory System

* Item collection
* Item storage
* Item management
* Consumable item usage

### Equipment System

* Weapons
* Armor
* Accessories
* Equip and unequip functionality
* Equipment stat bonuses

### Enemy System

* Multiple enemy types:

  * Goblin
  * Bandit
  * Skeleton
  * Elite
* Unique stats per enemy
* Enemy-specific loot tables

### Wave System

* Progressive enemy waves
* Difficulty scaling
* Wave announcements

### Logic Challenge System

* Mathematical Logic questions
* Tautology identification
* Contradiction identification
* Contingency identification
* XP rewards for correct answers
* Feedback for correct and incorrect responses

### Additional Features

* Fullscreen mode (F11)
* Controls panel
* God Mode for testing
* Camera system
* Drop notifications
* Equipment notifications
* Healing effects

---

## Technologies Used

* C
* Raylib
* GCC (MinGW)
* Git
* GitHub
* Visual Studio Code

---

## Installation

### Prerequisites

Before running the project, make sure you have:

* GCC (MinGW) installed
* Raylib installed and configured
* Git installed (optional)

### Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/ze-pilintra.git
cd ze-pilintra
```

### Compile the Project

```bash
gcc main.c game.c player.c map.c stats.c enemy.c combat.c inventory.c drops.c equipment.c items.c wave.c logic.c -o jogo.exe -lraylib -lopengl32 -lgdi32 -lwinmm
```

> If your project does not contain `items.c`, simply remove it from the command.

### Run the Game

```bash
jogo.exe
```

Or simply double-click the executable after compiling.

---

## How to Play

### Basic Controls

| Key   | Action               |
| ----- | -------------------- |
| A     | Move Left            |
| D     | Move Right           |
| SPACE | Jump                 |
| J     | Attack               |
| I     | Open Inventory       |
| E     | Open Equipment       |
| ENTER | Confirm Selection    |
| U     | Unequip Item         |
| ESC   | Return to Menu       |
| F11   | Toggle Fullscreen    |
| H     | Show / Hide Controls |

---

## Gameplay Loop

1. Defeat enemies.
2. Collect loot and equipment.
3. Gain experience and level up.
4. Upgrade your character's stats.
5. Survive increasingly difficult waves.
6. Complete logic challenges every few waves.
7. Earn bonus XP by answering correctly.

---

## Logic Challenge System

Every few waves, the player receives a Mathematical Logic challenge.

The objective is to classify a proposition as:

* Tautology
* Contradiction
* Contingency

Correct answers reward bonus experience points and accelerate character progression.

This system was designed to combine educational content with RPG gameplay.

---

## Debug Controls

| Key | Action            |
| --- | ----------------- |
| F1  | Add Potion        |
| F2  | Add Gold Coin     |
| F3  | Add Iron Sword    |
| F5  | Add Iron Sword    |
| F6  | Add Leather Armor |
| F7  | Add Lucky Ring    |
| F8  | Add Steel Sword   |
| F9  | Skip Current Wave |
| F10 | Toggle God Mode   |
| K   | Gain XP           |

---

## Project Structure

```text
src/
├── main.c
├── player.c
├── enemy.c
├── combat.c
├── inventory.c
├── equipment.c
├── drops.c
├── logic.c
├── wave.c
├── map.c
├── stats.c
└── game.c
```

---

## Current Version

**v0.1 - Playable Prototype**

Current systems available:

* Combat
* Character progression
* Equipment
* Inventory
* Enemy waves
* Loot drops
* Logic challenges
* UI improvements

---

## Future Improvements

* Character sprites and animations
* Enemy sprites
* Sound effects
* Background music
* NPC interactions
* Quest system
* Save/Load system
* Boss battles
* New maps
* Additional logic challenges
* Improved UI design

---

## Author

**Rafael José de Araújo Procópio da Cunha**

---

## Team

* Rafael José de Araújo Procópio da Cunha
