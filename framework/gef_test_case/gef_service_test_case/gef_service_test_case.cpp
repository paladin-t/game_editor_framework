/**
 * @version: 1.9
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

#include <assert.h>
#include <windows.h>
#include <list>
#include <gl/gl.h>
#include <gl/glu.h>

#include "gef_service_test_case.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

static gef::s32 wndW = 0;
static gef::s32 wndH = 0;

#ifndef WNDX2VIEW
#	define WNDX2VIEW(x) (((x) / (f32)wndW - 0.5f) * 2.0f)
#endif // WNDX2VIEW
#ifndef WNDY2VIEW
#	define WNDY2VIEW(y) (-((y) / (f32)wndH - 0.5f) * 2.0f)
#endif // WNDY2VIEW

struct _MemLeakDetector {
	_MemLeakDetector() {
	}
	~_MemLeakDetector() {
		gef::s32 c = _CrtDumpMemoryLeaks();

		if(c) {
			_asm int 3
		}
	}
};

static _MemLeakDetector __MEM_LEAK_DETECTOR;

struct Vec2f {
	gef::f32 x;
	gef::f32 y;
	Vec2f() : x(0.0f), y(0.0f) { }
	Vec2f(gef::f32 _x, gef::f32 _y) : x(_x), y(_y) { }
};

struct Vec3f : public Vec2f {
	gef::f32 z;
	Vec3f() : Vec2f(), z(0.0f) { }
	Vec3f(gef::f32 _x, gef::f32 _y, gef::f32 _z) : Vec2f(_x, _y), z(_z) { }
};

struct Quad {
	gef::u64 id;
	Vec2f position;
	Vec2f size;
	Vec3f color;
};

typedef std::list<Quad> QuadColl;

static Vec3f clearColor = Vec3f(0.0f, 0.0f, 0.0f);

static bool usePreAddQuad = false;
static Quad preAddQuad;
static struct {
	Quad* movingQuad;
	Vec2f mousePoint;
	Vec2f oldPos;
} movingContext;
static QuadColl quads;

static HDC hDC = NULL;
static HGLRC hRC = NULL;
static HWND hWnd = NULL;

static gef::u64 _genQuadId(void) {
	static gef::u64 id = 0;

	return ++id;
}

static bool _collideQuadWithPoint(const Quad &q, gef::f32 vx, gef::f32 vy) {
	gef::f32 minx = min(q.position.x, q.position.x + q.size.x);
	gef::f32 maxx = max(q.position.x, q.position.x + q.size.x);
	gef::f32 miny = min(q.position.y, q.position.y + q.size.y);
	gef::f32 maxy = max(q.position.y, q.position.y + q.size.y);

	return (vx >= minx && vx <= maxx && vy >= miny && vy <= maxy);
}

static Quad* _pickQuad(gef::f32 vx, gef::f32 vy) {
	Quad* result = NULL;
	for(QuadColl::reverse_iterator it = quads.rbegin(); it != quads.rend(); ++it) {
		if(_collideQuadWithPoint(*it, vx, vy)) {
			result = &(*it);
			break;
		}
	}

	return result;
}

static void _resizeGlScene(gef::s32 width, gef::s32 height) {
	if(height == 0)
		height = 1;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, width / (gef::f32)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static bool _initGl(void) {
	glShadeModel(GL_SMOOTH);
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return true;
}

static bool _drawGlScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_TRIANGLES);
		for(QuadColl::iterator it = quads.begin(); it != quads.end(); ++it) {
			const Quad &q = *it;

			glColor3f(q.color.x, q.color.y, q.color.z);
			glVertex3f(q.position.x, q.position.y, 0.0f);
			glColor3f(q.color.x, q.color.y, q.color.z);
			glVertex3f(q.position.x + q.size.x, q.position.y, 0.0f);
			glColor3f(q.color.x, q.color.y, q.color.z);
			glVertex3f(q.position.x, q.position.y + q.size.y, 0.0f);

			glColor3f(q.color.x, q.color.y, q.color.z);
			glVertex3f(q.position.x, q.position.y + q.size.y, 0.0f);
			glColor3f(q.color.x, q.color.y, q.color.z);
			glVertex3f(q.position.x + q.size.x, q.position.y + q.size.y, 0.0f);
			glColor3f(q.color.x, q.color.y, q.color.z);
			glVertex3f(q.position.x + q.size.x, q.position.y, 0.0f);
		}
		if(usePreAddQuad) {
			glColor3f(preAddQuad.color.x, preAddQuad.color.y, preAddQuad.color.z);
			glVertex3f(preAddQuad.position.x, preAddQuad.position.y, 0.0f);
			glColor3f(preAddQuad.color.x, preAddQuad.color.y, preAddQuad.color.z);
			glVertex3f(preAddQuad.position.x + preAddQuad.size.x, preAddQuad.position.y, 0.0f);
			glColor3f(preAddQuad.color.x, preAddQuad.color.y, preAddQuad.color.z);
			glVertex3f(preAddQuad.position.x, preAddQuad.position.y + preAddQuad.size.y, 0.0f);

			glColor3f(preAddQuad.color.x, preAddQuad.color.y, preAddQuad.color.z);
			glVertex3f(preAddQuad.position.x, preAddQuad.position.y + preAddQuad.size.y, 0.0f);
			glColor3f(preAddQuad.color.x, preAddQuad.color.y, preAddQuad.color.z);
			glVertex3f(preAddQuad.position.x + preAddQuad.size.x, preAddQuad.position.y + preAddQuad.size.y, 0.0f);
			glColor3f(preAddQuad.color.x, preAddQuad.color.y, preAddQuad.color.z);
			glVertex3f(preAddQuad.position.x + preAddQuad.size.x, preAddQuad.position.y, 0.0f);
		}
	glEnd();

	return true;
}

static void _killGlWindow(void) {
	if(hRC) {
		if(!wglMakeCurrent(NULL, NULL))
			MessageBoxA(hWnd, "Releasing DC / RC failed", __FUNCTION__, MB_OK | MB_ICONINFORMATION);
		if(!wglDeleteContext(hRC))
			MessageBoxA(hWnd, "Releasing RC failed", __FUNCTION__, MB_OK | MB_ICONINFORMATION);
		hRC = NULL;
	}

	if(hDC && !ReleaseDC(hWnd, hDC)) {
		MessageBoxA(hWnd, "Releasing DC failed", __FUNCTION__, MB_OK | MB_ICONINFORMATION);
		hDC = NULL;
	}
}

static bool _createGlWindow(gef::s32 width, gef::s32 height, gef::s32 bits) {
	GLuint PixelFormat;

	RECT WindowRect;
	WindowRect.left = (LONG)0;
	WindowRect.right = (LONG)width;
	WindowRect.top = (LONG)0;
	WindowRect.bottom = (LONG)height;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), 1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, bits,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	if(!(hDC = GetDC(hWnd))) {
		_killGlWindow();
		MessageBoxA(hWnd, "Cannot create a window device context", __FUNCTION__, MB_OK | MB_ICONINFORMATION);

		return false;
	}

	if(!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {
		_killGlWindow();
		MessageBoxA(hWnd, "Cannot create a correct pixel format", __FUNCTION__, MB_OK | MB_ICONINFORMATION);

		return false;
	}

	if(!SetPixelFormat(hDC, PixelFormat, &pfd)) {
		_killGlWindow();
		MessageBoxA(hWnd, "Cannot set pixel format", __FUNCTION__, MB_OK | MB_ICONINFORMATION);

		return false;
	}

	if(!(hRC = wglCreateContext(hDC))) {
		_killGlWindow();
		MessageBoxA(hWnd, "Cannot create OpenGL rendering context", __FUNCTION__, MB_OK | MB_ICONINFORMATION);

		return false;
	}

	if(!wglMakeCurrent(hDC, hRC)) {
		_killGlWindow();
		MessageBoxA(hWnd, "Cannot active current OpenGL rendering context", __FUNCTION__, MB_OK | MB_ICONINFORMATION);

		return false;
	}

	_resizeGlScene(width, height);

	if(!_initGl()) {
		_killGlWindow();
		MessageBoxA(hWnd, "Initializing failed", __FUNCTION__, MB_OK | MB_ICONINFORMATION);

		return false;
	}

	return true;
}

namespace gef {

class CmdAdd : public ICommand {

public:
	CmdAdd();
	virtual ~CmdAdd();

private:
	virtual void impl_exec(void);
	virtual void impl_undo(void);
	virtual void impl_redo(void);

};

class CmdRemove : public ICommand {

public:
	CmdRemove();
	virtual ~CmdRemove();

private:
	virtual void impl_exec(void);
	virtual void impl_undo(void);
	virtual void impl_redo(void);

};

class CmdMove : public ICommand {

public:
	CmdMove();
	virtual ~CmdMove();

private:
	virtual void impl_exec(void);
	virtual void impl_undo(void);
	virtual void impl_redo(void);

};

class Service : public IService, Final<Service> {

private:
	typedef Map<MsgType, MsgProcFunc> MsgGroupInfo;
	typedef Map<MsgGroup, MsgGroupInfo> MsgProcInfo;

private:
	virtual bool impl_regMsgProc(MsgGroup g, MsgType t, MsgProcFunc f);
    virtual bool impl_unregMsgProc(MsgGroup g, MsgType t);
    virtual void impl_sink(MsgGroup g, MsgType t, const ParamList &p);
	virtual bool impl_getEdited(void);
	virtual void impl_postInitDone(void);
	virtual void impl_prevDestroy(void);

private:
	MsgProcInfo mMsgProcInfoColl;

};

}

namespace gef {

static IService* gService = NULL;

static void _addAndExecCmd(ICommand* cmd);
static void _onResHwndFunc(MsgGroup g, MsgType t, const ParamList &p);
static void _onTimerFunc(MsgGroup g, MsgType t, const ParamList &p);
static void _onResizeFunc(MsgGroup g, MsgType t, const ParamList &p);
static void _onTestFunc(MsgGroup g, MsgType t, const ParamList &p);
static void _onTestSetClearColorFunc(MsgGroup g, MsgType t, const ParamList &p);
static void _onTestPreAddQuadFunc(MsgGroup g, MsgType t, const ParamList &p);
static void _onTestSetPreAddedQuadSize(MsgGroup g, MsgType t, const ParamList &p);
static void _onTestAddQuadFunc(MsgGroup g, MsgType t, const ParamList &p);
static void _onTestRemoveQuadFunc(MsgGroup g, MsgType t, const ParamList &p);
static void _onTestPreMoveQuad(MsgGroup g, MsgType t, const ParamList &p);
static void _onTestMovingQuad(MsgGroup g, MsgType t, const ParamList &p);
static void _onTestMoveQuadFunc(MsgGroup g, MsgType t, const ParamList &p);

}

namespace gef {

CmdAdd::CmdAdd() {
}

CmdAdd::~CmdAdd() {
}

void CmdAdd::impl_exec(void) {
	Quad q;
	q.id = getParam("ID");
	q.position.x = getParam("POS_X");
	q.position.y = getParam("POS_Y");
	q.size.x = getParam("SIZE_X");
	q.size.y = getParam("SIZE_Y");
	q.color.x = getParam("COL_R");
	q.color.y = getParam("COL_G");
	q.color.z = getParam("COL_B");
	quads.push_back(q);
}

void CmdAdd::impl_undo(void) {
	quads.pop_back();
}

void CmdAdd::impl_redo(void) {
	exec();
}

CmdRemove::CmdRemove() {
}

CmdRemove::~CmdRemove() {
}

void CmdRemove::impl_exec(void) {
	u64 id = getParam("ID");
	for(QuadColl::iterator it = quads.begin(); it != quads.end(); ++it) {
		if(id == it->id) {
			QuadColl::iterator nit = it; ++nit;
			bool ie = nit == quads.end();
			setParam("IS_END", ie);
			if(!ie)
				setParam("NEXT_ID", nit->id);
			quads.erase(it);
			break;
		}
	}
}

void CmdRemove::impl_undo(void) {
	bool ie = getParam("IS_END");
	QuadColl::iterator nit = quads.end();
	if(!ie) {
		u64 nid = getParam("NEXT_ID");
		for(QuadColl::iterator it = quads.begin(); it != quads.end(); ++it) {
			if(it->id == nid) {
				nit = it;
				break;
			}
		}
	}
	Quad q;
	q.id = getParam("ID");
	q.position.x = getParam("POS_X");
	q.position.y = getParam("POS_Y");
	q.size.x = getParam("SIZE_X");
	q.size.y = getParam("SIZE_Y");
	q.color.x = getParam("COL_R");
	q.color.y = getParam("COL_G");
	q.color.z = getParam("COL_B");
	quads.insert(nit, q);
}

void CmdRemove::impl_redo(void) {
	exec();
}

CmdMove::CmdMove() {
}

CmdMove::~CmdMove() {
}

void CmdMove::impl_exec(void) {
	u64 id = getParam("ID");
	for(QuadColl::iterator it = quads.begin(); it != quads.end(); ++it) {
		if(it->id == id) {
			f32 nx = getParam("POS_X");
			f32 ny = getParam("POS_Y");
			it->position.x = nx;
			it->position.y = ny;
			break;
		}
	}
}

void CmdMove::impl_undo(void) {
	u64 id = getParam("ID");
	for(QuadColl::iterator it = quads.begin(); it != quads.end(); ++it) {
		if(it->id == id) {
			f32 ox = getParam("OLD_POS_X");
			f32 oy = getParam("OLD_POS_Y");
			it->position.x = ox;
			it->position.y = oy;
			break;
		}
	}
}

void CmdMove::impl_redo(void) {
	exec();
}

bool Service::impl_regMsgProc(MsgGroup g, MsgType t, MsgProcFunc f) {
	mMsgProcInfoColl[g][t] = f;

	return true;
}

bool Service::impl_unregMsgProc(MsgGroup g, MsgType t) {
	if(mMsgProcInfoColl.find(g) == mMsgProcInfoColl.end()) return false;
	if(mMsgProcInfoColl[g].find(t) == mMsgProcInfoColl[g].end()) return false;
	mMsgProcInfoColl[g].erase(t);

	return true;
}

void Service::impl_sink(MsgGroup g, MsgType t, const ParamList &p) {
	if(mMsgProcInfoColl.find(g) == mMsgProcInfoColl.end()) return;
	if(mMsgProcInfoColl[g].find(t) == mMsgProcInfoColl[g].end()) return;
	(*mMsgProcInfoColl[g][t])(g, t, p);
}

bool Service::impl_getEdited(void) {
	return false;
}

void Service::impl_postInitDone(void) {
	ParamList p;
	p.push_back(Obj("Canvas"));
	bubble(MGT_SYSTEM, MST_SYS_REQ_HWND, p);
}

void Service::impl_prevDestroy(void) {
	quads.clear();
	_killGlWindow();
}

}

gef::IService* OpenGefService(void) {
	gef::IService* result = new gef::Service();

	result->regMsgProc(gef::MGT_SYSTEM, gef::MST_SYS_RES_HWND, gef::_onResHwndFunc);
	result->regMsgProc(gef::MGT_SYSTEM, gef::MST_SYS_TIMER, gef::_onTimerFunc);
	result->regMsgProc(gef::MGT_SYSTEM, gef::MST_SYS_RESIZE, gef::_onResizeFunc);
	result->regMsgProc(gef::MGT_TEST, gef::MTT_TEST_COMMON, gef::_onTestFunc);
	result->regMsgProc(gef::MGT_TEST, gef::MTT_TEST_SET_CLEAR_COLOR, gef::_onTestSetClearColorFunc);
	result->regMsgProc(gef::MGT_TEST, gef::MTT_TEST_PRE_ADD_QUAD, gef::_onTestPreAddQuadFunc);
	result->regMsgProc(gef::MGT_TEST, gef::MTT_TEST_SET_PRE_ADDED_QUAD_SIZE, gef::_onTestSetPreAddedQuadSize);
	result->regMsgProc(gef::MGT_TEST, gef::MTT_TEST_ADD_QUAD, gef::_onTestAddQuadFunc);
	result->regMsgProc(gef::MGT_TEST, gef::MTT_TEST_REMOVE_QUAD, gef::_onTestRemoveQuadFunc);
	result->regMsgProc(gef::MGT_TEST, gef::MTT_TEST_PRE_MOVE_QUAD, gef::_onTestPreMoveQuad);
	result->regMsgProc(gef::MGT_TEST, gef::MTT_TEST_MOVING_QUAD, gef::_onTestMovingQuad);
	result->regMsgProc(gef::MGT_TEST, gef::MTT_TEST_MOVE_QUAD, gef::_onTestMoveQuadFunc);

	gef::gService = result;

	return result;
}

void CloseGefService(gef::IService* svr) {
	assert(gef::gService == svr);
	if(svr) {
		delete svr;
		gef::gService = NULL;
	}
}

gef::ICommand* CreateGefCommand(void) {
	return NULL;
}

gef::ICommand* CreateGefCommandByNum(gef::u32 n) {
	gef::ICommand* result = NULL;
	switch(n) {
		case gef::MTT_TEST_ADD_QUAD:
			result = new gef::CmdAdd();
			break;
		case gef::MTT_TEST_REMOVE_QUAD:
			result = new gef::CmdRemove();
			break;
		case gef::MTT_TEST_MOVE_QUAD:
			result = new gef::CmdMove();
			break;
	}

	return result;
}

gef::ICommand* CreateGefCommandByStr(gef::ConstStr s) {
	return NULL;
}

void DestroyGefCommand(gef::ICommand* cmd) {
	delete cmd;
}

namespace gef {

void _addAndExecCmd(ICommand* cmd) {
	cmd->exec();
	ParamList par;
	par.push_back(Obj(cmd));
	gService->bubble(MGT_EDIT, MET_EDIT_ADD_CMD, par);
}

void _onResHwndFunc(MsgGroup g, MsgType t, const ParamList &p) {
	ConstStr const full = p.at(0).getStr();
	ConstStr const part = p.at(1).getStr();
	{ full; part; }
	union { HWND hwnd; Ptr ptr; } u;
	u.ptr = p.at(2).getPtr();
	hWnd = u.hwnd;
	s32 w = p.at(3);
	s32 h = p.at(4);
	_killGlWindow();
	_createGlWindow(w, h, 0);

	gService->echo("Service test case GL context created.");
}

void _onTimerFunc(MsgGroup g, MsgType t, const ParamList &p) {
	if(!hWnd) return;

	_drawGlScene();
	SwapBuffers(hDC);
}

void _onResizeFunc(MsgGroup g, MsgType t, const ParamList &p) {
	ConstStr const full = p.at(0).getStr();
	ConstStr const part = p.at(1).getStr();
	{ full; part; }
	if(strcmp(part, "Canvas") != 0) return;
	wndW = p.at(2);
	wndH = p.at(3);

	if(!hWnd) return;

	_resizeGlScene(wndW, wndH);
}

void _onTestFunc(MsgGroup g, MsgType t, const ParamList &p) {
	StdStr s;
	ParamList::const_iterator begin = p.begin();
	union { HWND hwnd; Ptr ptr; } u;
	u.ptr = begin->getPtr();
	++begin;
	for(ParamList::const_iterator it = begin; it != p.end(); ++it)
		s += it->toStr();
	MessageBoxA(u.hwnd, s.c_str(), "GEF", MB_OK);
}

void _onTestSetClearColorFunc(MsgGroup g, MsgType t, const ParamList &p) {
	f32 _r = p.at(0);
	f32 _g = p.at(1);
	f32 _b = p.at(2);
	clearColor.x = _r;
	clearColor.y = _g;
	clearColor.z = _b;
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
}

void _onTestPreAddQuadFunc(MsgGroup g, MsgType t, const ParamList &p) {
	usePreAddQuad = true;
	s32 sx = p.at(0);
	s32 sy = p.at(1);
	s32 cr = p.at(2);
	s32 cg = p.at(3);
	s32 cb = p.at(4);
	preAddQuad.position.x = WNDX2VIEW(sx);
	preAddQuad.position.y = WNDY2VIEW(sy);
	preAddQuad.size.x = 0.0f;
	preAddQuad.size.y = 0.0f;
	preAddQuad.color.x = cr / 255.0f;
	preAddQuad.color.y = cg / 255.0f;
	preAddQuad.color.z = cb / 255.0f;
}

void _onTestSetPreAddedQuadSize(MsgGroup g, MsgType t, const ParamList &p) {
	s32 ex = p.at(0);
	s32 ey = p.at(1);
	preAddQuad.size.x = WNDX2VIEW(ex) - preAddQuad.position.x;
	preAddQuad.size.y = WNDY2VIEW(ey) - preAddQuad.position.y;
}

void _onTestAddQuadFunc(MsgGroup g, MsgType t, const ParamList &p) {
	ICommand* cmd = CreateGefCommandByNum(MTT_TEST_ADD_QUAD);
	cmd->setParam("ID", Obj(_genQuadId()));
	cmd->setParam("POS_X", Obj(preAddQuad.position.x));
	cmd->setParam("POS_Y", Obj(preAddQuad.position.y));
	cmd->setParam("SIZE_X", Obj(preAddQuad.size.x));
	cmd->setParam("SIZE_Y", Obj(preAddQuad.size.y));
	cmd->setParam("COL_R", Obj(preAddQuad.color.x));
	cmd->setParam("COL_G", Obj(preAddQuad.color.y));
	cmd->setParam("COL_B", Obj(preAddQuad.color.z));
	_addAndExecCmd(cmd);
	usePreAddQuad = false;
}

void _onTestRemoveQuadFunc(MsgGroup g, MsgType t, const ParamList &p) {
	s32 sx = p.at(0);
	s32 sy = p.at(1);
	f32 vx = WNDX2VIEW(sx);
	f32 vy = WNDY2VIEW(sy);
	Quad* q = _pickQuad(vx, vy);
	if(!q) return;
	ICommand* cmd = CreateGefCommandByNum(MTT_TEST_REMOVE_QUAD);
	cmd->setParam("ID", Obj(q->id));
	cmd->setParam("POS_X", Obj(q->position.x));
	cmd->setParam("POS_Y", Obj(q->position.y));
	cmd->setParam("SIZE_X", Obj(q->size.x));
	cmd->setParam("SIZE_Y", Obj(q->size.y));
	cmd->setParam("COL_R", Obj(q->color.x));
	cmd->setParam("COL_G", Obj(q->color.y));
	cmd->setParam("COL_B", Obj(q->color.z));
	_addAndExecCmd(cmd);
}

void _onTestPreMoveQuad(MsgGroup g, MsgType t, const ParamList &p) {
	s32 sx = p.at(0);
	s32 sy = p.at(1);
	f32 vx = WNDX2VIEW(sx);
	f32 vy = WNDY2VIEW(sy);
	Quad* q = _pickQuad(vx, vy);
	if(!q) return;
	movingContext.movingQuad = new Quad;
	*movingContext.movingQuad = *q;
	movingContext.mousePoint.x = vx;
	movingContext.mousePoint.y = vy;
	movingContext.oldPos = q->position;
}

void _onTestMovingQuad(MsgGroup g, MsgType t, const ParamList &p) {
	if(!movingContext.movingQuad) return;
	s32 ex = p.at(0);
	s32 ey = p.at(1);
	f32 vx = WNDX2VIEW(ex);
	f32 vy = WNDY2VIEW(ey);
	f32 dx = vx - movingContext.mousePoint.x;
	f32 dy = vy - movingContext.mousePoint.y;
	movingContext.mousePoint.x = vx;
	movingContext.mousePoint.y = vy;
	movingContext.movingQuad->position.x += dx;
	movingContext.movingQuad->position.y += dy;
	for(QuadColl::iterator it = quads.begin(); it != quads.end(); ++it) {
		if(it->id == movingContext.movingQuad->id) {
			it->position = movingContext.movingQuad->position;
			break;
		}
	}
}

void _onTestMoveQuadFunc(MsgGroup g, MsgType t, const ParamList &p) {
	if(!movingContext.movingQuad) return;
	ICommand* cmd = CreateGefCommandByNum(MTT_TEST_MOVE_QUAD);
	cmd->setParam("ID", Obj(movingContext.movingQuad->id));
	cmd->setParam("POS_X", Obj(movingContext.movingQuad->position.x));
	cmd->setParam("POS_Y", Obj(movingContext.movingQuad->position.y));
	cmd->setParam("OLD_POS_X", Obj(movingContext.oldPos.x));
	cmd->setParam("OLD_POS_Y", Obj(movingContext.oldPos.y));
	_addAndExecCmd(cmd);
	delete movingContext.movingQuad;
	movingContext.movingQuad = NULL;
}

}
