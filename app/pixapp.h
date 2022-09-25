#ifndef PIXAPP_H__ /*/////////////////////////////////////////////////////////*/
#define PIXAPP_H__

// PixToy uses the Sweetie 16 palette by GrafxKid:
//   https://lospec.com/palette-list/sweetie-16
pixU32 PIXPAL[16] =
{
0xFF1A1C2C,0xFF5D275D,0xFFB13E53,0xFFEF7D57,
0xFFFFCD75,0xFFA7F070,0xFF38B764,0xFF257179,
0xFF29366F,0xFF3B5DC9,0xFF41A6F6,0xFF73EFF7,
0xFFF4F4F4,0xFF94B0C2,0xFF566C86,0xFF333C57
};

PIXDEF pixVOID        pix_app_init      (pixVOID);
PIXDEF pixVOID        pix_app_quit      (pixVOID);
PIXDEF pixVOID        pix_app_tick      (pixFLOAT dt);
PIXDEF const pixCHAR* pix_app_build     (pixCHAR* src);
PIXDEF pixVOID        pix_app_start     (pixVOID);
PIXDEF pixVOID        pix_app_pause     (pixVOID);
PIXDEF pixBOOL        pix_app_is_playing(pixVOID);
PIXDEF pixBOOL        pix_app_is_paused (pixVOID);
PIXDEF pixU8*         pix_app_get_screen(pixVOID);

#endif /* PIXAPP_H__ /////////////////////////////////////////////////////////*/
