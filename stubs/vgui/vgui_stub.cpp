//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "vgui_stub/vgui_stub.h"
#include "vgui_export.h"
#include "dar.h"

#include <stdio.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#pragma warning( disable: 4100 )

namespace vgui
{
	VGUIAPI void vgui_strcpy(char* dst, int dstLen, const char* src)
	{
	}

	VGUIAPI char* vgui_strdup(const char* src)
	{
		return nullptr;
	}

	class VGUIAPI App;
	class VGUIAPI BaseFontPlat;
	class VGUIAPI Bitmap;
	//class VGUIAPI BitmapTGA;
	class VGUIAPI Border;
	//class VGUIAPI BorderLayout;
	class VGUIAPI BuildGroup;
	//class VGUIAPI Button;
	class VGUIAPI ButtonController;
	class VGUIAPI ButtonGroup;
	//class VGUIAPI Color;
	class VGUIAPI Cursor;
	//class VGUIAPI DataInputStream;
	class VGUIAPI Desktop;
	//class VGUIAPI DesktopIcon;
	//class VGUIAPI FileInputStream;
	class VGUIAPI FocusChangeSignal;
	class VGUIAPI FocusNavGroup;
	class VGUIAPI Font;
	//class VGUIAPI Frame;
	//class VGUIAPI HeaderPanel;
	//class VGUIAPI Image;
	class VGUIAPI ImagePanel;
	//class VGUIAPI InputSignal;
	//class VGUIAPI InputStream;
	class VGUIAPI Label;
	//class VGUIAPI Layout;
	class VGUIAPI LayoutInfo;
	//class VGUIAPI LineBorder;
	//class VGUIAPI LoweredBorder;
	//class VGUIAPI MenuSeparator;
	//class VGUIAPI MiniApp;
	//class VGUIAPI Panel;
	class VGUIAPI RepaintSignal;
	//class VGUIAPI Scheme;
	//class VGUIAPI ScrollBar;
	//class VGUIAPI ScrollPanel;
	//class VGUIAPI Slider;
	//class VGUIAPI StackLayout;
	//class VGUIAPI Surface;
	//class VGUIAPI SurfaceBase;
	//class VGUIAPI TablePanel;
	//class VGUIAPI TextEntry;
	//class VGUIAPI TextGrid;
	//class VGUIAPI TextImage;
	//class VGUIAPI TextPanel;

	enum VGUIAPI KeyCode
	{
		KEY_0 = 0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		KEY_PAD_0,
		KEY_PAD_1,
		KEY_PAD_2,
		KEY_PAD_3,
		KEY_PAD_4,
		KEY_PAD_5,
		KEY_PAD_6,
		KEY_PAD_7,
		KEY_PAD_8,
		KEY_PAD_9,
		KEY_PAD_DIVIDE,
		KEY_PAD_MULTIPLY,
		KEY_PAD_MINUS,
		KEY_PAD_PLUS,
		KEY_PAD_ENTER,
		KEY_PAD_DECIMAL,
		KEY_LBRACKET,
		KEY_RBRACKET,
		KEY_SEMICOLON,
		KEY_APOSTROPHE,
		KEY_BACKQUOTE,
		KEY_COMMA,
		KEY_PERIOD,
		KEY_SLASH,
		KEY_BACKSLASH,
		KEY_MINUS,
		KEY_EQUAL,
		KEY_ENTER,
		KEY_SPACE,
		KEY_BACKSPACE,
		KEY_TAB,
		KEY_CAPSLOCK,
		KEY_NUMLOCK,
		KEY_ESCAPE,
		KEY_SCROLLLOCK,
		KEY_INSERT,
		KEY_DELETE,
		KEY_HOME,
		KEY_END,
		KEY_PAGEUP,
		KEY_PAGEDOWN,
		KEY_BREAK,
		KEY_LSHIFT,
		KEY_RSHIFT,
		KEY_LALT,
		KEY_RALT,
		KEY_LCONTROL,
		KEY_RCONTROL,
		KEY_LWIN,
		KEY_RWIN,
		KEY_APP,
		KEY_UP,
		KEY_LEFT,
		KEY_DOWN,
		KEY_RIGHT,
		KEY_F1,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_F11,
		KEY_F12,
		KEY_LAST,
	};

	enum VGUIAPI MouseCode
	{
		MOUSE_LEFT = 0,
		MOUSE_RIGHT,
		MOUSE_MIDDLE,
		MOUSE_LAST,
	};


	class VGUIAPI InputStream
	{
	public:
		virtual void  seekStart(bool& success) = 0;
		virtual void  seekRelative(int count, bool& success) = 0;
		virtual void  seekEnd(bool& success) = 0;
		virtual int   getAvailable(bool& success) = 0;
		virtual uchar readUChar(bool& success) = 0;
		virtual void  readUChar(uchar* buf, int count, bool& success) = 0;
		virtual void  close(bool& success) = 0;
	};

	class VGUIAPI DataInputStream : public InputStream
	{
	private:
		InputStream* _is;
	public:
		DataInputStream(InputStream* is) {}
	public:
		virtual void  seekStart(bool& success) {}
		virtual void  seekRelative(int count, bool& success) {}
		virtual void  seekEnd(bool& success) {}
		virtual int   getAvailable(bool& success) { return 0; }
		virtual void  readUChar(uchar* buf, int count, bool& success) {}
		virtual void  close(bool& success) {}
		virtual void  close() {}
	public:
		virtual bool   readBool(bool& success) { return false; }
		virtual char   readChar(bool& success) { return '\0'; }
		virtual uchar  readUChar(bool& success) { return '\0'; }
		virtual short  readShort(bool& success) { return 0; }
		virtual ushort readUShort(bool& success) { return 0; }
		virtual int    readInt(bool& success) { return 0; }
		virtual uint   readUInt(bool& success) { return 0; }
		virtual long   readLong(bool& success) { return 0; }
		virtual ulong  readULong(bool& success) { return 0; }
		virtual float  readFloat(bool& success) { return 0; }
		virtual double readDouble(bool& success) { return 0; }
		virtual void   readLine(char* buf, int bufLen, bool& success) {}
	};

	class VGUIAPI FileInputStream : public InputStream
	{
	private:
		FILE* _fp;
	public:
		FileInputStream(const char* fileName, bool textMode) {}
	public:
		virtual void  seekStart(bool& success) {}
		virtual void  seekRelative(int count, bool& success) {}
		virtual void  seekEnd(bool& success) {}
		virtual int   getAvailable(bool& success) { return 0; }
		virtual uchar readUChar(bool& success) { return '\0'; }
		virtual void  readUChar(uchar* buf, int count, bool& success) {}
		virtual void  close(bool& success) {}
		virtual void  close() {}
	};


