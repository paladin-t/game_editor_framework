/**
 * @version: 1.9
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

#ifndef __GEF_SERVICE_BASE_H__
#define __GEF_SERVICE_BASE_H__

#include <exception>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <windows.h>

#ifdef GEF_SERVICE_BASE_EXPORTS
#	define GEF_SERVICE_BASE_API __declspec(dllexport)
#else
#	define GEF_SERVICE_BASE_API __declspec(dllimport)
#endif

namespace gef {

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

typedef char* Str;
typedef const char* ConstStr;
typedef void* Ptr;
typedef const void* ConstPtr;

typedef std::string StdStr;

typedef u32 MsgGroup;
typedef u32 MsgType;

#ifdef _DEBUG
namespace InternalFinal { template<typename T> class ThisClassIsFinal { protected: ThisClassIsFinal() { } }; }
template<typename T> class Final : private virtual InternalFinal::ThisClassIsFinal<T> { friend typename T; };
#else // _DEBUG
template<typename T> class Final { };
#endif // _DEBUG

class GefServiceBaseException : public std::exception {

public:
	GefServiceBaseException(ConstStr const &d) : std::exception(d) { }

};

template<typename _Ty, typename _Ax = std::allocator<_Ty>>
class Vector : public std::vector<_Ty, _Ax> {

public:
	const_reference at(s32 _pos) const { return _Myt::at(_get_positive_pos(_pos)); }
	reference at(s32 _pos) { return _Myt::at(_get_positive_pos(_pos)); }
	const_reference operator [] (s32 _pos) const { return _Myt::operator [](_get_positive_pos(_pos)); }
	reference operator [] (s32 _pos) { return _Myt::operator [](_get_positive_pos(_pos)); }

private:
	size_type _get_positive_pos(s32 _pos) const { while(_pos < 0) { _pos += size(); } size_type p = (size_type)_pos; return p; }

};

template<typename _Kty, typename _Ty, typename _Pr = std::less<_Kty>, typename _Alloc = std::allocator<std::pair<const _Kty, _Ty>>>
class Map : public std::map<_Kty, _Ty, _Pr, _Alloc> { };

GEF_SERVICE_BASE_API void GEF_EXCEPT(ConstStr fmt, ...);
GEF_SERVICE_BASE_API void GEF_ENSURE(bool exp, ConstStr fmt, ...);

struct GEF_SERVICE_BASE_API Obj : Final<Obj> {
	enum Types {
		OT_NIL,
		OT_S32,
		OT_U32,
		OT_S64,
		OT_U64,
		OT_F32,
		OT_F64,
		OT_STR,
		OT_PTR,
		OT_BLN,
	};
	union Data {
		s8 _raw32[4];
		s8 _raw64[8];
		s32 _s32;
		u32 _u32;
		s64 _s64;
		u64 _u64;
		f32 _f32;
		f64 _f64;
		Str _str;
		Ptr _ptr;
		bool _bln;
	};

	Types type;
	Data data;
	size_t size;

	static Obj NIL;

	Obj() : type(OT_NIL) { memset(data._raw64, 0, sizeof(data._raw64)); size = sizeof(data._raw64); }
	Obj(const Obj &other) { _tidy(); type = other.type; size = other.size; if(type == OT_STR) data._str = _strdup(other.data._str); else data = other.data; }
	~Obj() { _tidy(); }
	Obj(s32 d) { type = OT_S32; size = sizeof(data._s32); data._s32 = d; }
	Obj(u32 d) { type = OT_U32; size = sizeof(data._u32); data._u32 = d; }
	Obj(s64 d) { type = OT_S64; size = sizeof(data._s64); data._s64 = d; }
	Obj(u64 d) { type = OT_U64; size = sizeof(data._u64); data._u64 = d; }
	Obj(f32 d) { type = OT_F32; size = sizeof(data._f32); data._f32 = d; }
	Obj(f64 d) { type = OT_F64; size = sizeof(data._f64); data._f64 = d; }
	Obj(ConstStr const d) { type = OT_STR; size = sizeof(data._str); data._str = _strdup(d); }
	Obj(s8* d) { type = OT_STR; size = sizeof(data._str); data._str = _strdup((ConstStr)d); }
	Obj(const s8* d) { type = OT_STR; size = sizeof(data._str); data._str = _strdup((ConstStr)d); }
	Obj(Ptr d) { type = OT_PTR; size = sizeof(data._ptr); data._ptr = d; }
	Obj(bool d) { type = OT_BLN; size = sizeof(data._bln); data._bln = d; }
	Obj &operator = (const Obj &other) { _tidy(); type = other.type; if(type == OT_STR) data._str = _strdup(other.data._str); else data = other.data; return *this; }
	Types getType(void) const { return type; }
	s8* getRaw(void) { return data._raw64; }
	operator s32(void) const { GEF_ENSURE(type == OT_S32, "Obj type not match"); return data._s32; }
	operator u32(void) const { GEF_ENSURE(type == OT_U32, "Obj type not match"); return data._u32; }
	operator s64(void) const { GEF_ENSURE(type == OT_S64, "Obj type not match"); return data._s64; }
	operator u64(void) const { GEF_ENSURE(type == OT_U64, "Obj type not match"); return data._u64; }
	operator f32(void) const { GEF_ENSURE(type == OT_F32, "Obj type not match"); return data._f32; }
	operator f64(void) const { GEF_ENSURE(type == OT_F64, "Obj type not match"); return data._f64; }
	operator bool(void) const { GEF_ENSURE(type == OT_BLN, "Obj type not match"); return data._bln; }
	Str getStr(void) const { GEF_ENSURE(type == OT_STR, "Obj type not match"); return data._str; }
	Ptr getPtr(void) const { GEF_ENSURE(type == OT_PTR, "Obj type not match"); return data._ptr; }
	StdStr toStr(void) const;
	size_t getSize(void) const { return size; }
	bool operator == (const Obj &other) const { if(type != other.type) return false; if(type != OT_STR) return memcmp(data._raw64, other.data._raw64, size) == 0; else return strcmp(data._str, other.data._str) == 0; }
	bool operator != (const Obj &other) const { if(type != other.type) return true; if(type != OT_STR) return memcmp(data._raw64, other.data._raw64, size) != 0; else return strcmp(data._str, other.data._str) != 0; }
	bool operator < (const Obj &other) const { if(type != other.type) return type < other.type; if(type != OT_STR) return memcmp(data._raw64, other.data._raw64, size) < 0; else return strcmp(data._str, other.data._str) < 0; }
	void _tidy(void) { if(type == OT_STR) { free(data._str); type = OT_NIL; } }
};

typedef Vector<Obj> ParamList;
typedef Map<StdStr, Obj> StrParamDict;
typedef Map<Obj, Obj> ObjParamDict;

typedef void (* MsgProcFunc)(MsgGroup g, MsgType t, const ParamList &p);

class GEF_SERVICE_BASE_API IService {

public:
	IService();
	virtual ~IService();

	bool regMsgProc(MsgGroup g, MsgType t, MsgProcFunc f);
    bool unregMsgProc(MsgGroup g, MsgType t);
    void setMsgBubbler(MsgProcFunc f);
	void setMsgBroadcaster(MsgProcFunc f);
	void bubble(MsgGroup g, MsgType t, const ParamList &p);
	void broadcast(MsgGroup g, MsgType t, const ParamList &p);
    void sink(MsgGroup g, MsgType t, const ParamList &p);
	bool getEdited(void);
	void postInitDone(void);
	void prevDestroy(void);
	void echo(const ParamList &p);
	void echo(ConstStr fmt, ...);

private:
	virtual bool impl_regMsgProc(MsgGroup g, MsgType t, MsgProcFunc f) = 0;
    virtual bool impl_unregMsgProc(MsgGroup g, MsgType t) = 0;
    virtual void impl_sink(MsgGroup g, MsgType t, const ParamList &p) = 0;
	virtual bool impl_getEdited(void) = 0;
	virtual void impl_postInitDone(void);
	virtual void impl_prevDestroy(void);

private:
	MsgProcFunc mBubbler;
	MsgProcFunc mBroadcaster;

};

class GEF_SERVICE_BASE_API ICommand {

public:
	ICommand();
	virtual ~ICommand();

	void _setId(u32 id);
	u32 _getId(void) const;

	void setParam(const StdStr &n, const Obj &v);
	const Obj &getParam(const StdStr &n) const;

	void exec(void);
	void undo(void);
	void redo(void);

private:
	virtual void impl_setParam(const StdStr &n, const Obj &v);
	virtual const Obj &impl_getParam(const StdStr &n) const;

	virtual void impl_exec(void) = 0;
	virtual void impl_undo(void) = 0;
	virtual void impl_redo(void) = 0;

private:
	u32 mId;
	StrParamDict* mParamDict;

};

}

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#	ifndef OPEN_GEF_SERVICE_FUNC_NAME
#		define OPEN_GEF_SERVICE_FUNC_NAME "OpenGefService"
#	endif // OPEN_GEF_SERVICE_FUNC_NAME

#	ifndef CLOSE_GEF_SERVICE_FUNC_NAME
#		define CLOSE_GEF_SERVICE_FUNC_NAME "CloseGefService"
#	endif // CLOSE_GEF_SERVICE_FUNC_NAME

	typedef gef::IService* (* OpenGefServiceFunc)(void);
	typedef void (* CloseGefServiceFunc)(gef::IService* svr);

#	ifndef CREATE_GEF_COMMAND_FUNC_NAME
#		define CREATE_GEF_COMMAND_FUNC_NAME "CreateGefCommand"
#	endif // CREATE_GEF_COMMAND_FUNC_NAME
#	ifndef CREATE_GEF_COMMAND_BY_NUM_FUNC_NAME
#		define CREATE_GEF_COMMAND_BY_NUM_FUNC_NAME "CreateGefCommandByNum"
#	endif // CREATE_GEF_COMMAND_BY_NUM_FUNC_NAME
#	ifndef CREATE_GEF_COMMAND_BY_STR_FUNC_NAME
#		define CREATE_GEF_COMMAND_BY_STR_FUNC_NAME "CreateGefCommandByStr"
#	endif // CREATE_GEF_COMMAND_BY_STR_FUNC_NAME
#	ifndef DESTROY_GEF_COMMAND_FUNC_NAME
#		define DESTROY_GEF_COMMAND_FUNC_NAME "DestroyGefCommand"
#	endif // DESTROY_GEF_COMMAND_FUNC_NAME

	typedef gef::ICommand* (* CreateGefCommandFunc)(void);
	typedef gef::ICommand* (* CreateGefCommandByNumFunc)(gef::u32 n);
	typedef gef::ICommand* (* CreateGefCommandByStrFunc)(gef::ConstStr s);
	typedef void (* DestroyGefCommandFunc)(gef::ICommand* cmd);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __GEF_SERVICE_BASE_H__
