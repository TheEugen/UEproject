# UEproject

An Unreal Engine 5 RPG prototype written in C++. The project explores core RPG systems from scratch: NPC AI with scheduled daily routines, a waynet-based pathfinding graph, a dialog system, an inventory with weapon equipping, and a data-driven NPC pipeline powered by XML. Also includes a formal EBNF grammar sketch for a custom in-game scripting language.

---

## Features

### Waynet & Pathfinding
NPCs navigate the world via a waynet — a graph of `Waypoint` actors connected by edges. The `WaynetHandler` implements multiple graph search algorithms for NPC pathfinding, allowing characters to route between named waypoints at runtime.

### NPC Daily Routines
Each NPC has a list of `DailyRoutine` entries. Every entry pairs an `IngameTime` (hour + minute) with a `DailyRoutineActivities` enum value and a target waypoint name. The AI evaluates the current in-game time and dispatches the NPC to the appropriate location and activity on schedule.

### XML-Driven NPC Data
NPC definitions live in `Data/DATA_NPC.xml` and are loaded at runtime via `XMLHandler`, which wraps Unreal's `FXmlFile`. For each NPC the file stores:

- **Attributes:** name, id, level, learning points, current/max HP, current/max mana
- **Spawnpoint:** name of the waypoint the NPC spawns at
- **Daily routines:** list of scheduled time/waypoint/activity entries

### Dialog System
`UDialog` holds an identifier, a title line displayed in the UI, and a sequential list of `UDialogInstruction` objects. Calling `start()` executes all instructions in order. `DialogOptionsWidget`, `DialogLineWidget`, and `SubtitleWidget` handle the in-world UI presentation.

### Inventory System
`UInventoryComponent` is an actor component attached to characters. It supports:

- **Fixed-size slot grid** with stackable and non-stackable item handling
- **Partial stacks** — new items fill existing partial stacks before creating new ones
- **Overflow splitting** — quantities exceeding `maxStackSize` are automatically split across multiple stacks
- **Weapon equipping** with three named attach points: `Right`, `Left`, `Back`
- **Toggleable inventory window** (`InventoryWindowWidget`) that switches input mode between UI and game

### Quest System
`QuestHandler`, `Quest`, `QuestSubpart`, and `QuestLogWidget` are scaffolded and ready for implementation.

### Custom Scripting Language (EBNF)
`ebnf_updated.g4` defines a formal grammar for an in-game scripting language named **RPG**. It covers:

- **Types:** `int`, `int64`, `uint`, `uint64`, `bool`, `char`, `wchar`, `string`
- **Control flow:** `if/else if/else`, `for`, `while`, `return`
- **Arithmetic:** addition, subtraction, multiplication, division, boolean flip
- **Game declarations:** `NPC { ... }` and `ITEM { ... }` blocks
- **Character set:** full Latin alphabet including German umlauts (`ä`, `ö`, `ü`, `ß`)

---

## Project Structure

```
UEproject/
├── Data/
│   └── DATA_NPC.xml          # NPC definitions (attributes, spawnpoints, routines)
├── AICharacter.cpp/.h         # AI character base class
├── RPGAIController.cpp/.h     # AI controller
├── NPCController.cpp/.h       # NPC-specific controller logic
├── NPCDataHandler.cpp/.h      # Bridges XMLHandler data to NPC actors
├── WaynetHandler.cpp/.h       # Waynet graph + pathfinding algorithms
├── Waypoint.cpp/.h            # Waypoint actor
├── DailyRoutine.cpp/.h        # Time-based NPC schedule entry
├── IngameTime.cpp/.h          # In-game clock (hour/minute)
├── Dialog.cpp/.h              # Dialog object (id, title, instructions)
├── DialogInstruction.cpp/.h   # Executable dialog instruction
├── DialogLineWidget.cpp/.h    # UI: single dialog line
├── DialogOptionsWidget.cpp/.h # UI: dialog choice list
├── SubtitleWidget.cpp/.h      # UI: subtitle/speech display
├── FocusWidget.cpp/.h         # UI: world-space focus indicator
├── FocusWidgetComponent.cpp/.h
├── InventoryComponent.cpp/.h  # Inventory actor component
├── InventorySlotWidget.cpp/.h
├── InventoryTooltipWidget.cpp/.h
├── InventoryWindowWidget.cpp/.h
├── Item.cpp/.h                # Base item class
├── Item_Weapon.cpp/.h         # Weapon item with attach points
├── Item_Miscellaneous.cpp/.h
├── Container.cpp/.h           # World container (chest, etc.)
├── Quest.cpp/.h
├── QuestSubpart.cpp/.h
├── QuestHandler.cpp/.h
├── QuestLogWidget.cpp/.h
├── XMLHandler.cpp/.h          # XML NPC data loader
├── RPGGameInstance.cpp/.h     # Game instance (persistent state)
├── RPGPlayerController.cpp/.h
├── RPG_cppCharacter.cpp/.h    # Player character
├── RPG_cppGameMode.cpp/.h
├── UI.cpp/.h                  # Main HUD widget
├── InteractInterface.cpp/.h   # Interface for interactable actors
├── RPG_cpp.Build.cs           # Unreal build rules
└── ebnf_updated.g4            # EBNF grammar for RPG scripting language
```

---

## Requirements

- Unreal Engine 5.0
- Visual Studio 2019 or later (Windows)
- No third-party plugins required — uses only built-in UE5 modules (`XmlParser`, `UMG`, `AIModule`, `NavigationSystem`)

## Building

1. Clone the repository.
2. Update the hardcoded XML path in `XMLHandler.cpp` to match your local project directory.
3. Right-click the `.uproject` file and select **Generate Visual Studio project files**.
4. Open the solution in Visual Studio and build.

---

## Notes

- The XML path in `XMLHandler.cpp` is currently hardcoded to `C:\dev\Unreal Projects\RPG_cpp 5.0\...` and must be updated before the NPC data pipeline will work.
- The quest system is scaffolded but not yet implemented.
- This is a prototype / learning project and does not represent a complete or shippable game.