	class VGUIAPI Scheme
	{
	public:
		enum SchemeColor
		{
			sc_user = 0,
			sc_black,
			sc_white,
			sc_primary1,
			sc_primary2,
			sc_primary3,
			sc_secondary1,
			sc_secondary2,
			sc_secondary3,
			sc_last,
		};
		enum SchemeFont
		{
			sf_user = 0,
			sf_primary1,
			sf_primary2,
			sf_primary3,
			sf_secondary1,
			sf_last,
		};
		enum SchemeCursor
		{
			scu_user = 0,
			scu_none,
			scu_arrow,
			scu_ibeam,
			scu_hourglass,
			scu_crosshair,
			scu_up,
			scu_sizenwse,
			scu_sizenesw,
			scu_sizewe,
			scu_sizens,
			scu_sizeall,
			scu_no,
			scu_hand,
			scu_last,
		};
	public:
		Scheme() {}
	public:
		virtual void    setColor(SchemeColor sc, int r, int g, int b, int a) {}
		virtual void    getColor(SchemeColor sc, int& r, int& g, int& b, int& a) {}
		virtual void    setFont(SchemeFont sf, Font* font) {}
		virtual Font* getFont(SchemeFont sf) { return NULL; }
		virtual void    setCursor(SchemeCursor sc, Cursor* cursor) {}
		virtual Cursor* getCursor(SchemeCursor sc) { return NULL; }
	protected:
		int     _color[sc_last][4];
		Font* _font[sf_last];
		Cursor* _cursor[scu_last];
	};


	class VGUIAPI Color
	{
	private:
		uchar               _color[4];
		Scheme::SchemeColor _schemeColor;
	public:
		Color() {}
		Color(int r, int g, int b, int a) {}
		Color(Scheme::SchemeColor sc) {}
	private:
		virtual void init() {}
	public:
		virtual void setColor(int r, int g, int b, int a) {}
		virtual void setColor(Scheme::SchemeColor sc) {}
		virtual void getColor(int& r, int& g, int& b, int& a) {}
		virtual void getColor(Scheme::SchemeColor& sc) {}
		virtual int  operator[](int index) { return 0; }
	};

	class VGUIAPI Cursor
	{
	public:
		enum DefaultCursor
		{
			dc_user,
			dc_none,
			dc_arrow,
			dc_ibeam,
			dc_hourglass,
			dc_crosshair,
			dc_up,
			dc_sizenwse,
			dc_sizenesw,
			dc_sizewe,
			dc_sizens,
			dc_sizeall,
			dc_no,
			dc_hand,
			dc_last,
		};
	private:
		int           _hotspot[2];
		Bitmap* _bitmap;
		DefaultCursor _dc;
	public:
		Cursor(DefaultCursor dc) {}
		Cursor(Bitmap* bitmap, int hotspotX, int hotspotY) {}
	public:
		virtual void getHotspot(int& x, int& y) {}
	private:
		virtual void privateInit(Bitmap* bitmap, int hotspotX, int hotspotY) {}
	public:
		virtual Bitmap* getBitmap() { return NULL; }
		virtual DefaultCursor getDefaultCursor() { return dc_arrow; }
	};

	class VGUIAPI Font
	{
	public:
		Font(const char* name, int tall, int wide, float rotation, int weight, bool italic, bool underline, bool strikeout, bool symbol) {}
		// If pFileData is non-NULL, then it will try to load the 32-bit (RLE) TGA file. If that fails,
		// it will create the font using the specified parameters.
		// pUniqueName should be set if pFileData and fileDataLen are set so it can determine if a font is already loaded.
		Font(const char* name, void* pFileData, int fileDataLen, int tall, int wide, float rotation, int weight, bool italic, bool underline, bool strikeout, bool symbol) {}
	private:
		virtual void init(const char* name, void* pFileData, int fileDataLen, int tall, int wide, float rotation, int weight, bool italic, bool underline, bool strikeout, bool symbol) {}
	public:
		BaseFontPlat* getPlat() { return NULL; }
		virtual void getCharRGBA(int ch, int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, uchar* rgba) {}
		virtual void getCharABCwide(int ch, int& a, int& b, int& c) {}
		virtual void getTextSize(const char* text, int& wide, int& tall) {}
		virtual int  getTall() { return 0; }
#ifndef _WIN32
		virtual int getWide() { return 0; }
#endif
		virtual int  getId() { return 0; }
	protected:
		char* _name;
		BaseFontPlat* _plat;
		int			_id;
	};

	static Font s_StubFont( "", 0, 0, 0, 0, false, false, false, false );

	class VGUIAPI SurfaceBase
	{
	public:
		SurfaceBase(Panel* embeddedPanel) {}
	protected:
		~SurfaceBase() {}
	public:
		virtual Panel* getPanel() { return NULL; }
		virtual void   requestSwap() {}
		virtual void   resetModeInfo() {}
		virtual int    getModeInfoCount() { return 0; }
		virtual bool   getModeInfo(int mode, int& wide, int& tall, int& bpp) { return false; }
		virtual App* getApp() { return NULL; }
		virtual void   setEmulatedCursorVisible(bool state) {}
		virtual void   setEmulatedCursorPos(int x, int y) {}
	public:
		virtual void setTitle(const char* title) = 0;
		virtual bool setFullscreenMode(int wide, int tall, int bpp) = 0;
		virtual void setWindowedMode() = 0;
		virtual void setAsTopMost(bool state) = 0;
		virtual void createPopup(Panel* embeddedPanel) = 0;
		virtual bool hasFocus() = 0;
		virtual bool isWithin(int x, int y) = 0;
		virtual int  createNewTextureID(void) = 0;
		virtual void GetMousePos(int& x, int& y) = 0;
	protected:
		virtual void addModeInfo(int wide, int tall, int bpp) {}
	protected:
		virtual void drawSetColor(int r, int g, int b, int a) = 0;
		virtual void drawFilledRect(int x0, int y0, int x1, int y1) = 0;
		virtual void drawOutlinedRect(int x0, int y0, int x1, int y1) = 0;
		virtual void drawSetTextFont(Font* font) = 0;
		virtual void drawSetTextColor(int r, int g, int b, int a) = 0;
		virtual void drawSetTextPos(int x, int y) = 0;
		virtual void drawPrintText(const char* text, int textLen) = 0;
		virtual void drawSetTextureRGBA(int id, const char* rgba, int wide, int tall) = 0;
		virtual void drawSetTexture(int id) = 0;
		virtual void drawTexturedRect(int x0, int y0, int x1, int y1) = 0;
		virtual void invalidate(Panel* panel) = 0;
		virtual void enableMouseCapture(bool state) = 0;
		virtual void setCursor(Cursor* cursor) = 0;
		virtual void swapBuffers() = 0;
		virtual void pushMakeCurrent(Panel* panel, bool useInsets) = 0;
		virtual void popMakeCurrent(Panel* panel) = 0;
		virtual void applyChanges() = 0;
	protected:
		bool       _needsSwap;
		App* _app;
		Panel* _embeddedPanel;
		Dar<char*> _modeInfoDar;
		ImagePanel* _emulatedCursor;
		Cursor* _currentCursor;
	};

