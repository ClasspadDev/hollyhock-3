#pragma once
#include "util.hpp"

class GUIRadioButton : public GUIElement {
public:
	enum Flag : int {
		/// Causes the radio button to be selected by default.
		FlagSelected = 1 << 2,

		/// Makes the radio button interactive.
		FlagEnabled = 1 << 15
	};

	GUIRadioButton(
		int x, int y,
		const char *text,
		int flags
	);
};

