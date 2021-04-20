#pragma once
#include <os.h>

#define ESC_Break\
	if(isKeyPressed(KEY_NSPIRE_ESC)){ \
		wait_no_key_pressed();\
		break;\
}