	class VGUIAPI InputSignal
	{
	public:
		virtual void cursorMoved(int x, int y, Panel* panel) = 0;
		virtual void cursorEntered(Panel* panel) = 0;
		virtual void cursorExited(Panel* panel) = 0;
		virtual void mousePressed(MouseCode code, Panel* panel) = 0;
		virtual void mouseDoublePressed(MouseCode code, Panel* panel) = 0;
		virtual void mouseReleased(MouseCode code, Panel* panel) = 0;
		virtual void mouseWheeled(int delta, Panel* panel) = 0;
		virtual void keyPressed(KeyCode code, Panel* panel) = 0;
		virtual void keyTyped(KeyCode code, Panel* panel) = 0;
		virtual void keyReleased(KeyCode code, Panel* panel) = 0;
		virtual void keyFocusTicked(Panel* panel) = 0;
	};

	class VGUIAPI Layout
	{
	public:
		Layout() {}
	public:
		virtual void performLayout(Panel* panel) {}
	};

	class VGUIAPI BorderLayout : public Layout
	{
	public:
		enum Alignment
		{
			a_center = 0,
			a_north,
			a_south,
			a_east,
			a_west,
		};
	private:
		int _inset;
	public:
		BorderLayout(int inset) {}
	public:
		virtual void        performLayout(Panel* panel) {}
		virtual LayoutInfo* createLayoutInfo(Alignment alignment) { return NULL; }
	};

	class VGUIAPI Panel
	{
	public:
		Panel() {}
		Panel(int x, int y, int wide, int tall)
		{
			init(x, y, wide, tall);
		}
	private:
		void init(int x, int y, int wide, int tall) {}
	public:
		virtual void setPos(int x, int y) {}
		virtual void getPos(int& x, int& y) {}
		virtual void setSize(int wide, int tall) {}
		virtual void getSize(int& wide, int& tall) {}
		virtual void setBounds(int x, int y, int wide, int tall) {}
		virtual void getBounds(int& x, int& y, int& wide, int& tall) {}
		virtual int getWide() { return 0; }
		virtual int getTall() { return 0; }
		virtual Panel* getParent() { return nullptr; }
		virtual void setVisible(bool state) {}
		virtual bool isVisible() { return false; }
		virtual bool isVisibleUp() { return false; }
		virtual void repaint() {}
		virtual void repaintAll() {}
		virtual void getAbsExtents(int& x0, int& y0, int& x1, int& y1) {}
		virtual void getClipRect(int& x0, int& y0, int& x1, int& y1) {}
		virtual void setParent(Panel* newParent) {}
		virtual void addChild(Panel* child) {}
		virtual void insertChildAt(Panel* child, int index) {}
		virtual void removeChild(Panel* child) {}
		virtual bool wasMousePressed(MouseCode code) { return false; }
		virtual bool wasMouseDoublePressed(MouseCode code) { return false; }
		virtual bool isMouseDown(MouseCode code) { return false; }
		virtual bool wasMouseReleased(MouseCode code) { return false; }
		virtual bool wasKeyPressed(KeyCode code) { return false; }
		virtual bool isKeyDown(KeyCode code) { return false; }
		virtual bool wasKeyTyped(KeyCode code) { return false; }
		virtual bool wasKeyReleased(KeyCode code) { return false; }
		virtual void addInputSignal(InputSignal* s) {}
		virtual void removeInputSignal(InputSignal* s) {}
		virtual void addRepaintSignal(RepaintSignal* s) {}
		virtual void removeRepaintSignal(RepaintSignal* s) {}
		virtual bool isWithin(int x,int y) { return false; }
		virtual Panel* isWithinTraverse(int x,int y) { return nullptr; }
		virtual void localToScreen(int& x,int& y) {}
		virtual void screenToLocal(int& x,int& y) {}
		virtual void setCursor(Cursor* cursor) {}
		virtual void setCursor(Scheme::SchemeCursor scu) {}
		virtual Cursor* getCursor() { return nullptr; }
		virtual void setMinimumSize(int wide,int tall) {}
		virtual void getMinimumSize(int& wide,int& tall) {}
		virtual void requestFocus() {}
		virtual bool hasFocus() { return false; }
		virtual int getChildCount() { return 0; }
		virtual Panel* getChild(int index) { return nullptr; }
		virtual void setLayout(Layout* layout) {}
		virtual void invalidateLayout(bool layoutNow) {}
		virtual void setFocusNavGroup(FocusNavGroup* focusNavGroup) {}
		virtual void requestFocusPrev() {}
		virtual void requestFocusNext() {}
		virtual void addFocusChangeSignal(FocusChangeSignal* s) {}
		virtual bool isAutoFocusNavEnabled() { return false; }
		virtual void setAutoFocusNavEnabled(bool state) {}
		virtual void setBorder(Border* border) {}
		virtual void setPaintBorderEnabled(bool state) {}
		virtual void setPaintBackgroundEnabled(bool state) {}
		virtual void setPaintEnabled(bool state) {}
		virtual void getInset(int& left, int& top, int& right, int& bottom) {}
		virtual void getPaintSize(int& wide, int& tall) {}
		virtual void setPreferredSize(int wide, int tall) {}
		virtual void getPreferredSize(int& wide, int& tall) {}
		virtual SurfaceBase* getSurfaceBase() { return nullptr; }
		virtual bool isEnabled() { return false; }
		virtual void setEnabled(bool state) {}
		virtual void setBuildGroup(BuildGroup* buildGroup, const char* panelPersistanceName) {}
		virtual bool isBuildGroupEnabled() { return false; }
		virtual void removeAllChildren() {}
		virtual void repaintParent() {}
		virtual Panel* createPropertyPanel() { return nullptr; }
		virtual void getPersistanceText(char* buf, int bufLen) {}
		virtual void applyPersistanceText(const char* buf) {}
		virtual void setFgColor(Scheme::SchemeColor sc) {}
		virtual void setBgColor(Scheme::SchemeColor sc) {}
		virtual void setFgColor(int r, int g, int b, int a) {}
		virtual void setBgColor(int r, int g, int b, int a) {}
		virtual void getFgColor(int& r, int& g, int& b, int& a) {}
		virtual void getBgColor(int& r, int& g, int& b, int& a) {}
		virtual void setBgColor(Color color) {}
		virtual void setFgColor(Color color) {}
		virtual void getBgColor(Color& color) {}
		virtual void getFgColor(Color& color) {}
		virtual void setAsMouseCapture(bool state) {}
		virtual void setAsMouseArena(bool state) {}
		virtual App* getApp() { return nullptr; }
		virtual void getVirtualSize(int& wide, int& tall) {}
		virtual void setLayoutInfo(LayoutInfo* layoutInfo) {}
		virtual LayoutInfo* getLayoutInfo() { return nullptr; }
		virtual bool isCursorNone() { return nullptr; }
	public: //bullshit public
		virtual void solveTraverse() {}
		virtual void paintTraverse() {}
		virtual void setSurfaceBaseTraverse(SurfaceBase* surfaceBase) {}
	protected:
		virtual void performLayout() {}
		virtual void internalPerformLayout() {}
		virtual void drawSetColor(Scheme::SchemeColor sc) {}
		virtual void drawSetColor(int r, int g, int b, int a) {}
		virtual void drawFilledRect(int x0, int y0, int x1, int y1) {}
		virtual void drawOutlinedRect(int x0, int y0, int x1, int y1) {}
		virtual void drawSetTextFont(Scheme::SchemeFont sf) {}
		virtual void drawSetTextFont(Font* font) {}
		virtual void drawSetTextColor(Scheme::SchemeColor sc) {}
		virtual void drawSetTextColor(int r, int g, int b, int a) {}
		virtual void drawSetTextPos(int x, int y) {}
		virtual void drawPrintText(const char* str, int strlen) {}
		virtual void drawPrintText(int x, int y, const char* str, int strlen) {}
		virtual void drawPrintChar(char ch) {}
		virtual void drawPrintChar(int x, int y, char ch) {}
		virtual void drawSetTextureRGBA(int id, const char* rgba, int wide, int tall) {}
		virtual void drawSetTexture(int id) {}
		virtual void drawTexturedRect(int x0, int y0, int x1, int y1) {}
		virtual void solve() {}
		virtual void paintTraverse(bool repaint) {}
		virtual void paintBackground() {}
		virtual void paint() {}
		virtual void paintBuildOverlay() {}
		virtual void internalCursorMoved(int x, int y) {}
		virtual void internalCursorEntered() {}
		virtual void internalCursorExited() {}
		virtual void internalMousePressed(MouseCode code) {}
		virtual void internalMouseDoublePressed(MouseCode code) {}
		virtual void internalMouseReleased(MouseCode code) {}
		virtual void internalMouseWheeled(int delta) {}
		virtual void internalKeyPressed(KeyCode code) {}
		virtual void internalKeyTyped(KeyCode code) {}
		virtual void internalKeyReleased(KeyCode code) {}
		virtual void internalKeyFocusTicked() {}
		virtual void internalFocusChanged(bool lost) {}
		virtual void internalSetCursor() {}
	protected:
		int _pos[2];
		int _size[2];
		int _loc[2];
		int _minimumSize[2];
		int _preferredSize[2];
		Dar<Panel*>_childDar;
		Panel* _parent;
		SurfaceBase* _surfaceBase;
		Dar<InputSignal*> _inputSignalDar;
		Dar<RepaintSignal*> _repaintSignalDar;
		int _clipRect[4];
		Cursor* _cursor;
		Scheme::SchemeCursor _schemeCursor;
		bool _visible;
		Layout* _layout;
		bool _needsLayout;
		FocusNavGroup* _focusNavGroup;
		Dar<FocusChangeSignal*> _focusChangeSignalDar;
		bool _autoFocusNavEnabled;
		Border* _border;
	private:
		bool _needsRepaint;
		bool _enabled;
		BuildGroup* _buildGroup;
		Color _fgColor;
		Color _bgColor;
		LayoutInfo* _layoutInfo;
		bool _paintBorderEnabled;
		bool _paintBackgroundEnabled;
		bool _paintEnabled;
	};

