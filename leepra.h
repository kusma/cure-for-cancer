/*
	Lib for Eventyrlig og Ekstatisk Pixelbasert Realtime Animasjon
	aka LEEPRA.
*/

int leepra_open( char* title, int width, int height, bool fullscreen );
void leepra_update( void* data );
void leepra_close();
