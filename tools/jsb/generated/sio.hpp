#ifndef __sio_h__
#define __sio_h__

#include "jsapi.h"
#include "jsfriendapi.h"


extern JSClass  *jsb_SIOClient_class;
extern JSObject *jsb_SIOClient_prototype;

JSBool js_sio_SIOClient_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_sio_SIOClient_finalize(JSContext *cx, JSObject *obj);
void js_register_sio_SIOClient(JSContext *cx, JSObject *global);
void register_all_sio(JSContext* cx, JSObject* obj);
JSBool js_sio_SIOClient_getNCenter(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_sio_SIOClient_getUri(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_sio_SIOClient_send(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_sio_SIOClient_fireEvent(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_sio_SIOClient_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_sio_SIOClient_disconnect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_sio_SIOClient_emit(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_sio_SIOClient_connect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_sio_SIOClient_SIOClient(JSContext *cx, uint32_t argc, jsval *vp);
#endif

