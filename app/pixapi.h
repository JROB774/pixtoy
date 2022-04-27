#ifndef PIXAPI_H__ /*/////////////////////////////////////////////////////////*/
#define PIXAPI_H__

#define PIXAPI(name) PIXDEF pixINT PIXAPI_##name(lua_State* lua)

PIXDEF pixVOID pix_register_api(lua_State* lua);

// BITS //////
PIXAPI(band );
PIXAPI(bor  );
PIXAPI(bxor );
PIXAPI(bnot );
// MATH //////
PIXAPI(abs  );
PIXAPI(acos );
PIXAPI(asin );
PIXAPI(atan );
PIXAPI(round);
PIXAPI(ceil );
PIXAPI(floor);
PIXAPI(deg  );
PIXAPI(rad  );
PIXAPI(exp  );
PIXAPI(fmod );
PIXAPI(modf );
PIXAPI(log  );
PIXAPI(min  );
PIXAPI(max  );
PIXAPI(sqrt );
PIXAPI(cos  );
PIXAPI(sin  );
PIXAPI(tan  );
PIXAPI(seed );
PIXAPI(rand );
// DRAW //////
PIXAPI(clrs );
PIXAPI(pset );
PIXAPI(pget );
PIXAPI(line );
PIXAPI(rect );
PIXAPI(circ );

#endif /* PIXAPI_H__ /////////////////////////////////////////////////////////*/