	static Panel s_StubPanel;

	class VGUIAPI StackLayout : public Layout
	{
	private:
		int  _vgap;
		bool _fitWide;
	public:
		StackLayout(int vgap, bool fitWide) {}
	public:
		virtual void performLayout(Panel* panel) {}
	};

	class VGUIAPI Image
	{
	public:
		virtual void setPos(int x, int y) {}
		virtual void getPos(int& x, int& y) {}
		virtual void getSize(int& wide, int& tall) {}
		virtual void setColor(Color color) {}
		virtual void getColor(Color& color) {}
	protected:
		virtual void setSize(int wide, int tall) {}
		virtual void drawSetColor(Scheme::SchemeColor sc) {}
		virtual void drawSetColor(int r, int g, int b, int a) {}
		virtual void drawFilledRect(int x0, int y0, int x1, int y1) {}
		virtual void drawOutlinedRect(int x0, int y0, int x1, int y1) {}
		virtual void drawSetTextFont(Scheme::SchemeFont sf) {}
		virtual void drawSetTextFont(Font* font) {}
		virtual void drawSetTextColor(Scheme::SchemeColor sc) {}
		virtual void drawSetTextColor(int r, int g, int b, int a) {}
		virtual void drawSetTextPos(int x, int y) {}
		virtual void drawPrintText(const char* str, int strlen) {}
		virtual void drawPrintText(int x, int y, const char* str, int strlen) {}
		virtual void drawPrintChar(char ch) {}
		virtual void drawPrintChar(int x, int y, char ch) {}
		virtual void drawSetTextureRGBA(int id, const char* rgba, int wide, int tall) {}
		virtual void drawSetTexture(int id) {}
		virtual void drawTexturedRect(int x0, int y0, int x1, int y1) {}
		virtual void paint(Panel* panel) {}
	public:
		virtual void doPaint(Panel* panel) {}
	};

	class VGUIAPI Bitmap : public Image
	{
	private:
		int         _id;
		bool        _uploaded;
	public:
		Bitmap() {}
	protected:
		virtual void setSize(int wide, int tall) {}
		virtual void setRGBA(int x, int y, uchar r, uchar g, uchar b, uchar a) {}
	public:
		virtual void paint(Panel* panel) {}
	protected:
		uchar* _rgba;
	};

	class VGUIAPI BitmapTGA : public Bitmap
	{
	public:
		BitmapTGA(InputStream* is, bool invertAlpha) {}
	private:
		virtual bool loadTGA(InputStream* is, bool invertAlpha) { return false; }
	};

	class VGUIAPI Border : public Image
	{
	public:
		Border() {}
		Border(int left, int top, int right, int bottom) {}
	public:
		virtual void setInset(int left, int top, int right, int bottom) {}
		virtual void getInset(int& left, int& top, int& right, int& bottom) {}
	protected:
		virtual void drawFilledRect(int x0, int y0, int x1, int y1) {}
		virtual void drawOutlinedRect(int x0, int y0, int x1, int y1) {}
		virtual void drawSetTextPos(int x, int y) {}
		virtual void drawPrintText(int x, int y, const char* str, int strlen) {}
		virtual void drawPrintChar(int x, int y, char ch) {}
	protected:
		int _inset[4];
	private:
		Panel* _panel;
	};

	class VGUIAPI LoweredBorder : public Border
	{
	public:
		LoweredBorder() {}
	protected:
		virtual void paint(Panel* panel) {}
	};

	class VGUIAPI TextImage : public Image
	{
	private:
		char* _text;
		int                       _textBufferLen;
		vgui::Scheme::SchemeFont  _schemeFont;
		vgui::Font* _font;
		int                       _textColor[4];
		vgui::Scheme::SchemeColor _textSchemeColor;
	public:
		TextImage(int textBufferLen, const char* text) {}
		TextImage(const char* text) {}
	private:
		virtual void  init(int textBufferLen, const char* text) {}
	public:
		virtual void  getTextSize(int& wide, int& tall) {}
		virtual void  getTextSizeWrapped(int& wide, int& tall) {}
		virtual Font* getFont() { return &s_StubFont; }
		virtual void  setText(int textBufferLen, const char* text) {}
		virtual void  setText(const char* text) {}
		virtual void  setFont(vgui::Scheme::SchemeFont schemeFont) {}
		virtual void  setFont(vgui::Font* font) {}
		virtual void  setSize(int wide, int tall) {}
	protected:
		virtual void paint(Panel* panel) {}
	};

