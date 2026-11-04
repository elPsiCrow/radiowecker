// Board-specific wiring of drivers. Each hardware/<board>/ folder provides its own implementation.
#pragma once

class UiManager;

// Registers the displays and inputs that exist on this board; called before UiManager::begin().
void boardSetup(UiManager& ui);
