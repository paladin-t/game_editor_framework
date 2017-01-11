/**
 * @version: 1.2
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

#include "Stdafx.h"
#include "gef_wrapper_util.h"

using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

namespace gef {

String^ WrapperUtil::ApplicationDirectory::get() {
	return Directory::GetParent(Application::ExecutablePath)->FullName;
}

String^ WrapperUtil::ConvertString(ConstStr n) {
	return gcnew String(n);
}

StdStr WrapperUtil::ConvertString(String^ m) {
	IntPtr p = Marshal::StringToHGlobalAnsi(m);
	ConstStr linkStr = static_cast<Str>(p.ToPointer());
	StdStr result(linkStr);
	Marshal::FreeHGlobal(p);

	return result;
}

Object^ WrapperUtil::ConvertObject(const Obj &n) {
	Object^ result = nullptr;
	switch(n.type) {
		case Obj::OT_NIL:
			break;
		case Obj::OT_S32:
			result = n.data._s32;
			break;
		case Obj::OT_U32:
			result = n.data._u32;
			break;
		case Obj::OT_S64:
			result = n.data._s64;
			break;
		case Obj::OT_U64:
			result = n.data._u64;
			break;
		case Obj::OT_F32:
			result = n.data._f32;
			break;
		case Obj::OT_F64:
			result = n.data._f64;
			break;
		case Obj::OT_STR:
			result = ConvertString(n.data._str);
			break;
		case Obj::OT_PTR:
			result = IntPtr(n.data._ptr);
			break;
		case Obj::OT_BLN:
			result = n.data._bln;
			break;
		default:
			GEF_EXCEPT("Unsupported Obj type");
			break;
	}

	return result;
}

Obj WrapperUtil::ConvertObject(Object^ m) {
	if(m == nullptr) return Obj();
	Type^ t = m->GetType();
	if(t == Int32::typeid) {
		return Obj((Int32)m);
	} else if(t == UInt32::typeid) {
		return Obj((UInt32)m);
	} else if(t == Int64::typeid) {
		return Obj((Int64)m);
	} else if(t == UInt64::typeid) {
		return Obj((UInt64)m);
	} else if(t == Single::typeid) {
		return Obj((Single)m);
	} else if(t == Double::typeid) {
		return Obj((Double)m);
	} else if(t == String::typeid) {
		StdStr s = ConvertString((String^)m);
		return Obj((Str)s.c_str());
	} else if(t == IntPtr::typeid) {
		IntPtr p = (IntPtr)m;
		return Obj(p.ToPointer());
	} else if(t == Boolean::typeid) {
		return Obj((Boolean)m);
	} else {
		GEF_EXCEPT("Unsupported Object type");
	}

	return Obj();
}

List<Object^>^ WrapperUtil::ConvertParamList(const ParamList &n) {
	List<Object^>^ result = gcnew List<Object^>();
	for(ParamList::const_iterator it = n.begin(); it != n.end(); ++it) {
		Object^ o = ConvertObject(*it);
		result->Add(o);
	}

	return result;
}

ParamList WrapperUtil::ConvertParamList(List<Object^>^ m) {
	ParamList result;
	ConvertParamList(m, result);

	return result;
}

void WrapperUtil::ConvertParamList(List<Object^>^ m, ParamList &o) {
	o.clear();
	for each(Object^ obj in m) {
		Obj nobj = ConvertObject(obj);
		o.push_back(nobj);
	}
}

Dictionary<String^, Object^>^ WrapperUtil::ConvertParamDict(const StrParamDict &n) {
	Dictionary<String^, Object^>^ result = gcnew Dictionary<String^, Object^>();
	for(StrParamDict::const_iterator it = n.begin(); it != n.end(); ++it) {
		String^ k = ConvertString(it->first.c_str());
		Object^ o = ConvertObject(it->second);
		result[k] = o;
	}

	return result;
}

StrParamDict WrapperUtil::ConvertParamDict(Dictionary<String^, Object^>^ m) {
	StrParamDict result;
	ConvertParamDict(m, result);

	return result;
}

void WrapperUtil::ConvertParamDict(Dictionary<String^, Object^>^ m, StrParamDict &o) {
	o.clear();
	Dictionary<String^, Object^>::Enumerator^ p = m->GetEnumerator();
	while(p->MoveNext()) {
		StdStr k = ConvertString(p->Current.Key);
		Obj nobj = ConvertObject(p->Current.Value);
		o[k] = nobj;
	}
}

Dictionary<Object^, Object^>^ WrapperUtil::ConvertParamDict(const ObjParamDict &n) {
	Dictionary<Object^, Object^>^ result = gcnew Dictionary<Object^, Object^>();
	for(ObjParamDict::const_iterator it = n.begin(); it != n.end(); ++it) {
		Object^ k = ConvertObject(it->first);
		Object^ o = ConvertObject(it->second);
		result[k] = o;
	}

	return result;
}

ObjParamDict WrapperUtil::ConvertParamDict(Dictionary<Object^, Object^>^ m) {
	ObjParamDict result;
	ConvertParamDict(m, result);

	return result;
}

void WrapperUtil::ConvertParamDict(Dictionary<Object^, Object^>^ m, ObjParamDict &o) {
	o.clear();
	Dictionary<Object^, Object^>::Enumerator^ p = m->GetEnumerator();
	while(p->MoveNext()) {
		Obj k = ConvertObject(p->Current.Key);
		Obj nobj = ConvertObject(p->Current.Value);
		o[k] = nobj;
	}
}

}