	class VGUIAPI ImagePanel : public Panel
	{
	public:
		inline ImagePanel()
		{
			_image = NULL;
		}

		ImagePanel(Image* image) {}
	public:
		virtual void setImage(Image* image) {}
	protected:
		virtual void paintBackground() {}
	protected:
		Image* _image;
	};

	class VGUIAPI Label : public Panel
	{
	public:
		enum Alignment
		{
			a_northwest = 0,
			a_north,
			a_northeast,
			a_west,
			a_center,
			a_east,
			a_southwest,
			a_south,
			a_southeast,
		};
	public:
		Label(int textBufferLen, const char* text, int x, int y, int wide, int tall) {}
		Label(const char* text, int x, int y, int wide, int tall) {}
		Label(const char* text, int x, int y) {}
		Label(const char* text) {}

		inline Label() : Panel(0, 0, 10, 10)
		{
			init(1, "", true);
		}
	private:
		void init(int textBufferLen, const char* text, bool textFitted) {}
	public:
		virtual void setImage(Image* image) {}
		virtual void setText(int textBufferLen, const char* text) {}
		virtual void setText(const char* format, ...) {}
		virtual void setFont(Scheme::SchemeFont schemeFont) {}
		virtual void setFont(Font* font) {}
		virtual void getTextSize(int& wide, int& tall) {}
		virtual void getContentSize(int& wide, int& tall) {}
		virtual void setTextAlignment(Alignment alignment) {}
		virtual void setContentAlignment(Alignment alignment) {}
		virtual Panel* createPropertyPanel() { return nullptr; }
		virtual void setFgColor(int r, int g, int b, int a) {}
		virtual void setFgColor(vgui::Scheme::SchemeColor sc) {}
		virtual void setContentFitted(bool state) {}
	protected:
		virtual void computeAlignment(int& tx0, int& ty0, int& tx1, int& ty1, int& ix0, int& iy0, int& ix1, int& iy1, int& minX, int& minY, int& maxX, int& maxY) {}
		virtual void paint() {}
		virtual void recomputeMinimumSize() {}
	protected:
		bool       _textEnabled;
		bool       _imageEnabled;
		bool       _contentFitted;
		Alignment  _textAlignment;
		Alignment  _contentAlignment;
		TextImage* _textImage;
		Image* _image;
	};

	class VGUIAPI Button : public Label
	{
	public:
		Button(const char* text, int x, int y, int wide, int tall) {}
		Button(const char* text, int x, int y) {}
	private:
		void init() {}
	public:
		virtual void setSelected(bool state) {}
		virtual void setSelectedDirect(bool state) {}
		virtual void setArmed(bool state) {}
		virtual bool isSelected() { return false; }
		virtual void doClick() {}
		virtual void addActionSignal(ActionSignal* s) {}
		virtual void setButtonGroup(ButtonGroup* buttonGroup) {}
		virtual bool isArmed() { return false; }
		virtual void setButtonBorderEnabled(bool state) {}
		virtual void setMouseClickEnabled(MouseCode code, bool state) {}
		virtual bool isMouseClickEnabled(MouseCode code) { return false; }
		virtual void fireActionSignal() {}
		virtual Panel* createPropertyPanel() { return NULL; }
	protected:
		virtual void setButtonController(ButtonController* _buttonController) {}
		virtual void paintBackground() {}
	protected:
		char* _text;
		bool               _armed;
		bool               _selected;
		bool               _buttonBorderEnabled;
		Dar<ActionSignal*> _actionSignalDar;
		int                _mouseClickMask;
		ButtonGroup* _buttonGroup;
		ButtonController* _buttonController;
	};

	class VGUIAPI Frame : public Panel
	{
	public:
		Frame(int x, int y, int wide, int tall) {}
	public:
		virtual void   setSize(int wide, int tall) {}
		virtual void   setInternal(bool state) {}
		virtual void   paintBackground() {}
		virtual bool   isInternal() { return false; }
		virtual Panel* getClient() { return NULL; }
		virtual void   setTitle(const char* title) {}
		virtual void   getTitle(char* buf, int bufLen) {}
		virtual void	 setMoveable(bool state) {}
		virtual void   setSizeable(bool state) {}
		virtual bool   isMoveable() { return false; }
		virtual bool   isSizeable() { return false; }
		virtual void   addFrameSignal(FrameSignal* s) {}
		virtual void   setVisible(bool state) {}
		virtual void   setMenuButtonVisible(bool state) {}
		virtual void   setTrayButtonVisible(bool state) {}
		virtual void   setMinimizeButtonVisible(bool state) {}
		virtual void   setMaximizeButtonVisible(bool state) {}
		virtual void   setCloseButtonVisible(bool state) {}
	public: //bullshit public
		virtual void fireClosingSignal() {}
		virtual void fireMinimizingSignal() {}
	protected:
		char* _title;
		bool              _internal;
		bool              _sizeable;
		bool              _moveable;
		Panel* _topGrip;
		Panel* _bottomGrip;
		Panel* _leftGrip;
		Panel* _rightGrip;
		Panel* _topLeftGrip;
		Panel* _topRightGrip;
		Panel* _bottomLeftGrip;
		Panel* _bottomRightGrip;
		Panel* _captionGrip;
		Panel* _client;
		Button* _trayButton;
		Button* _minimizeButton;
		Button* _maximizeButton;
		Button* _closeButton;
		Button* _menuButton;
		Dar<FrameSignal*> _frameSignalDar;
		Frame* _resizeable;
	};

	class VGUIAPI LineBorder : public Border
	{
	private:
		Color _color;
	public:
		LineBorder() {}
		LineBorder(int thickness) {}
		LineBorder(Color color) {}
		LineBorder(int thickness, Color color) {}

		inline void setLineColor(int r, int g, int b, int a) { _color = Color(r, g, b, a); }
	private:
		virtual void init(int thickness, Color color) {}
	protected:
		virtual void paint(Panel* panel) {}
	};

	class VGUIAPI MenuSeparator : public Label
	{
	public:
		MenuSeparator(const char* text) {}
	protected:
		virtual void paintBackground() {}
	};

	class VGUIAPI TextPanel : public Panel
	{
	private:
		TextImage* _textImage;
	public:
		TextPanel(const char* text, int x, int y, int wide, int tall) {}
	public:
		virtual void setText(const char* text) {}
		virtual void setFont(vgui::Scheme::SchemeFont schemeFont) {}
		virtual void setFont(vgui::Font* font) {}
		virtual void setSize(int wide, int tall) {}
		virtual void setFgColor(int r, int g, int b, int a) {}
		virtual void setFgColor(Scheme::SchemeColor sc) {}
		virtual TextImage* getTextImage() { return NULL; }
	protected:
		virtual void paint() {}
	};

	class VGUIAPI App
	{
	public:
		App()
		{
			_instance = this;
		}

