#ifndef PIXAPP_H__ /*/////////////////////////////////////////////////////////*/
#define PIXAPP_H__

PIXDEF pixVOID        pix_app_init      (pixU32* screen);
PIXDEF pixVOID        pix_app_quit      (pixVOID);
PIXDEF pixVOID        pix_app_tick      (pixFLOAT dt);
PIXDEF const pixCHAR* pix_app_build     (pixCHAR* src);
PIXDEF pixVOID        pix_app_start     (pixVOID);
PIXDEF pixVOID        pix_app_pause     (pixVOID);
PIXDEF pixVOID        pix_app_reset     (pixVOID);
PIXDEF pixBOOL        pix_app_is_playing(pixVOID);
PIXDEF pixBOOL        pix_app_is_paused (pixVOID);
PIXDEF pixU32*        pix_app_get_screen(pixVOID);

#endif /* PIXAPP_H__ /////////////////////////////////////////////////////////*/
