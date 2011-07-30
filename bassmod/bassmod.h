/* BASSMOD 1.3 C/C++ header file, copyright (c) 1999-2001 Ian Luck.
   Please report bugs/suggestions/etc... to bassmod@un4seen.com */

#ifndef BASSMOD_H
#define BASSMOD_H

#ifdef WIN32
#include <wtypes.h>
#else
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define CALLBACK
#define LOWORD(a) ((a)&0xffff)
#define HIWORD(a) ((a)>>16)
#define MAKELONG(a,b) (DWORD)(((a)&0xffff)|((b)<<16))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSDEF
#ifdef WIN32
#define BASSDEF(f) WINAPI f
#else
#define BASSDEF(f) f
#endif
#endif

typedef DWORD HSYNC;		// synchronizer handle

// Error codes returned by BASSMOD_GetErrorCode()
#define BASS_OK				0	// all is OK
#define BASS_ERROR_MEM		1	// memory error
#define BASS_ERROR_FILEOPEN	2	// can't open the file
#define BASS_ERROR_DRIVER	3	// can't find a free/valid driver
#define BASS_ERROR_HANDLE	5	// invalid handle
#define BASS_ERROR_FORMAT	6	// unsupported format
#define BASS_ERROR_POSITION	7	// invalid playback position
#define BASS_ERROR_INIT		8	// BASS_Init has not been successfully called
#define BASS_ERROR_ALREADY	14	// already initialized/loaded
#define BASS_ERROR_ILLTYPE	19	// an illegal type was specified
#define BASS_ERROR_ILLPARAM	20	// an illegal parameter was specified
#define BASS_ERROR_DEVICE	23	// illegal device number
#define BASS_ERROR_NOPLAY	24	// not playing
#define BASS_ERROR_NOMUSIC	28	// no MOD music has been loaded
#define BASS_ERROR_UNKNOWN	-1	// some other mystery error

// Device setup flags
#define BASS_DEVICE_8BITS	1	// use 8 bit resolution, else 16 bit
#define BASS_DEVICE_MONO	2	// use mono, else stereo
#define BASS_DEVICE_NOSYNC	16	// disable synchronizers
#define BASS_DEVICE_LEAVEVOL	32	// leave the volume as it is

// Music flags
#define BASS_MUSIC_RAMP		1	// normal ramping
#define BASS_MUSIC_RAMPS	2	// sensitive ramping
/* Ramping doesn't take a lot of extra processing and improves
the sound quality by removing "clicks". Sensitive ramping will
leave sharp attacked samples, unlike normal ramping. */
#define BASS_MUSIC_LOOP		4	// loop music
#define BASS_MUSIC_FT2MOD	16	// play .MOD as FastTracker 2 does
#define BASS_MUSIC_PT1MOD	32	// play .MOD as ProTracker 1 does
#define BASS_MUSIC_POSRESET	256	// stop all notes when moving position
#define BASS_MUSIC_SURROUND	512	// surround sound
#define BASS_MUSIC_SURROUND2	1024	// surround sound (mode 2)
#define BASS_MUSIC_STOPBACK	2048	// stop the music on a backwards jump effect
#define BASS_MUSIC_CALCLEN	8192	// calculate playback length

/* Sync types (with BASSMOD_MusicSetSync() "param" and SYNCPROC "data"
definitions) & flags. */
#define BASS_SYNC_MUSICPOS	0
#define BASS_SYNC_POS		0
/* Sync when the music reaches a position.
param: LOWORD=order (0=first, -1=all) HIWORD=row (0=first, -1=all)
data : LOWORD=order HIWORD=row */
#define BASS_SYNC_MUSICINST	1
/* Sync when an instrument (sample for the non-instrument based formats)
is played in the music (not including retrigs).
param: LOWORD=instrument (1=first) HIWORD=note (0=c0...119=b9, -1=all)
data : LOWORD=note HIWORD=volume (0-64) */
#define BASS_SYNC_END		2
/* Sync when the music reaches the end.
param: not used
data : 1 = the sync is triggered by a backward jump, otherwise not used */
#define BASS_SYNC_MUSICFX	3
/* Sync when the "sync" effect (XM/MTM/MOD: E8x/Wxx, IT/S3M: S2x) is used.
param: 0:data=pos, 1:data="x" value
data : param=0: LOWORD=order HIWORD=row, param=1: "x" value */
#define BASS_SYNC_ONETIME	0x80000000	// FLAG: sync only once, else continuously

typedef void (CALLBACK SYNCPROC)(HSYNC handle, DWORD data, DWORD user);
/* Sync callback function. NOTE: a sync callback function should be very
quick (eg. just posting a message) as other syncs cannot be processed
until it has finished.
handle : The sync that has occured
data   : Additional data associated with the sync's occurance */

DWORD BASSDEF(BASSMOD_GetVersion)();
/* Retrieve the version number of BASSMOD that is loaded.
RETURN : The BASSMOD version (LOWORD.HIWORD) */

BOOL BASSDEF(BASSMOD_GetDeviceDescription)(int devnum, char **desc);
/* Get the text description of a device. This function can be used to
enumerate the available devices.
devnum : The device (0=first)
desc   : Pointer to store pointer to text description at */

