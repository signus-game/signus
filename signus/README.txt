Signus: The Artefact Wars
=========================

https://github.com/signus-game/signus

Signus is a classic Czech turn-based strategy war game similar to the old
Battle Isle series. The game was commercially released in 1998 and
open-sourced by the original authors in 2002.


Controls
--------

- Left mouse button: Select unit, give orders
- Right mouse button: Change unit action

Note that some actions are only available on certain map fields. For example,
you must hover mouse over a unit to select the rearm/refuel action.

- F1: Reserve time units for 1 shot
- F2: Reserve time units for 2 shots
- F3: Reserve time units for 3 shots
- F5: Open load game dialog
- F6: Open save game dialog
- F10: Open in-game menu
- Tab: Switch to next weapon
- Alt+Enter: Toggle fullscreen mode (scaled to current desktop resolution)
- C: Center screen on selected unit
- D: Toggle unit status bars
- E: End turn
- I: Open unit description
- M: Mark current unit as finished until the end of turn and select next
     unfinished unit
- N: Select next unit
- 1: Change action to unit selection
- 2: Change action to movement
- 3: Change action to attack
- 4: Change action to unit special


Game options
------------

Game options can be accessed from the sidebar menu or by pressing F10 while
playing a mission.

- Scrolling speed, Units movement speed, Units animation speed:
  These sliders control the speed of scrolling the map and animated unit
  actions. Higher value means faster movement. The maximum speed depends on
  CPU performance so you have to tune these values for best experience.

- Tooltip delay:
  This slider controls how quickly the mouse-over tooltip text appears after
  you hover the mouse over some object or button. Higher value means shorter
  delay.

- Full screen:
  Toggles full screen mode.

- Play intro on startup:
  Toggles the logo animations and first half of game intro which play every
  time you start Signus. The second half of intro will play when you start a
  new game regardless of this setting.

- Fast computer configuration:
  Toggles various UI animations. Purely cosmetic setting with no effect on
  gameplay.

- Units stop when new enemy unit spotted:
  Gameplay feature which makes map exploration slightly safer.


Feature options
---------------

The Features button in game options window opens another settings dialog where
you can choose gameplay options which were not available in the original
commercial release of the game. The settings are divided into bugfixes and
optional quality of life features.

Bugfixes correct minor bugs in game logic which were clearly unintentional but
fixing them changes the game experience compared to the original. Optional
quality of life features remove some of the most frustrating aspects of the
game and slightly reduce game difficulty but enabling them should make the
experience more enjoyable.

The recommended way to play is to enable all bugfixes and disable all optional
quality of life features (default). To play the game as close to the original
release as possible, disable all bugfixes and optional features in this
dialog.


Reporting bugs
--------------

To report a game bug, create new issue in the GitHub repository:
https://github.com/signus-game/signus/issues

If the game crashes while you play, you can attach the crash autosave in the
issue. You'll find it in the Signus config directory:
- On Windows: %APPDATA%\signus\crashguard_saved_state
- On Linux: ~/.signus/crashguard_saved_state

Note that when you start Signus again after crash and choose to resume
previous game or go to main menu, the crash autosave will be deleted (unless
resuming the game results in another crash). When Signus asks you whether to
resume a crashed game on startup and you want to file a bugreport, choose
"Exit game" to preserve the crash autosave.
