#include "sio.hpp"
#include "cocos2d_specifics.hpp"
#include "SIOClient.h"

template<class T>
static JSBool dummy_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
	TypeTest<T> t;
	T* cobj = new T();
	js_type_class_t *p;
	uint32_t typeId = t.s_id();
	HASH_FIND_INT(_js_global_type_ht, &typeId, p);
	assert(p);
	JSObject *_tmp = JS_NewObject(cx, p->jsclass, p->proto, p->parentProto);
	js_proxy_t *pp;
	JS_NEW_PROXY(pp, cobj, _tmp);
	JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(_tmp));

	return JS_TRUE;
}

static JSBool empty_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
	return JS_FALSE;
}


JSClass  *jsb_SIOClient_class;
JSObject *jsb_SIOClient_prototype;

JSBool js_sio_SIOClient_getNCenter(JSContext *cx, uint32_t argc, jsval *vp)
{
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
	js_proxy_t *proxy; JS_GET_NATIVE_PROXY(proxy, obj);
	SIOClient* cobj = (SIOClient *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
	if (argc == 0) {
		Poco::NotificationCenter* ret = cobj->getNCenter();
		jsval jsret;
		do {
			if (ret) {
				js_proxy_t *proxy = js_get_or_create_proxy<Poco::NotificationCenter>(cx, ret);
				jsret = OBJECT_TO_JSVAL(proxy->obj);
			} else {
				jsret = JSVAL_NULL;
			}
		} while (0);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	return JS_FALSE;
}
JSBool js_sio_SIOClient_getUri(JSContext *cx, uint32_t argc, jsval *vp)
{
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
	js_proxy_t *proxy; JS_GET_NATIVE_PROXY(proxy, obj);
	SIOClient* cobj = (SIOClient *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
	if (argc == 0) {
		std::string ret = cobj->getUri();
		jsval jsret;
		jsret = std_string_to_jsval(cx, ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	return JS_FALSE;
}
JSBool js_sio_SIOClient_send(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	JSBool ok = JS_TRUE;
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
	js_proxy_t *proxy; JS_GET_NATIVE_PROXY(proxy, obj);
	SIOClient* cobj = (SIOClient *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
	if (argc == 1) {
		std::string arg0;
		ok &= jsval_to_std_string(cx, argv[0], &arg0);
		JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
		cobj->send(arg0);
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}
JSBool js_sio_SIOClient_fireEvent(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	JSBool ok = JS_TRUE;
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
	js_proxy_t *proxy; JS_GET_NATIVE_PROXY(proxy, obj);
	SIOClient* cobj = (SIOClient *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
	if (argc == 2) {
		const char* arg0;
		Ptr arg1;
		std::string arg0_tmp; ok &= jsval_to_std_string(cx, argv[0], &arg0_tmp); arg0 = arg0_tmp.c_str();
		#pragma warning NO CONVERSION TO NATIVE FOR Ptr;
		JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
		cobj->fireEvent(arg0, arg1);
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 2);
	return JS_FALSE;
}
JSBool js_sio_SIOClient_init(JSContext *cx, uint32_t argc, jsval *vp)
{
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
	js_proxy_t *proxy; JS_GET_NATIVE_PROXY(proxy, obj);
	SIOClient* cobj = (SIOClient *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
	if (argc == 0) {
		bool ret = cobj->init();
		jsval jsret;
		jsret = BOOLEAN_TO_JSVAL(ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	return JS_FALSE;
}
JSBool js_sio_SIOClient_disconnect(JSContext *cx, uint32_t argc, jsval *vp)
{
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
	js_proxy_t *proxy; JS_GET_NATIVE_PROXY(proxy, obj);
	SIOClient* cobj = (SIOClient *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
	if (argc == 0) {
		cobj->disconnect();
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	return JS_FALSE;
}
JSBool js_sio_SIOClient_emit(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	JSBool ok = JS_TRUE;
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
	js_proxy_t *proxy; JS_GET_NATIVE_PROXY(proxy, obj);
	SIOClient* cobj = (SIOClient *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
	if (argc == 2) {
		std::string arg0;
		std::string arg1;
		ok &= jsval_to_std_string(cx, argv[0], &arg0);
		ok &= jsval_to_std_string(cx, argv[1], &arg1);
		JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
		cobj->emit(arg0, arg1);
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 2);
	return JS_FALSE;
}
JSBool js_sio_SIOClient_connect(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	JSBool ok = JS_TRUE;
	if (argc == 1) {
		std::string arg0;
		ok &= jsval_to_std_string(cx, argv[0], &arg0);
		JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
		SIOClient* ret = SIOClient::connect(arg0);
		jsval jsret;
		do {
		if (ret) {
			js_proxy_t *proxy = js_get_or_create_proxy<SIOClient>(cx, ret);
			jsret = OBJECT_TO_JSVAL(proxy->obj);
		} else {
			jsret = JSVAL_NULL;
		}
	} while (0);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
	}
	JS_ReportError(cx, "wrong number of arguments");
	return JS_FALSE;
}

JSBool js_sio_SIOClient_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	JSBool ok = JS_TRUE;
	if (argc == 3) {
		std::string arg0;
		std::string arg1;
		SIOClientImpl* arg2;
		ok &= jsval_to_std_string(cx, argv[0], &arg0);
		ok &= jsval_to_std_string(cx, argv[1], &arg1);
		do {
			js_proxy_t *proxy;
			JSObject *tmpObj = JSVAL_TO_OBJECT(argv[2]);
			JS_GET_NATIVE_PROXY(proxy, tmpObj);
			arg2 = (SIOClientImpl*)(proxy ? proxy->ptr : NULL);
			JSB_PRECONDITION2( arg2, cx, JS_FALSE, "Invalid Native Object");
		} while (0);
		JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
		SIOClient* cobj = new SIOClient(arg0, arg1, arg2);
		TypeTest<SIOClient> t;
		js_type_class_t *typeClass;
		uint32_t typeId = t.s_id();
		HASH_FIND_INT(_js_global_type_ht, &typeId, typeClass);
		assert(typeClass);
		JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
		JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
		// link the native object with the javascript object
		js_proxy_t *p;
		JS_NEW_PROXY(p, cobj, obj);
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 3);
	return JS_FALSE;
}




void js_sio_SIOClient_finalize(JSFreeOp *fop, JSObject *obj) {
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    JS_GET_NATIVE_PROXY(jsproxy, obj);
    if (jsproxy) {
        JS_GET_PROXY(nproxy, jsproxy->ptr);

        SIOClient *nobj = static_cast<SIOClient *>(nproxy->ptr);
        if (nobj)
            delete nobj;
        
        JS_REMOVE_PROXY(nproxy, jsproxy);
    }
}

static JSBool js_sio_SIOClient_ctor(JSContext *cx, uint32_t argc, jsval *vp)
{
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
    SIOClient *nobj = new SIOClient();
    js_proxy_t* p;
    JS_NEW_PROXY(p, nobj, obj);
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return JS_TRUE;
}

void js_register_sio_SIOClient(JSContext *cx, JSObject *global) {
	jsb_SIOClient_class = (JSClass *)calloc(1, sizeof(JSClass));
	jsb_SIOClient_class->name = "Client";
	jsb_SIOClient_class->addProperty = JS_PropertyStub;
	jsb_SIOClient_class->delProperty = JS_PropertyStub;
	jsb_SIOClient_class->getProperty = JS_PropertyStub;
	jsb_SIOClient_class->setProperty = JS_StrictPropertyStub;
	jsb_SIOClient_class->enumerate = JS_EnumerateStub;
	jsb_SIOClient_class->resolve = JS_ResolveStub;
	jsb_SIOClient_class->convert = JS_ConvertStub;
	jsb_SIOClient_class->finalize = js_sio_SIOClient_finalize;
	jsb_SIOClient_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		{0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
	};

	static JSFunctionSpec funcs[] = {
		JS_FN("getNCenter", js_sio_SIOClient_getNCenter, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getUri", js_sio_SIOClient_getUri, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("send", js_sio_SIOClient_send, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("fireEvent", js_sio_SIOClient_fireEvent, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("init", js_sio_SIOClient_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("disconnect", js_sio_SIOClient_disconnect, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("emit", js_sio_SIOClient_emit, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("ctor", js_sio_SIOClient_ctor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
	};

	static JSFunctionSpec st_funcs[] = {
		JS_FN("connect", js_sio_SIOClient_connect, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FS_END
	};

	jsb_SIOClient_prototype = JS_InitClass(
		cx, global,
		NULL, // parent proto
		jsb_SIOClient_class,
		js_sio_SIOClient_constructor, 0, // constructor
		properties,
		funcs,
		NULL, // no static properties
		st_funcs);
	// make the class enumerable in the registered namespace
	JSBool found;
	JS_SetPropertyAttributes(cx, global, "Client", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

	// add the proto and JSClass to the type->js info hash table
	TypeTest<SIOClient> t;
	js_type_class_t *p;
	uint32_t typeId = t.s_id();
	HASH_FIND_INT(_js_global_type_ht, &typeId, p);
	if (!p) {
		p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
		p->type = typeId;
		p->jsclass = jsb_SIOClient_class;
		p->proto = jsb_SIOClient_prototype;
		p->parentProto = NULL;
		HASH_ADD_INT(_js_global_type_ht, type, p);
	}
}

void register_all_sio(JSContext* cx, JSObject* obj) {
	// first, try to get the ns
	jsval nsval;
	JSObject *ns;
	JS_GetProperty(cx, obj, "sio", &nsval);
	if (nsval == JSVAL_VOID) {
		ns = JS_NewObject(cx, NULL, NULL, NULL);
		nsval = OBJECT_TO_JSVAL(ns);
		JS_SetProperty(cx, obj, "sio", &nsval);
	} else {
		JS_ValueToObject(cx, nsval, &ns);
	}
	obj = ns;

	js_register_sio_SIOClient(cx, obj);
}