DWORD BASSDEF(BASSMOD_ErrorGetCode)();
/* Get the BASS_ERROR_xxx error code. Use this function to get the
reason for an error. */


BOOL BASSDEF(BASSMOD_Init)(int device, DWORD freq, DWORD flags);
/* Initialize the digital output. This must be called before all
following BASSMOD functions.
device : Device to use (0=first, -1=default, -2=no sound)
freq   : Output sample rate
flags  : BASS_DEVICE_xxx flags
*/

void BASSDEF(BASSMOD_Free)();
/* Free all resources used by the digital output, including the MOD music. */

float BASSDEF(BASSMOD_GetCPU)();
/* Get the current CPU usage of BASSMOD to mix the MOD music.
RETURN : The CPU usage percentage */

BOOL BASSDEF(BASSMOD_SetVolume)(DWORD volume);
/* Set the digital output master volume.
volume : Desired volume level (0-100) */

int BASSDEF(BASSMOD_GetVolume)();
/* Get the digital output master volume.
RETURN : The volume level (0-100, -1=error) */


BOOL BASSDEF(BASSMOD_MusicLoad)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags);
/* Load a music (XM/MOD/S3M/IT/MTM). The amplification and pan
seperation are initially set to 50, use BASSMOD_MusicSetAmplify()
and BASSMOD_MusicSetPanSep() to adjust them.
mem    : TRUE = Load music from memory
file   : Filename (mem=FALSE) or memory location (mem=TRUE)
offset : File offset to load the music from (only used if mem=FALSE)
length : Data length (only used if mem=FALSE, 0=use to end of file)
flags  : BASS_MUSIC_xxx flags */

void BASSDEF(BASSMOD_MusicFree)();
/* Free the music's resources. */

char *BASSDEF(BASSMOD_MusicGetName)();
/* Retrieves the music's name.
RETURN : The music's name (NULL=error) */

DWORD BASSDEF(BASSMOD_MusicGetLength)(BOOL playlen);
/* Retrieves the length of the music in patterns (how many "orders" there are)
or in output bytes (requires BASS_MUSIC_CALCLEN was used with BASSMOD_MusicLoad).
handle : Music handle
playlen: TRUE=get the playback length, FALSE=get the pattern length
RETURN : The length of the music (0xFFFFFFFF=error) */

BOOL BASSDEF(BASSMOD_MusicPlay)();
/* Play the music. Playback continues from where it was last
stopped/paused. */

BOOL BASSDEF(BASSMOD_MusicPlayEx)(DWORD pos, int flags, BOOL reset);
/* Play the music, specifying start position and playback flags.
pos    : Position to start playback from, LOWORD=order HIWORD=row
flags  : BASS_MUSIC_xxx flags. These flags overwrite the defaults
         specified when the music was loaded. (-1=use current flags)
reset  : TRUE = Stop all current playing notes and reset bpm/etc... */

BOOL BASSDEF(BASSMOD_MusicSetAmplify)(DWORD amp);
/* Set the music's amplification level.
amp    : Amplification level (0-100) */

BOOL BASSDEF(BASSMOD_MusicSetPanSep)(DWORD pan);
/* Set a music's pan seperation.
pan    : Pan seperation (0-100, 50=linear) */

BOOL BASSDEF(BASSMOD_MusicSetPositionScaler)(DWORD scale);
/* Set the music's "GetPosition" scaler
When you call BASSMOD_MusicGetPosition, the "row" (HIWORD) will be
scaled by this value. By using a higher scaler, you can get a more
precise position indication.
scale  : The scaler (1-256) */


BOOL BASSDEF(BASSMOD_MusicIsActive)();
/* Check if the MOD music is active (playing). */

BOOL BASSDEF(BASSMOD_MusicStop)();
/* Stop the MOD music. */

BOOL BASSDEF(BASSMOD_MusicPause)();
/* Pause the MOD music. Use BASSMOD_MusicPlay to resume playback. */

BOOL BASSDEF(BASSMOD_MusicSetPosition)(DWORD pos);
/* Set the current playback position of the MOD music.
pos    : LOWORD=order HIWORD=row ... use MAKELONG(order,row) */

DWORD BASSDEF(BASSMOD_MusicGetPosition)();
/* Get the current playback position of the MOD music.
RETURN : the position... LOWORD=order HIWORD=row (see BASSMOD_MusicSetPositionScaler)
	(0xffffffff=error) */

HSYNC BASSDEF(BASSMOD_MusicSetSync)(DWORD type, DWORD param, SYNCPROC *proc, DWORD user);
/* Setup a sync on the music. Multiple syncs may be used.
type   : Sync type (BASS_SYNC_xxx type & flags)
param  : Sync parameters (see the BASS_SYNC_xxx type description)
proc   : User defined callback function
user   : The 'user' value passed to the callback function
RETURN : Sync handle (NULL=error) */

BOOL BASSDEF(BASSMOD_MusicRemoveSync)(HSYNC sync);
/* Remove a sync from the music
sync   : Handle of sync to remove */


#ifdef __cplusplus
}
#endif

#endif
