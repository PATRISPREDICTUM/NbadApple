#include <os.h>
#include <nspireio/nspireio.h>
#include "Video.h"

int main(void) {
	// CX tested!
	if (!has_colors)
		return 0;
	Video vid;
    nio_init(&console, NIO_MAX_COLS, NIO_MAX_ROWS, 0, 0, NIO_COLOR_BLACK, NIO_COLOR_WHITE, TRUE);
	nio_fprintf(&console,"Loading Data!\n");
	int error=init_Video("frames.tns",&vid);
	if(!error){
		nio_free(&console);
		Player(&vid);
		closevid(&vid);
		return 0;
	}
	nio_fprintf(&console,"Error %d\n",error);
	closevid(&vid);
	nio_free(&console);
	wait_key_pressed();
	return 0;
}