		App(bool externalMain)
		{
			_instance = this;
		}
	public:
		static App* getInstance() { return _instance; }
		//TODO: the public and public bullshit are all messed up, need to organize
		//TODO: actually all of the access needs to be properly thought out while you are at it
	public:
		virtual void    start() {}
		virtual void    stop() {}
		virtual void    externalTick() {}
		virtual bool    wasMousePressed(MouseCode code, Panel* panel) { return false; }
		virtual bool    wasMouseDoublePressed(MouseCode code, Panel* panel) { return false; }
		virtual bool    isMouseDown(MouseCode code, Panel* panel) { return false; }
		virtual bool    wasMouseReleased(MouseCode code, Panel* panel) { return false; }
		virtual bool    wasKeyPressed(KeyCode code, Panel* panel) { return false; }
		virtual bool    isKeyDown(KeyCode code, Panel* panel) { return false; }
		virtual bool    wasKeyTyped(KeyCode code, Panel* panel) { return false; }
		virtual bool    wasKeyReleased(KeyCode code, Panel* panel) { return false; }
		virtual void    addTickSignal(TickSignal* s) {}
		virtual void    setCursorPos(int x, int y) {}
		virtual void    getCursorPos(int& x, int& y) {}
		virtual void    setMouseCapture(Panel* panel) {}
		virtual void    setMouseArena(int x0, int y0, int x1, int y1, bool enabled) {}
		virtual void    setMouseArena(Panel* panel) {}
		virtual void    requestFocus(Panel* panel) {}
		virtual Panel* getFocus() { return NULL; }
		virtual void    repaintAll() {}
		virtual void    setScheme(Scheme* scheme) {}
		virtual Scheme* getScheme() { return NULL; }
		virtual void    enableBuildMode() {}
		virtual long    getTimeMillis() { return 0; }
		virtual char    getKeyCodeChar(KeyCode code, bool shifted) { return '\0'; }
		virtual void    getKeyCodeText(KeyCode code, char* buf, int buflen) {}
		virtual int     getClipboardTextCount() { return 0; }
		virtual void    setClipboardText(const char* text, int textLen) {}
		virtual int     getClipboardText(int offset, char* buf, int bufLen) { return 0; }
		virtual void    reset() {}
		virtual void    internalSetMouseArena(int x0, int y0, int x1, int y1, bool enabled) {}
		virtual bool    setRegistryString(const char* key, const char* value) { return false; }
		virtual bool    getRegistryString(const char* key, char* value, int valueLen) { return false; }
		virtual bool    setRegistryInteger(const char* key, int value) { return false; }
		virtual bool    getRegistryInteger(const char* key, int& value) { return false; }
		virtual void    setCursorOveride(Cursor* cursor) {}
		virtual Cursor* getCursorOveride() { return NULL; }
		virtual void    setMinimumTickMillisInterval(int interval) {}
	public: //bullshit public stuff
		virtual void main(int argc, char* argv[]) = 0;
		virtual void run() {}
		virtual void internalCursorMoved(int x, int y, SurfaceBase* surfaceBase) {} //expects input in surface space
		virtual void internalMousePressed(MouseCode code, SurfaceBase* surfaceBase) {}
		virtual void internalMouseDoublePressed(MouseCode code, SurfaceBase* surfaceBase) {}
		virtual void internalMouseReleased(MouseCode code, SurfaceBase* surfaceBase) {}
		virtual void internalMouseWheeled(int delta, SurfaceBase* surfaceBase) {}
		virtual void internalKeyPressed(KeyCode code, SurfaceBase* surfaceBase) {}
		virtual void internalKeyTyped(KeyCode code, SurfaceBase* surfaceBase) {}
		virtual void internalKeyReleased(KeyCode code, SurfaceBase* surfaceBase) {}
	private:
		virtual void init() {}
		virtual void updateMouseFocus(int x, int y, SurfaceBase* surfaceBase) {}
		virtual void setMouseFocus(Panel* newMouseFocus) {}
	protected:
		virtual void surfaceBaseCreated(SurfaceBase* surfaceBase) {}
		virtual void surfaceBaseDeleted(SurfaceBase* surfaceBase) {}
		virtual void platTick() {}
		virtual void internalTick() {}
	protected:
		static App* _instance;
	protected:
		bool              _running;
		bool              _externalMain;
		Dar<SurfaceBase*> _surfaceBaseDar;
		Panel* _keyFocus;
		Panel* _oldMouseFocus;
		Panel* _mouseFocus;
		Panel* _mouseCapture;
		Panel* _wantedKeyFocus;
		bool              _mousePressed[MOUSE_LAST];
		bool              _mouseDoublePressed[MOUSE_LAST];
		bool              _mouseDown[MOUSE_LAST];
		bool              _mouseReleased[MOUSE_LAST];
		bool              _keyPressed[KEY_LAST];
		bool              _keyTyped[KEY_LAST];
		bool              _keyDown[KEY_LAST];
		bool              _keyReleased[KEY_LAST];
		Dar<TickSignal*>  _tickSignalDar;
		Scheme* _scheme;
		bool              _buildMode;
		bool              _wantedBuildMode;
		Panel* _mouseArenaPanel;
		Cursor* _cursor[Cursor::dc_last];
		Cursor* _cursorOveride;
	private:
		long              _nextTickMillis;
		long              _minimumTickMillisInterval;
	};
	App* App::_instance = NULL;

	class VGUIAPI MiniApp : public App
	{
	public:
		MiniApp() {}
	public:
		virtual void   getName(char* buf, int bufLen) {}
		virtual Frame* createInstance() = 0;
	protected:
		virtual void setName(const char* name) {}
	protected:
		char* _name;
	};

	class VGUIAPI DesktopIcon : public Panel
	{
	public:
		DesktopIcon(MiniApp* miniApp, Image* image) {}
	public:
		virtual void doActivate() {}
		virtual void setImage(Image* image) {}
	public: //bullshit public
		virtual void     setDesktop(Desktop* desktop) {}
		virtual MiniApp* getMiniApp() { return NULL; }
	protected:
		virtual void paintBackground() {}
	protected:
		Desktop* _desktop;
		MiniApp* _miniApp;
		Image* _image;
	};

	class VGUIAPI Slider : public Panel
	{
	private:
		bool                  _vertical;
		bool                  _dragging;
		int                   _nobPos[2];
		int                   _nobDragStartPos[2];
		int                   _dragStartPos[2];
		Dar<IntChangeSignal*> _intChangeSignalDar;
		int                   _range[2];
		int                   _value;
		int                   _rangeWindow;
		bool                  _rangeWindowEnabled;
		int                   _buttonOffset;
	public:
		Slider(int x, int y, int wide, int tall, bool vertical) {}
	public:
		virtual void setValue(int value) {}
		virtual int  getValue() { return 0; }
		virtual bool isVertical() { return false; }
		virtual void addIntChangeSignal(IntChangeSignal* s) {}
		virtual void setRange(int min, int max) {}
		virtual void getRange(int& min, int& max) {}
		virtual void setRangeWindow(int rangeWindow) {}
		virtual void setRangeWindowEnabled(bool state) {}
		virtual void setSize(int wide, int tall) {}
		virtual void getNobPos(int& min, int& max) {}
		virtual bool hasFullRange() { return false; }
		virtual void setButtonOffset(int buttonOffset) {}
	private:
		virtual void recomputeNobPosFromValue() {}
		virtual void recomputeValueFromNobPos() {}
	public: //bullshit public
		virtual void privateCursorMoved(int x, int y, Panel* panel) {}
		virtual void privateMousePressed(MouseCode code, Panel* panel) {}
		virtual void privateMouseReleased(MouseCode code, Panel* panel) {}
	protected:
		virtual void fireIntChangeSignal() {}
		virtual void paintBackground() {}
	};

