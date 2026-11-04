// Builds the display-independent view-model from the application state.
#pragma once

#include "ui/view_model.h"

// Fills everything except `revision`, which the UI manager owns.
void buildViewModel(ViewModel& vm);
