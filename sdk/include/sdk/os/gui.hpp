/**
 * @file
 * @brief Functions and classes to create/display GUI elements.
 *
 * Example: display a simple dialog with a label
 * @code{cpp}
 * GUIDialog dialog(
 *     GUIDialog::Height25, GUIDialog::AlignTop,
 *     "Dialog Title",
 *     GUIDialog::KeyboardStateABC
 * );
 * 
 * GUILabel label(
 *     dialog.GetLeftX() + 10,
 *     dialog.GetTopY() + 10,
 *     "Label Text"
 * );
 * dialog.AddElement(label);
 * 
 * dialog.ShowDialog();
 * @endcode
 */

#pragma once

#include "gui.h"

#include "gui/button.hpp"
#include "gui/dialog.hpp"
#include "gui/dropDown.hpp"
#include "gui/label.hpp"
#include "gui/longLabel.hpp"
#include "gui/radioButton.hpp"
#include "gui/textBox.hpp"