	class VGUIAPI ScrollBar : public Panel
	{
	public:
		ScrollBar(int x, int y, int wide, int tall, bool vertical) {}
	public:
		virtual void    setValue(int value) {}
		virtual int     getValue() { return 0; }
		virtual void    addIntChangeSignal(IntChangeSignal* s) {}
		virtual void    setRange(int min, int max) {}
		virtual void    setRangeWindow(int rangeWindow) {}
		virtual void    setRangeWindowEnabled(bool state) {}
		virtual void    setSize(int wide, int tall) {}
		virtual bool    isVertical() { return false; }
		virtual bool    hasFullRange() { return false; }
		virtual void    setButton(Button* button, int index) {}
		virtual Button* getButton(int index) { return NULL; }
		virtual void    setSlider(Slider* slider) {}
		virtual Slider* getSlider() { return NULL; }
		virtual void 	doButtonPressed(int buttonIndex) {}
		virtual void    setButtonPressedScrollValue(int value) {}
		virtual void    validate() {}
	public: //bullshit public 
		virtual void fireIntChangeSignal() {}
	protected:
		virtual void performLayout() {}
	protected:
		Button* _button[2];
		Slider* _slider;
		Dar<IntChangeSignal*> _intChangeSignalDar;
		int     _buttonPressedScrollValue;
	};
	static ScrollBar s_StubScrollBar(0, 0, 0, 0, false);

	class VGUIAPI ScrollPanel : public Panel
	{
	private:
		Panel* _clientClip;
		Panel* _client;
		ScrollBar* _horizontalScrollBar;
		ScrollBar* _verticalScrollBar;
		bool       _autoVisible[2];
	public:
		ScrollPanel( int x, int y, int wide, int tall ) : Panel( x, y, wide, tall )
		{
		}
	protected:
		virtual void setSize(int wide, int tall) {}
	public:
		virtual void   setScrollBarVisible(bool horizontal, bool vertical) {}
		virtual void   setScrollBarAutoVisible(bool horizontal, bool vertical) {}
		virtual Panel* getClient() { return &s_StubPanel; }
		virtual Panel* getClientClip() { return NULL; }
		virtual void   setScrollValue(int horizontal, int vertical) {}
		virtual void   getScrollValue(int& horizontal, int& vertical) {}
		virtual void   recomputeClientSize() {}

		virtual ScrollBar* getHorizontalScrollBar()
		{
			return &s_StubScrollBar;
		}

		virtual ScrollBar* getVerticalScrollBar()
		{
			return &s_StubScrollBar;
		}

		virtual void       validate() {}
	public: //bullshit public
		virtual void recomputeScroll() {}
	};

	class VGUIAPI HeaderPanel : public Panel
	{
	private:
		Dar<Panel*>        _sliderPanelDar;
		Dar<Panel*>        _sectionPanelDar;
		Dar<ChangeSignal*> _changeSignalDar;
		Panel* _sectionLayer;
		int                _sliderWide;
		bool               _dragging;
		int                _dragSliderIndex;
		int                _dragSliderStartPos;
		int                _dragSliderStartX;
	public:
		HeaderPanel(int x, int y, int wide, int tall) {}
	protected:
		virtual void performLayout() {}
	public:
		virtual void addSectionPanel(Panel* panel) {}
		virtual void setSliderPos(int sliderIndex, int pos) {}
		virtual int  getSectionCount() { return 0; }
		virtual void getSectionExtents(int sectionIndex, int& x0, int& x1) {}
		virtual void addChangeSignal(ChangeSignal* s) {}
	public: //bullshit public
		virtual void fireChangeSignal() {}
		virtual void privateCursorMoved(int x, int y, Panel* panel) {}
		virtual void privateMousePressed(MouseCode code, Panel* panel) {}
		virtual void privateMouseReleased(MouseCode code, Panel* panel) {}
	};

	class VGUIAPI TablePanel : public Panel
	{
	private:
		vgui::Dar<int> _columnDar;
		bool           _gridVisible[2];
		int            _gridWide;
		int            _gridTall;
		int            _selectedCell[2];
		int            _mouseOverCell[2];
		int            _editableCell[2];
		Panel* _fakeInputPanel;
		bool           _columnSelectionEnabled;
		bool           _rowSelectionEnabled;
		bool           _cellSelectionEnabled;
		Panel* _editableCellPanel;
		int            _virtualSize[2];
		bool           _cellEditingEnabled;
	public:
		TablePanel(int x, int y, int wide, int tall, int columnCount) {}
	public:
		virtual void   setCellEditingEnabled(bool state) {}
		virtual void   setColumnCount(int columnCount) {}
		virtual void   setGridVisible(bool horizontal, bool vertical) {}
		virtual void   setGridSize(int gridWide, int gridTall) {}
		virtual int    getColumnCount() { return 0; }
		virtual void   setColumnExtents(int column, int x0, int x1) {}
		virtual void   setSelectedCell(int column, int row) {}
		virtual void   getSelectedCell(int& column, int& row) {}
		virtual void   setHeaderPanel(HeaderPanel* header) {}
		virtual void   setColumnSelectionEnabled(bool state) {}
		virtual void   setRowSelectionEnabled(bool state) {}
		virtual void   setCellSectionEnabled(bool state) {}
		virtual void   setEditableCell(int column, int row) {}
		virtual void   stopCellEditing() {}
		virtual void   getVirtualSize(int& wide, int& tall) {}
		virtual int    getRowCount() = 0;
		virtual int    getCellTall(int row) = 0;
		virtual Panel* getCellRenderer(int column, int row, bool columnSelected, bool rowSelected, bool cellSelected) = 0;
		virtual Panel* startCellEditing(int column, int row) = 0;
	protected:
		virtual void   paint() {}
		virtual Panel* isWithinTraverse(int x, int y) { return NULL; }
	private:
		virtual void privateMousePressed(MouseCode code, Panel* panel) {}
		virtual void privateMouseDoublePressed(MouseCode code, Panel* panel) {}
		virtual void privateKeyTyped(KeyCode code, Panel* panel) {}
	};

	class VGUIAPI TextGrid : public Panel
	{
	public:
		TextGrid(int gridWide, int gridTall, int x, int y, int wide, int tall) {}
	public:
		virtual void setGridSize(int wide, int tall) {}
		virtual void newLine() {}
		virtual void setXY(int x, int y) {}
		virtual int  vprintf(const char* format, va_list argList) { return 0; }
		virtual int  printf(const char* format, ...) { return 0; }
	protected:
		virtual void paintBackground() {}
	protected:
		int   _xy[2];
		int   _bgColor[3];
		int   _fgColor[3];
		char* _grid;
		int	  _gridSize[2];
	};

