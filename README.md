# Signus: The Artefact Wars

Classic turn-based strategy war game similar to the old Battle Isle series. Commercially released in 1998, open-sourced by the original authors in 2002.

Earth sent a fleet of colony ships to a distant planet called Arkonis. When the fleet arrived, they found the planet inhabited by an alien race andvanced beyond human comprehension. All attempts to communicate with the aliens failed and the colonists had no choice but to settle down on the planet. Their ships could not make another interplanetary journey. So they attacked the peaceful aliens.

The war was short and one-sided. The aliens couldn't understand the concept of war or surrender and soon their entire race perished. Large areas of the planet were left uninhabitable for thousands of years due to radiation. When the colonists realized that there is nobody left to surrender, they landed on the planet and built a settlement.

A thousand years later, nobody remembers the Earth, aliens or the genocide. The human civilization on Arkonis just barely reached the technology level allowing space flight again and rediscovered the abandoned colonization fleet in orbit. When they learned of their early history from ancient records, six large companies founded the Consortium to explore the uninhabitable areas and search for alien artifacts. Toricon Corporation, the largest company on Arkonis, pushed for a ban on this exploration in the Planetary Council and created the Signus special forces to stop it at all costs. Arkonis is now on the brink of war between two corporate superpowers.

Good luck, commander, Signus is at your command.

## Dependencies

- libSDL 2.0.5
- SDL2\_mixer (built with libmikmod for music playback)
- SDL2\_ttf
- SDL2\_image
- libvorbis
- libjpeg
- libpng

## Installation

Clone this repository and build both `signus` and `signus-data` subdirectiories:

    cd signus
    ./bootstrap
    ./configure
    make
    make install

    cd signus-data
    ./bootstrap
    ./configure
    make
    make install

## Controls

- Left mouse button: Select unit, give orders
- Right mouse button: Change unit action

Note that some actions are only available on certain map fields. For example, you must hover mouse over a unit to select the rearm/refuel action.

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
- M: Mark current unit as finished until the end of turn and select next unfinished unit
- N: Select next unit
- 1: Change action to unit selection
- 2: Change action to movement
- 3: Change action to attack
- 4: Change action to unit special
