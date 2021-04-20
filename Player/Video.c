#include "Video.h"

void write_frame(Video* vid){
	memcpy(REAL_SCREEN_BASE_ADDRESS,vid->Frame,sizeof(short)*PIXELS);
}

unsigned int load_frame(short frame, Video* vid){
	fseek(vid->File,vid->frame_offsets[frame],SEEK_SET);
	return fread(vid->frame_encoded,1,ENCODEDBUFFERCHACHE,vid->File);
}

int init_Video(char* path, Video* vid){
	vid->File = fopen(path,"rb");
	if(!vid->File)
		return -1;
	vid->current_frame=0;
	vid->frame_offsets=calloc(sizeof(int),FRAMES+1);
	if(!vid->File)
		return -2;
	vid->frame_encoded=calloc(1,ENCODEDBUFFERCHACHE+1);
	if(!vid->File)
		return -2;
	vid->Frame=calloc(sizeof(short),PIXELS+1);
	short frame=0;
	int count=0;
	int offset=1;
	int length=length = fread(vid->frame_encoded,1,ENCODEDBUFFERCHACHE,vid->File);
	short i=0;
	while(length){
		
		for(i=0; i<length; i++){

			count+=vid->frame_encoded[i];
			if(count>PIXELS){
				return 3;
			}
			if(count==PIXELS){
				frame++;
				vid->frame_offsets[frame]=offset+i;
				count=0;
			}
		}
		offset+=i;
		length = fread(vid->frame_encoded,1,ENCODEDBUFFERCHACHE,vid->File);
		if(!(frame%100))
			nio_fprintf(&console,"%f%%\n",frame*100.0/FRAMES);
		ESC_Break
	}
	vid->frame_offsets[++frame]=offset;
	return FRAMES-frame;
}

char decode_frame(Video* vid){
	int count = 0;
	short offset=0;
	unsigned char pix;
	while(count<PIXELS){
		pix=vid->frame_encoded[offset];
		for(int i=0; i<pix; i++){
			vid->Frame[count]=(offset%2)*65535;
			count++;
		}
		offset++;
	}
	return 0;
}

void Player(Video* vid){
	lcd_init(SCR_320x240_16);
	char autoplay=0;
	int old_frame=-1;
	while(1){
		if(autoplay){
			vid->current_frame++;
			if (vid->current_frame>=FRAMES)
				vid->current_frame=0;
		}

		if(old_frame!=vid->current_frame){
			load_frame(vid->current_frame,vid);
			decode_frame(vid);
			write_frame(vid);
			old_frame=vid->current_frame;
		}
		if(isKeyPressed(KEY_NSPIRE_PLUS)){
			vid->current_frame++;
			if (vid->current_frame>=FRAMES)
				vid->current_frame=0;
			wait_no_key_pressed();
		}
		if(isKeyPressed(KEY_NSPIRE_MINUS)){
			vid->current_frame--;
			if (vid->current_frame<0)
				vid->current_frame=FRAMES-1;
			wait_no_key_pressed();
		}
		if(isKeyPressed(KEY_NSPIRE_ENTER)){
			autoplay=!autoplay;
			wait_no_key_pressed();
		}
		if(isKeyPressed(KEY_NSPIRE_0))
			vid->current_frame=0;
		ESC_Break
	}
}

void closevid(Video *vid){
	free(vid->frame_offsets);
	free(vid->frame_encoded);
	free(vid->Frame);
	fclose(vid->File);
	vid->current_frame=-1;
}