	class VGUIAPI TextEntry : public Panel, public InputSignal
	{
	public:
		TextEntry(const char* text, int x, int y, int wide, int tall) {}
	public:
		virtual void setText(const char* text, int textLen) {}
		virtual void getText(int offset, char* buf, int bufLen) {}
		virtual void resetCursorBlink() {}
		virtual void doGotoLeft() {}
		virtual void doGotoRight() {}
		virtual void doGotoFirstOfLine() {}
		virtual void doGotoEndOfLine() {}
		virtual void doInsertChar(char ch) {}
		virtual void doBackspace() {}
		virtual void doDelete() {}
		virtual void doSelectNone() {}
		virtual void doCopySelected() {}
		virtual void doPaste() {}
		virtual void doPasteSelected() {}
		virtual void doDeleteSelected() {}
		virtual void addActionSignal(ActionSignal* s) {}
		virtual void setFont(Font* font) {}
		virtual void setTextHidden(bool bHideText) {}
	protected:
		virtual void paintBackground() {}
		virtual void setCharAt(char ch, int index) {}
	protected:
		virtual void fireActionSignal() {}
		virtual bool getSelectedRange(int& cx0, int& cx1) { return false; }
		virtual bool getSelectedPixelRange(int& cx0, int& cx1) { return false; }
		virtual int  cursorToPixelSpace(int cursorPos) { return 0; }
		virtual void selectCheck() {}
	protected: //InputSignal
		virtual void cursorMoved(int x, int y, Panel* panel) {}
		virtual void cursorEntered(Panel* panel) {}
		virtual void cursorExited(Panel* panel) {}
		virtual void mousePressed(MouseCode code, Panel* panel) {}
		virtual void mouseDoublePressed(MouseCode code, Panel* panel) {}
		virtual void mouseReleased(MouseCode code, Panel* panel) {}
		virtual void mouseWheeled(int delta, Panel* panel) {}
		virtual void keyPressed(KeyCode code, Panel* panel) {}
		virtual void keyTyped(KeyCode code, Panel* panel) {}
		virtual void keyReleased(KeyCode code, Panel* panel) {}
		virtual void keyFocusTicked(Panel* panel) {}
	protected:
		Dar<char>          _lineDar;
		int                _cursorPos;
		bool               _cursorBlink;
		bool               _hideText;
		long               _cursorNextBlinkTime;
		int                _cursorBlinkRate;
		int                _select[2];
		Dar<ActionSignal*> _actionSignalDar;
		Font* _font;
	};

	class VGUIAPI Surface : public SurfaceBase
	{
	public:
		Surface(Panel* embeddedPanel) : SurfaceBase(embeddedPanel) {}
	public:
		virtual void setTitle(const char* title) {}
		virtual bool setFullscreenMode(int wide, int tall, int bpp) { return false; }
		virtual void setWindowedMode() {}
		virtual void setAsTopMost(bool state) {}
		virtual int  getModeInfoCount() { return 0; }
		virtual void createPopup(Panel* embeddedPanel) {}
		virtual bool hasFocus() { return false; }
		virtual bool isWithin(int x, int y) { return false; }
		virtual void GetMousePos(int& x, int& y) {}
	protected:
		virtual int  createNewTextureID(void) { return 0; }
		virtual void drawSetColor(int r, int g, int b, int a) {}
		virtual void drawFilledRect(int x0, int y0, int x1, int y1) {}
		virtual void drawOutlinedRect(int x0, int y0, int x1, int y1) {}
		virtual void drawSetTextFont(Font* font) {}
		virtual void drawSetTextColor(int r, int g, int b, int a) {}
		virtual void drawSetTextPos(int x, int y) {}
		virtual void drawPrintText(const char* text, int textLen) {}
		virtual void drawSetTextureRGBA(int id, const char* rgba, int wide, int tall) {}
		virtual void drawSetTexture(int id) {}
		virtual void drawTexturedRect(int x0, int y0, int x1, int y1) {}
		virtual void invalidate(Panel* panel) {}
		virtual bool createPlat() { return false; }
		virtual bool recreateContext() { return false; }
		virtual void enableMouseCapture(bool state) {}
		virtual void setCursor(Cursor* cursor) {}
		virtual void swapBuffers() {}
		virtual void pushMakeCurrent(Panel* panel, bool useInsets) {}
		virtual void popMakeCurrent(Panel* panel) {}
		virtual void applyChanges() {}
	protected:
		class SurfacePlat* _plat;
		bool               _needsSwap;
		Panel* _embeddedPanel;
		Dar<char*>         _modeInfoDar;
		friend class App;
		friend class Panel;
	};

	class VGUIAPI BuildGroup
	{
	private:
		bool      _enabled;
		int       _snapX;
		int       _snapY;
		Cursor* _cursor_sizenwse;
		Cursor* _cursor_sizenesw;
		Cursor* _cursor_sizewe;
		Cursor* _cursor_sizens;
		Cursor* _cursor_sizeall;
		bool      _dragging;
		MouseCode _dragMouseCode;
		int       _dragStartPanelPos[2];
		int       _dragStartCursorPos[2];
		Panel* _currentPanel;
		Dar<ChangeSignal*> _currentPanelChangeSignalDar;
		Dar<Panel*> _panelDar;
		Dar<char*>  _panelNameDar;
	public:
		BuildGroup() {}
	public:
		virtual void   setEnabled(bool state) {}
		virtual bool   isEnabled() { return false; }
		virtual void   addCurrentPanelChangeSignal(ChangeSignal* s) {}
		virtual Panel* getCurrentPanel() { return NULL; }
		virtual void   copyPropertiesToClipboard() {}
	private:
		virtual void applySnap(Panel* panel) {}
		virtual void fireCurrentPanelChangeSignal() {}
	protected:
		virtual void    panelAdded(Panel* panel, const char* panelName) {}
		virtual void    cursorMoved(int x, int y, Panel* panel) {}
		virtual void    mousePressed(MouseCode code, Panel* panel) {}
		virtual void    mouseReleased(MouseCode code, Panel* panel) {}
		virtual void    mouseDoublePressed(MouseCode code, Panel* panel) {}
		virtual void    keyTyped(KeyCode code, Panel* panel) {}
		virtual Cursor* getCursor(Panel* panel) { return NULL; }
	};
}

static vgui::Scheme s_StubScheme;

class StubApp : public vgui::App
{
public:
	StubApp() : vgui::App()
	{
		m_pScheme = &s_StubScheme;
	}

	virtual void main(int argc, char* argv[])
	{

	}

	virtual void setScheme(vgui::Scheme* scheme)
	{
	}

	virtual vgui::Scheme* getScheme()
	{
		return m_pScheme;
	}

private:
	vgui::Scheme *m_pScheme;
};

static StubApp s_App;
static vgui::Panel s_StubPanel;

class CVGUIStub : public IVGUIStub
{
public:
	CVGUIStub()
	{
	}

	virtual ~CVGUIStub()
	{
	}

	virtual void Init()
	{
	}

	virtual void Shutdown()
	{
	}

	virtual void* GetRootPanel()
	{
		return &s_StubPanel;
	}
};

static CVGUIStub s_VGUIStub;

extern "C" VGUISTUB_API IVGUIStub * GetVGUIStub(void)
{
	return &s_VGUIStub;
}
