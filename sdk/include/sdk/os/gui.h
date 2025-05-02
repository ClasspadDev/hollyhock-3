#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

enum GUI_Buttons {
	BUTTON_OK = 1 << 5,
	BUTTON_YES = 1 << 6,
	BUTTON_NO = 1 << 7,
	BUTTON_ABORT = 1 << 8,
	BUTTON_RETRY = 1 << 9,
	BUTTON_CANCEL = 1 << 10
};

/**
 * Displays a message box with the specified title and content, retrieved
 * through their ID in the string table.
 *
 * @param unknown An unknown parameter.
 * @param titleStringID The ID of the string to use for the message box's title.
 * @param contentStringID The ID of the string to use for the message box's
 * content.
 */
extern void (*GUI_DisplayMessageBox)(int unknown, unsigned int titleStringID, unsigned int contentStringID);

/**
 * Displays a message box with the specified title and content. A prefix to the
 * content may be specified, which is displayed before the main content.
 * 
 * The buttons to be displayed can be set using the @p buttons bitfield.
 * Possible values are defined by @ref GUI_Buttons, which can be
 * bitwise OR'd together to display multiple buttons. A maximum of 3 buttons can
 * be specified.
 * 
 * The true usage of the 4th bit of the @p buttons bitfield is unknown, however
 * it has been observed to shrink the black box which forms the title bar of the
 * message box.
 * 
 * Note: if no buttons are specified, and the close button is disabled, it is
 * impossible to exit the message box.
 * 
 * @param unknown An unknown parameter.
 * @param[in] titleString A string to use for the message box's title.
 * @param[in] contentPrefix A string to prefix the content with.
 * @param[in] contentString A string to use for the message box's content.
 * @param buttons A bitfield specifying which buttons to show.
 * @param disableCloseButton Set to @c true to disable the close button.
 * @return An unknown GUI struct.
 */
extern void *(*GUI_DisplayMessageBox_Internal)(
	int unknown,
	const char *titleString,
	const char *contentPrefix, const char *contentString,
	int buttons, bool disableCloseButton
)
#ifndef __clang__
__attribute__((
	null_terminated_string_arg(2), null_terminated_string_arg(3), null_terminated_string_arg(4),
	access(read_only, 2), access(read_only, 3), access(read_only, 4)
))
#endif
;

#ifdef __cplusplus
}
#endif
