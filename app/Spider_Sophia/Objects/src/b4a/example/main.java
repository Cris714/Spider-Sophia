package b4a.example;


import anywheresoftware.b4a.B4AMenuItem;
import android.app.Activity;
import android.os.Bundle;
import anywheresoftware.b4a.BA;
import anywheresoftware.b4a.BALayout;
import anywheresoftware.b4a.B4AActivity;
import anywheresoftware.b4a.ObjectWrapper;
import anywheresoftware.b4a.objects.ActivityWrapper;
import java.lang.reflect.InvocationTargetException;
import anywheresoftware.b4a.B4AUncaughtException;
import anywheresoftware.b4a.debug.*;
import java.lang.ref.WeakReference;

public class main extends Activity implements B4AActivity{
	public static main mostCurrent;
	static boolean afterFirstLayout;
	static boolean isFirst = true;
    private static boolean processGlobalsRun = false;
	BALayout layout;
	public static BA processBA;
	BA activityBA;
    ActivityWrapper _activity;
    java.util.ArrayList<B4AMenuItem> menuItems;
	public static final boolean fullScreen = false;
	public static final boolean includeTitle = false;
    public static WeakReference<Activity> previousOne;
    public static boolean dontPause;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
        mostCurrent = this;
		if (processBA == null) {
			processBA = new BA(this.getApplicationContext(), null, null, "b4a.example", "b4a.example.main");
			processBA.loadHtSubs(this.getClass());
	        float deviceScale = getApplicationContext().getResources().getDisplayMetrics().density;
	        BALayout.setDeviceScale(deviceScale);
            
		}
		else if (previousOne != null) {
			Activity p = previousOne.get();
			if (p != null && p != this) {
                BA.LogInfo("Killing previous instance (main).");
				p.finish();
			}
		}
        processBA.setActivityPaused(true);
        processBA.runHook("oncreate", this, null);
		if (!includeTitle) {
        	this.getWindow().requestFeature(android.view.Window.FEATURE_NO_TITLE);
        }
        if (fullScreen) {
        	getWindow().setFlags(android.view.WindowManager.LayoutParams.FLAG_FULLSCREEN,   
        			android.view.WindowManager.LayoutParams.FLAG_FULLSCREEN);
        }
		
        processBA.sharedProcessBA.activityBA = null;
		layout = new BALayout(this);
		setContentView(layout);
		afterFirstLayout = false;
        WaitForLayout wl = new WaitForLayout();
        if (anywheresoftware.b4a.objects.ServiceHelper.StarterHelper.startFromActivity(this, processBA, wl, false))
		    BA.handler.postDelayed(wl, 5);

	}
	static class WaitForLayout implements Runnable {
		public void run() {
			if (afterFirstLayout)
				return;
			if (mostCurrent == null)
				return;
            
			if (mostCurrent.layout.getWidth() == 0) {
				BA.handler.postDelayed(this, 5);
				return;
			}
			mostCurrent.layout.getLayoutParams().height = mostCurrent.layout.getHeight();
			mostCurrent.layout.getLayoutParams().width = mostCurrent.layout.getWidth();
			afterFirstLayout = true;
			mostCurrent.afterFirstLayout();
		}
	}
	private void afterFirstLayout() {
        if (this != mostCurrent)
			return;
		activityBA = new BA(this, layout, processBA, "b4a.example", "b4a.example.main");
        
        processBA.sharedProcessBA.activityBA = new java.lang.ref.WeakReference<BA>(activityBA);
        anywheresoftware.b4a.objects.ViewWrapper.lastId = 0;
        _activity = new ActivityWrapper(activityBA, "activity");
        anywheresoftware.b4a.Msgbox.isDismissing = false;
        if (BA.isShellModeRuntimeCheck(processBA)) {
			if (isFirst)
				processBA.raiseEvent2(null, true, "SHELL", false);
			processBA.raiseEvent2(null, true, "CREATE", true, "b4a.example.main", processBA, activityBA, _activity, anywheresoftware.b4a.keywords.Common.Density, mostCurrent);
			_activity.reinitializeForShell(activityBA, "activity");
		}
        initializeProcessGlobals();		
        initializeGlobals();
        
        BA.LogInfo("** Activity (main) Create " + (isFirst ? "(first time)" : "") + " **");
        processBA.raiseEvent2(null, true, "activity_create", false, isFirst);
		isFirst = false;
		if (this != mostCurrent)
			return;
        processBA.setActivityPaused(false);
        BA.LogInfo("** Activity (main) Resume **");
        processBA.raiseEvent(null, "activity_resume");
        if (android.os.Build.VERSION.SDK_INT >= 11) {
			try {
				android.app.Activity.class.getMethod("invalidateOptionsMenu").invoke(this,(Object[]) null);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}
	public void addMenuItem(B4AMenuItem item) {
		if (menuItems == null)
			menuItems = new java.util.ArrayList<B4AMenuItem>();
		menuItems.add(item);
	}
	@Override
	public boolean onCreateOptionsMenu(android.view.Menu menu) {
		super.onCreateOptionsMenu(menu);
        try {
            if (processBA.subExists("activity_actionbarhomeclick")) {
                Class.forName("android.app.ActionBar").getMethod("setHomeButtonEnabled", boolean.class).invoke(
                    getClass().getMethod("getActionBar").invoke(this), true);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (processBA.runHook("oncreateoptionsmenu", this, new Object[] {menu}))
            return true;
		if (menuItems == null)
			return false;
		for (B4AMenuItem bmi : menuItems) {
			android.view.MenuItem mi = menu.add(bmi.title);
			if (bmi.drawable != null)
				mi.setIcon(bmi.drawable);
            if (android.os.Build.VERSION.SDK_INT >= 11) {
				try {
                    if (bmi.addToBar) {
				        android.view.MenuItem.class.getMethod("setShowAsAction", int.class).invoke(mi, 1);
                    }
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			mi.setOnMenuItemClickListener(new B4AMenuItemsClickListener(bmi.eventName.toLowerCase(BA.cul)));
		}
        
		return true;
	}   
 @Override
 public boolean onOptionsItemSelected(android.view.MenuItem item) {
    if (item.getItemId() == 16908332) {
        processBA.raiseEvent(null, "activity_actionbarhomeclick");
        return true;
    }
    else
        return super.onOptionsItemSelected(item); 
}
@Override
 public boolean onPrepareOptionsMenu(android.view.Menu menu) {
    super.onPrepareOptionsMenu(menu);
    processBA.runHook("onprepareoptionsmenu", this, new Object[] {menu});
    return true;
    
 }
 protected void onStart() {
    super.onStart();
    processBA.runHook("onstart", this, null);
}
 protected void onStop() {
    super.onStop();
    processBA.runHook("onstop", this, null);
}
    public void onWindowFocusChanged(boolean hasFocus) {
       super.onWindowFocusChanged(hasFocus);
       if (processBA.subExists("activity_windowfocuschanged"))
           processBA.raiseEvent2(null, true, "activity_windowfocuschanged", false, hasFocus);
    }
	private class B4AMenuItemsClickListener implements android.view.MenuItem.OnMenuItemClickListener {
		private final String eventName;
		public B4AMenuItemsClickListener(String eventName) {
			this.eventName = eventName;
		}
		public boolean onMenuItemClick(android.view.MenuItem item) {
			processBA.raiseEventFromUI(item.getTitle(), eventName + "_click");
			return true;
		}
	}
    public static Class<?> getObject() {
		return main.class;
	}
    private Boolean onKeySubExist = null;
    private Boolean onKeyUpSubExist = null;
	@Override
	public boolean onKeyDown(int keyCode, android.view.KeyEvent event) {
        if (processBA.runHook("onkeydown", this, new Object[] {keyCode, event}))
            return true;
		if (onKeySubExist == null)
			onKeySubExist = processBA.subExists("activity_keypress");
		if (onKeySubExist) {
			if (keyCode == anywheresoftware.b4a.keywords.constants.KeyCodes.KEYCODE_BACK &&
					android.os.Build.VERSION.SDK_INT >= 18) {
				HandleKeyDelayed hk = new HandleKeyDelayed();
				hk.kc = keyCode;
				BA.handler.post(hk);
				return true;
			}
			else {
				boolean res = new HandleKeyDelayed().runDirectly(keyCode);
				if (res)
					return true;
			}
		}
		return super.onKeyDown(keyCode, event);
	}
	private class HandleKeyDelayed implements Runnable {
		int kc;
		public void run() {
			runDirectly(kc);
		}
		public boolean runDirectly(int keyCode) {
			Boolean res =  (Boolean)processBA.raiseEvent2(_activity, false, "activity_keypress", false, keyCode);
			if (res == null || res == true) {
                return true;
            }
            else if (keyCode == anywheresoftware.b4a.keywords.constants.KeyCodes.KEYCODE_BACK) {
				finish();
				return true;
			}
            return false;
		}
		
	}
    @Override
	public boolean onKeyUp(int keyCode, android.view.KeyEvent event) {
        if (processBA.runHook("onkeyup", this, new Object[] {keyCode, event}))
            return true;
		if (onKeyUpSubExist == null)
			onKeyUpSubExist = processBA.subExists("activity_keyup");
		if (onKeyUpSubExist) {
			Boolean res =  (Boolean)processBA.raiseEvent2(_activity, false, "activity_keyup", false, keyCode);
			if (res == null || res == true)
				return true;
		}
		return super.onKeyUp(keyCode, event);
	}
	@Override
	public void onNewIntent(android.content.Intent intent) {
        super.onNewIntent(intent);
		this.setIntent(intent);
        processBA.runHook("onnewintent", this, new Object[] {intent});
	}
    @Override 
	public void onPause() {
		super.onPause();
        if (_activity == null)
            return;
        if (this != mostCurrent)
			return;
		anywheresoftware.b4a.Msgbox.dismiss(true);
        if (!dontPause)
            BA.LogInfo("** Activity (main) Pause, UserClosed = " + activityBA.activity.isFinishing() + " **");
        else
            BA.LogInfo("** Activity (main) Pause event (activity is not paused). **");
        if (mostCurrent != null)
            processBA.raiseEvent2(_activity, true, "activity_pause", false, activityBA.activity.isFinishing());		
        if (!dontPause) {
            processBA.setActivityPaused(true);
            mostCurrent = null;
        }

        if (!activityBA.activity.isFinishing())
			previousOne = new WeakReference<Activity>(this);
        anywheresoftware.b4a.Msgbox.isDismissing = false;
        processBA.runHook("onpause", this, null);
	}

	@Override
	public void onDestroy() {
        super.onDestroy();
		previousOne = null;
        processBA.runHook("ondestroy", this, null);
	}
    @Override 
	public void onResume() {
		super.onResume();
        mostCurrent = this;
        anywheresoftware.b4a.Msgbox.isDismissing = false;
        if (activityBA != null) { //will be null during activity create (which waits for AfterLayout).
        	ResumeMessage rm = new ResumeMessage(mostCurrent);
        	BA.handler.post(rm);
        }
        processBA.runHook("onresume", this, null);
	}
    private static class ResumeMessage implements Runnable {
    	private final WeakReference<Activity> activity;
    	public ResumeMessage(Activity activity) {
    		this.activity = new WeakReference<Activity>(activity);
    	}
		public void run() {
            main mc = mostCurrent;
			if (mc == null || mc != activity.get())
				return;
			processBA.setActivityPaused(false);
            BA.LogInfo("** Activity (main) Resume **");
            if (mc != mostCurrent)
                return;
		    processBA.raiseEvent(mc._activity, "activity_resume", (Object[])null);
		}
    }
	@Override
	protected void onActivityResult(int requestCode, int resultCode,
	      android.content.Intent data) {
		processBA.onActivityResult(requestCode, resultCode, data);
        processBA.runHook("onactivityresult", this, new Object[] {requestCode, resultCode});
	}
	private static void initializeGlobals() {
		processBA.raiseEvent2(null, true, "globals", false, (Object[])null);
	}
    public void onRequestPermissionsResult(int requestCode,
        String permissions[], int[] grantResults) {
        for (int i = 0;i < permissions.length;i++) {
            Object[] o = new Object[] {permissions[i], grantResults[i] == 0};
            processBA.raiseEventFromDifferentThread(null,null, 0, "activity_permissionresult", true, o);
        }
            
    }

public anywheresoftware.b4a.keywords.Common __c = null;
public static anywheresoftware.b4a.objects.B4XViewWrapper.XUI _xui = null;
public static anywheresoftware.b4a.objects.SocketWrapper.UDPSocket _udps = null;
public static anywheresoftware.b4a.objects.SocketWrapper.UDPSocket.UDPPacket _udpp = null;
public static anywheresoftware.b4a.objects.Timer _delaytimer = null;
public static anywheresoftware.b4a.objects.Timer _walktimer = null;
public static anywheresoftware.b4a.objects.Timer _responsetimer = null;
public anywheresoftware.b4a.objects.EditTextWrapper _ip = null;
public anywheresoftware.b4a.objects.EditTextWrapper _port = null;
public static String _serverip = "";
public static String _serverport = "";
public static boolean _click_enabled = false;
public static boolean _responsereceived = false;
public static boolean _connected = false;
public anywheresoftware.b4a.objects.TabHostWrapper _tabhost1 = null;
public joystickwrapper.joystickWrapper _js1 = null;
public joystickwrapper.joystickWrapper _walkjs = null;
public joystickwrapper.joystickWrapper _turnjs = null;
public b4a.example.jscontrol _js = null;
public b4a.example.jscontrol _walkcontrol = null;
public static String _pckt = "";
public anywheresoftware.b4a.objects.ButtonWrapper _connect = null;
public anywheresoftware.b4a.objects.ButtonWrapper _disconnect = null;
public anywheresoftware.b4a.objects.ButtonWrapper _active_button = null;
public anywheresoftware.b4a.objects.ButtonWrapper _mov_xz = null;
public anywheresoftware.b4a.objects.ButtonWrapper _turn_phi = null;
public anywheresoftware.b4a.objects.ButtonWrapper _mov_y = null;
public anywheresoftware.b4a.objects.ButtonWrapper _turn_thpsi = null;
public anywheresoftware.b4a.objects.ButtonWrapper _walk = null;
public anywheresoftware.b4a.objects.LabelWrapper _coords_info = null;
public anywheresoftware.b4a.objects.LabelWrapper _ip_info = null;
public anywheresoftware.b4a.objects.drawable.ColorDrawable _cd1 = null;
public anywheresoftware.b4a.objects.drawable.ColorDrawable _cd2 = null;
public anywheresoftware.b4a.objects.WebViewWrapper _videocam = null;
public uk.co.martinpearman.b4a.webviewextras.WebViewExtras _webextras = null;
public anywheresoftware.b4a.objects.SpinnerWrapper _spinner1 = null;
public b4a.example.starter _starter = null;

public static boolean isAnyActivityVisible() {
    boolean vis = false;
vis = vis | (main.mostCurrent != null);
return vis;}
public static String  _activity_create(boolean _firsttime) throws Exception{
 //BA.debugLineNum = 46;BA.debugLine="Sub Activity_Create(FirstTime As Boolean)";
 //BA.debugLineNum = 48;BA.debugLine="Activity.LoadLayout(\"Tabhost\")";
mostCurrent._activity.LoadLayout("Tabhost",mostCurrent.activityBA);
 //BA.debugLineNum = 49;BA.debugLine="TabHost1.AddTab(\"Conn\", \"Connect\")";
mostCurrent._tabhost1.AddTab(mostCurrent.activityBA,"Conn","Connect");
 //BA.debugLineNum = 50;BA.debugLine="TabHost1.AddTab(\"1 JS Control\", \"Tasks\")";
mostCurrent._tabhost1.AddTab(mostCurrent.activityBA,"1 JS Control","Tasks");
 //BA.debugLineNum = 51;BA.debugLine="TabHost1.AddTab(\"Video Control\", \"JoystickCam\")";
mostCurrent._tabhost1.AddTab(mostCurrent.activityBA,"Video Control","JoystickCam");
 //BA.debugLineNum = 54;BA.debugLine="WebExtras.addJavascriptInterface(VideoCam, \"javas";
mostCurrent._webextras.addJavascriptInterface(mostCurrent.activityBA,(android.webkit.WebView)(mostCurrent._videocam.getObject()),"javascript");
 //BA.debugLineNum = 55;BA.debugLine="VideoCam.LoadUrl(\"http://192.168.3.107\")";
mostCurrent._videocam.LoadUrl("http://192.168.3.107");
 //BA.debugLineNum = 59;BA.debugLine="ip.Text=\"192.168.4.1\"";
mostCurrent._ip.setText(BA.ObjectToCharSequence("192.168.4.1"));
 //BA.debugLineNum = 60;BA.debugLine="port.Text=\"3000\"";
mostCurrent._port.setText(BA.ObjectToCharSequence("3000"));
 //BA.debugLineNum = 61;BA.debugLine="serverIp = \"\"";
mostCurrent._serverip = "";
 //BA.debugLineNum = 62;BA.debugLine="serverPort = \"\"";
mostCurrent._serverport = "";
 //BA.debugLineNum = 64;BA.debugLine="Disconnect.Visible = False";
mostCurrent._disconnect.setVisible(anywheresoftware.b4a.keywords.Common.False);
 //BA.debugLineNum = 67;BA.debugLine="js1.ButtonDrawable = \"button\"";
mostCurrent._js1.setButtonDrawable("button");
 //BA.debugLineNum = 68;BA.debugLine="js1.PadBackground = \"pad\"";
mostCurrent._js1.setPadBackground("pad");
 //BA.debugLineNum = 69;BA.debugLine="walkJS.ButtonDrawable = \"button\"";
mostCurrent._walkjs.setButtonDrawable("button");
 //BA.debugLineNum = 70;BA.debugLine="walkJS.PadBackground = \"pad\"";
mostCurrent._walkjs.setPadBackground("pad");
 //BA.debugLineNum = 71;BA.debugLine="turnJS.ButtonDrawable = \"button\"";
mostCurrent._turnjs.setButtonDrawable("button");
 //BA.debugLineNum = 72;BA.debugLine="turnJS.PadBackground = \"pad\"";
mostCurrent._turnjs.setPadBackground("pad");
 //BA.debugLineNum = 74;BA.debugLine="js.Initialize(0)";
mostCurrent._js._initialize /*String*/ (processBA,(int) (0));
 //BA.debugLineNum = 75;BA.debugLine="walkControl.Initialize(4)";
mostCurrent._walkcontrol._initialize /*String*/ (processBA,(int) (4));
 //BA.debugLineNum = 78;BA.debugLine="cd1.Initialize(Colors.RGB(231, 102, 136), 10dip)";
mostCurrent._cd1.Initialize(anywheresoftware.b4a.keywords.Common.Colors.RGB((int) (231),(int) (102),(int) (136)),anywheresoftware.b4a.keywords.Common.DipToCurrent((int) (10)));
 //BA.debugLineNum = 79;BA.debugLine="cd2.Initialize(Colors.RGB(120, 184, 169), 10dip)";
mostCurrent._cd2.Initialize(anywheresoftware.b4a.keywords.Common.Colors.RGB((int) (120),(int) (184),(int) (169)),anywheresoftware.b4a.keywords.Common.DipToCurrent((int) (10)));
 //BA.debugLineNum = 80;BA.debugLine="Mov_xz.Background = cd1";
mostCurrent._mov_xz.setBackground((android.graphics.drawable.Drawable)(mostCurrent._cd1.getObject()));
 //BA.debugLineNum = 81;BA.debugLine="click_enabled = True";
_click_enabled = anywheresoftware.b4a.keywords.Common.True;
 //BA.debugLineNum = 82;BA.debugLine="Active_Button = Mov_xz";
mostCurrent._active_button = mostCurrent._mov_xz;
 //BA.debugLineNum = 84;BA.debugLine="IP_info.Text = \"Disconnected\"";
mostCurrent._ip_info.setText(BA.ObjectToCharSequence("Disconnected"));
 //BA.debugLineNum = 85;BA.debugLine="Coords_info.Text = \"Ang = 0\" & CRLF & _ 					   \"";
mostCurrent._coords_info.setText(BA.ObjectToCharSequence("Ang = 0"+anywheresoftware.b4a.keywords.Common.CRLF+"Pwr = 0"+anywheresoftware.b4a.keywords.Common.CRLF+"x   = 0"+anywheresoftware.b4a.keywords.Common.CRLF+"y   = 0"+anywheresoftware.b4a.keywords.Common.CRLF+"z   = 0"+anywheresoftware.b4a.keywords.Common.CRLF+"th  = 0"+anywheresoftware.b4a.keywords.Common.CRLF+"phi = 0"+anywheresoftware.b4a.keywords.Common.CRLF+"psi = 0"));
 //BA.debugLineNum = 95;BA.debugLine="Spinner1.AddAll(Array As String(\"Turn\", \"Mov(x/z)";
mostCurrent._spinner1.AddAll(anywheresoftware.b4a.keywords.Common.ArrayToList(new String[]{"Turn","Mov(x/z)","Turn(phi)","Up/Down","Dance"}));
 //BA.debugLineNum = 97;BA.debugLine="If FirstTime Then";
if (_firsttime) { 
 //BA.debugLineNum = 98;BA.debugLine="udps.Initialize(\"UDP\", 3000, 8192)";
_udps.Initialize(processBA,"UDP",(int) (3000),(int) (8192));
 //BA.debugLineNum = 99;BA.debugLine="delayTimer.Initialize(\"delayTimer\", 12)";
_delaytimer.Initialize(processBA,"delayTimer",(long) (12));
 //BA.debugLineNum = 100;BA.debugLine="walkTimer.Initialize(\"walkTimer\", 25)";
_walktimer.Initialize(processBA,"walkTimer",(long) (25));
 //BA.debugLineNum = 101;BA.debugLine="walkTimer.Enabled = True";
_walktimer.setEnabled(anywheresoftware.b4a.keywords.Common.True);
 //BA.debugLineNum = 102;BA.debugLine="responseTimer.Initialize(\"responseTimer\", 5000)";
_responsetimer.Initialize(processBA,"responseTimer",(long) (5000));
 };
 //BA.debugLineNum = 105;BA.debugLine="End Sub";
return "";
}
public static String  _activity_pause(boolean _userclosed) throws Exception{
 //BA.debugLineNum = 107;BA.debugLine="Sub Activity_Pause (UserClosed As Boolean)";
 //BA.debugLineNum = 108;BA.debugLine="If connected Then";
if (_connected) { 
 //BA.debugLineNum = 109;BA.debugLine="Log(\"sent disconnect\")";
anywheresoftware.b4a.keywords.Common.LogImpl("2196610","sent disconnect",0);
 //BA.debugLineNum = 110;BA.debugLine="DisconnectApp";
_disconnectapp();
 };
 //BA.debugLineNum = 112;BA.debugLine="End Sub";
return "";
}
public static String  _connect_click() throws Exception{
 //BA.debugLineNum = 215;BA.debugLine="Private Sub Connect_Click";
 //BA.debugLineNum = 216;BA.debugLine="responseReceived = False";
_responsereceived = anywheresoftware.b4a.keywords.Common.False;
 //BA.debugLineNum = 217;BA.debugLine="udpp.Initialize(\"C\".GetBytes(\"ASCII\"), ip.Text, p";
_udpp.Initialize("C".getBytes("ASCII"),mostCurrent._ip.getText(),(int)(Double.parseDouble(mostCurrent._port.getText())));
 //BA.debugLineNum = 218;BA.debugLine="udps.Send(udpp)";
_udps.Send(_udpp);
 //BA.debugLineNum = 219;BA.debugLine="connect.Enabled = False";
mostCurrent._connect.setEnabled(anywheresoftware.b4a.keywords.Common.False);
 //BA.debugLineNum = 220;BA.debugLine="connect.Text = \"Connecting...\"";
mostCurrent._connect.setText(BA.ObjectToCharSequence("Connecting..."));
 //BA.debugLineNum = 221;BA.debugLine="responseTimer.Enabled = True";
_responsetimer.setEnabled(anywheresoftware.b4a.keywords.Common.True);
 //BA.debugLineNum = 224;BA.debugLine="End Sub";
return "";
}
public static String  _delaytimer_tick() throws Exception{
 //BA.debugLineNum = 123;BA.debugLine="Sub delayTimer_Tick";
 //BA.debugLineNum = 124;BA.debugLine="delayTimer.Enabled = False";
_delaytimer.setEnabled(anywheresoftware.b4a.keywords.Common.False);
 //BA.debugLineNum = 125;BA.debugLine="click_enabled = True";
_click_enabled = anywheresoftware.b4a.keywords.Common.True;
 //BA.debugLineNum = 126;BA.debugLine="End Sub";
return "";
}
public static String  _disconnect_click() throws Exception{
 //BA.debugLineNum = 226;BA.debugLine="Private Sub Disconnect_Click";
 //BA.debugLineNum = 227;BA.debugLine="DisconnectApp";
_disconnectapp();
 //BA.debugLineNum = 228;BA.debugLine="End Sub";
return "";
}
public static String  _disconnectapp() throws Exception{
 //BA.debugLineNum = 230;BA.debugLine="Sub DisconnectApp";
 //BA.debugLineNum = 231;BA.debugLine="udpp.Initialize(\"D\".GetBytes(\"ASCII\"), ip.Text, p";
_udpp.Initialize("D".getBytes("ASCII"),mostCurrent._ip.getText(),(int)(Double.parseDouble(mostCurrent._port.getText())));
 //BA.debugLineNum = 232;BA.debugLine="udps.Send(udpp)";
_udps.Send(_udpp);
 //BA.debugLineNum = 233;BA.debugLine="ToastMessageShow(\"Disconnected\", True)";
anywheresoftware.b4a.keywords.Common.ToastMessageShow(BA.ObjectToCharSequence("Disconnected"),anywheresoftware.b4a.keywords.Common.True);
 //BA.debugLineNum = 234;BA.debugLine="Disconnect.Visible = False";
mostCurrent._disconnect.setVisible(anywheresoftware.b4a.keywords.Common.False);
 //BA.debugLineNum = 235;BA.debugLine="IP_info.Text = \"Disconnected\"";
mostCurrent._ip_info.setText(BA.ObjectToCharSequence("Disconnected"));
 //BA.debugLineNum = 236;BA.debugLine="connect.Visible = True";
mostCurrent._connect.setVisible(anywheresoftware.b4a.keywords.Common.True);
 //BA.debugLineNum = 237;BA.debugLine="connect.Enabled = True";
mostCurrent._connect.setEnabled(anywheresoftware.b4a.keywords.Common.True);
 //BA.debugLineNum = 238;BA.debugLine="connect.Text = \"Connect\"";
mostCurrent._connect.setText(BA.ObjectToCharSequence("Connect"));
 //BA.debugLineNum = 239;BA.debugLine="serverIp = \"\"";
mostCurrent._serverip = "";
 //BA.debugLineNum = 240;BA.debugLine="serverPort = \"\"";
mostCurrent._serverport = "";
 //BA.debugLineNum = 241;BA.debugLine="connected = False";
_connected = anywheresoftware.b4a.keywords.Common.False;
 //BA.debugLineNum = 242;BA.debugLine="End Sub";
return "";
}
public static String  _globals() throws Exception{
 //BA.debugLineNum = 25;BA.debugLine="Sub Globals";
 //BA.debugLineNum = 27;BA.debugLine="Dim ip, port As EditText";
mostCurrent._ip = new anywheresoftware.b4a.objects.EditTextWrapper();
mostCurrent._port = new anywheresoftware.b4a.objects.EditTextWrapper();
 //BA.debugLineNum = 28;BA.debugLine="Dim serverIp, serverPort As String";
mostCurrent._serverip = "";
mostCurrent._serverport = "";
 //BA.debugLineNum = 30;BA.debugLine="Dim click_enabled, responseReceived, connected As";
_click_enabled = false;
_responsereceived = false;
_connected = false;
 //BA.debugLineNum = 31;BA.debugLine="Dim TabHost1 As TabHost";
mostCurrent._tabhost1 = new anywheresoftware.b4a.objects.TabHostWrapper();
 //BA.debugLineNum = 33;BA.debugLine="Dim js1, walkJS, turnJS As JoyStick";
mostCurrent._js1 = new joystickwrapper.joystickWrapper();
mostCurrent._walkjs = new joystickwrapper.joystickWrapper();
mostCurrent._turnjs = new joystickwrapper.joystickWrapper();
 //BA.debugLineNum = 34;BA.debugLine="Dim js, walkControl As JSControl";
mostCurrent._js = new b4a.example.jscontrol();
mostCurrent._walkcontrol = new b4a.example.jscontrol();
 //BA.debugLineNum = 35;BA.debugLine="Dim pckt As String";
mostCurrent._pckt = "";
 //BA.debugLineNum = 37;BA.debugLine="Dim connect, Disconnect, Active_Button, Mov_xz, T";
mostCurrent._connect = new anywheresoftware.b4a.objects.ButtonWrapper();
mostCurrent._disconnect = new anywheresoftware.b4a.objects.ButtonWrapper();
mostCurrent._active_button = new anywheresoftware.b4a.objects.ButtonWrapper();
mostCurrent._mov_xz = new anywheresoftware.b4a.objects.ButtonWrapper();
mostCurrent._turn_phi = new anywheresoftware.b4a.objects.ButtonWrapper();
mostCurrent._mov_y = new anywheresoftware.b4a.objects.ButtonWrapper();
mostCurrent._turn_thpsi = new anywheresoftware.b4a.objects.ButtonWrapper();
mostCurrent._walk = new anywheresoftware.b4a.objects.ButtonWrapper();
 //BA.debugLineNum = 38;BA.debugLine="Dim Coords_info, IP_info As Label";
mostCurrent._coords_info = new anywheresoftware.b4a.objects.LabelWrapper();
mostCurrent._ip_info = new anywheresoftware.b4a.objects.LabelWrapper();
 //BA.debugLineNum = 40;BA.debugLine="Dim cd1, cd2 As ColorDrawable";
mostCurrent._cd1 = new anywheresoftware.b4a.objects.drawable.ColorDrawable();
mostCurrent._cd2 = new anywheresoftware.b4a.objects.drawable.ColorDrawable();
 //BA.debugLineNum = 41;BA.debugLine="Dim VideoCam As WebView";
mostCurrent._videocam = new anywheresoftware.b4a.objects.WebViewWrapper();
 //BA.debugLineNum = 42;BA.debugLine="Dim WebExtras As WebViewExtras";
mostCurrent._webextras = new uk.co.martinpearman.b4a.webviewextras.WebViewExtras();
 //BA.debugLineNum = 43;BA.debugLine="Dim Spinner1 As Spinner";
mostCurrent._spinner1 = new anywheresoftware.b4a.objects.SpinnerWrapper();
 //BA.debugLineNum = 44;BA.debugLine="End Sub";
return "";
}
public static String  _js1_value_changed(double _angle,double _angledegrees,double _powr) throws Exception{
 //BA.debugLineNum = 275;BA.debugLine="Sub js1_value_changed(Angle As Double, angleDegree";
 //BA.debugLineNum = 276;BA.debugLine="js.Joystick_Action(Angle, powr)";
mostCurrent._js._joystick_action /*String*/ (_angle,_powr);
 //BA.debugLineNum = 278;BA.debugLine="Coords_info.Text = js.GetInfo";
mostCurrent._coords_info.setText(BA.ObjectToCharSequence(mostCurrent._js._getinfo /*String*/ ()));
 //BA.debugLineNum = 280;BA.debugLine="If click_enabled And js.GetMode <> 4 Then";
if (_click_enabled && mostCurrent._js._getmode /*int*/ ()!=4) { 
 //BA.debugLineNum = 281;BA.debugLine="delayTimer.Enabled = True";
_delaytimer.setEnabled(anywheresoftware.b4a.keywords.Common.True);
 //BA.debugLineNum = 282;BA.debugLine="click_enabled = False";
_click_enabled = anywheresoftware.b4a.keywords.Common.False;
 //BA.debugLineNum = 284;BA.debugLine="pckt = js.GetPckt()";
mostCurrent._pckt = mostCurrent._js._getpckt /*String*/ ();
 //BA.debugLineNum = 286;BA.debugLine="If pckt <> \"\" Then";
if ((mostCurrent._pckt).equals("") == false) { 
 //BA.debugLineNum = 287;BA.debugLine="Log(pckt)";
anywheresoftware.b4a.keywords.Common.LogImpl("21245196",mostCurrent._pckt,0);
 //BA.debugLineNum = 288;BA.debugLine="Send_Packet(pckt)";
_send_packet(mostCurrent._pckt);
 };
 };
 //BA.debugLineNum = 293;BA.debugLine="End Sub";
return "";
}
public static String  _mov_xz_click() throws Exception{
 //BA.debugLineNum = 149;BA.debugLine="Private Sub Mov_xz_Click";
 //BA.debugLineNum = 150;BA.debugLine="Active_Button.Background = cd2";
mostCurrent._active_button.setBackground((android.graphics.drawable.Drawable)(mostCurrent._cd2.getObject()));
 //BA.debugLineNum = 152;BA.debugLine="js.SetMode(0)";
mostCurrent._js._setmode /*String*/ ((int) (0));
 //BA.debugLineNum = 153;BA.debugLine="Log(\"Mov(x/z)\")";
anywheresoftware.b4a.keywords.Common.LogImpl("2458756","Mov(x/z)",0);
 //BA.debugLineNum = 155;BA.debugLine="Mov_xz.Background = cd1";
mostCurrent._mov_xz.setBackground((android.graphics.drawable.Drawable)(mostCurrent._cd1.getObject()));
 //BA.debugLineNum = 156;BA.debugLine="Active_Button = Mov_xz";
mostCurrent._active_button = mostCurrent._mov_xz;
 //BA.debugLineNum = 157;BA.debugLine="End Sub";
return "";
}
public static String  _mov_y_click() throws Exception{
 //BA.debugLineNum = 169;BA.debugLine="Private Sub Mov_y_Click";
 //BA.debugLineNum = 170;BA.debugLine="Active_Button.Background = cd2";
mostCurrent._active_button.setBackground((android.graphics.drawable.Drawable)(mostCurrent._cd2.getObject()));
 //BA.debugLineNum = 172;BA.debugLine="js.SetMode(2)";
mostCurrent._js._setmode /*String*/ ((int) (2));
 //BA.debugLineNum = 173;BA.debugLine="Log(\"Mov(y)\")";
anywheresoftware.b4a.keywords.Common.LogImpl("2589828","Mov(y)",0);
 //BA.debugLineNum = 175;BA.debugLine="Mov_y.Background = cd1";
mostCurrent._mov_y.setBackground((android.graphics.drawable.Drawable)(mostCurrent._cd1.getObject()));
 //BA.debugLineNum = 176;BA.debugLine="Active_Button = Mov_y";
mostCurrent._active_button = mostCurrent._mov_y;
 //BA.debugLineNum = 177;BA.debugLine="End Sub";
return "";
}

public static void initializeProcessGlobals() {
    
    if (main.processGlobalsRun == false) {
	    main.processGlobalsRun = true;
		try {
		        main._process_globals();
starter._process_globals();
		
        } catch (Exception e) {
			throw new RuntimeException(e);
		}
    }
}public static String  _process_globals() throws Exception{
 //BA.debugLineNum = 16;BA.debugLine="Sub Process_Globals";
 //BA.debugLineNum = 19;BA.debugLine="Private xui As XUI";
_xui = new anywheresoftware.b4a.objects.B4XViewWrapper.XUI();
 //BA.debugLineNum = 20;BA.debugLine="Private udps As UDPSocket";
_udps = new anywheresoftware.b4a.objects.SocketWrapper.UDPSocket();
 //BA.debugLineNum = 21;BA.debugLine="Private udpp As UDPPacket";
_udpp = new anywheresoftware.b4a.objects.SocketWrapper.UDPSocket.UDPPacket();
 //BA.debugLineNum = 22;BA.debugLine="Dim delayTimer, walkTimer, responseTimer As Timer";
_delaytimer = new anywheresoftware.b4a.objects.Timer();
_walktimer = new anywheresoftware.b4a.objects.Timer();
_responsetimer = new anywheresoftware.b4a.objects.Timer();
 //BA.debugLineNum = 23;BA.debugLine="End Sub";
return "";
}
public static String  _reset_click() throws Exception{
 //BA.debugLineNum = 199;BA.debugLine="Private Sub Reset_Click";
 //BA.debugLineNum = 200;BA.debugLine="Send_Packet(\"A0\")";
_send_packet("A0");
 //BA.debugLineNum = 201;BA.debugLine="End Sub";
return "";
}
public static String  _responsetimer_tick() throws Exception{
 //BA.debugLineNum = 262;BA.debugLine="Sub responseTimer_Tick";
 //BA.debugLineNum = 263;BA.debugLine="responseTimer.Enabled = False";
_responsetimer.setEnabled(anywheresoftware.b4a.keywords.Common.False);
 //BA.debugLineNum = 264;BA.debugLine="If responseReceived = False Then";
if (_responsereceived==anywheresoftware.b4a.keywords.Common.False) { 
 //BA.debugLineNum = 265;BA.debugLine="ToastMessageShow(\"No response from server. Pleas";
anywheresoftware.b4a.keywords.Common.ToastMessageShow(BA.ObjectToCharSequence("No response from server. Please try again."),anywheresoftware.b4a.keywords.Common.True);
 //BA.debugLineNum = 266;BA.debugLine="connect.Text = \"Connect\"";
mostCurrent._connect.setText(BA.ObjectToCharSequence("Connect"));
 //BA.debugLineNum = 267;BA.debugLine="connect.Enabled = True";
mostCurrent._connect.setEnabled(anywheresoftware.b4a.keywords.Common.True);
 };
 //BA.debugLineNum = 269;BA.debugLine="End Sub";
return "";
}
public static String  _send_packet(String _msg) throws Exception{
 //BA.debugLineNum = 116;BA.debugLine="Sub Send_Packet(msg As String)";
 //BA.debugLineNum = 117;BA.debugLine="If serverIp <> \"\" And serverPort <> \"\" Then";
if ((mostCurrent._serverip).equals("") == false && (mostCurrent._serverport).equals("") == false) { 
 //BA.debugLineNum = 118;BA.debugLine="udpp.Initialize(msg.GetBytes(\"ASCII\"), serverIp,";
_udpp.Initialize(_msg.getBytes("ASCII"),mostCurrent._serverip,(int)(Double.parseDouble(mostCurrent._serverport)));
 //BA.debugLineNum = 119;BA.debugLine="udps.Send(udpp)";
_udps.Send(_udpp);
 };
 //BA.debugLineNum = 121;BA.debugLine="End Sub";
return "";
}
public static String  _spinner1_itemclick(int _position,Object _value) throws Exception{
int _js_mode = 0;
 //BA.debugLineNum = 328;BA.debugLine="Private Sub Spinner1_ItemClick (Position As Int, V";
 //BA.debugLineNum = 329;BA.debugLine="Dim js_mode As Int = Position - 1";
_js_mode = (int) (_position-1);
 //BA.debugLineNum = 330;BA.debugLine="If js_mode = -1 Then";
if (_js_mode==-1) { 
 //BA.debugLineNum = 331;BA.debugLine="js_mode = 4";
_js_mode = (int) (4);
 };
 //BA.debugLineNum = 333;BA.debugLine="walkControl.SetMode(js_mode)";
mostCurrent._walkcontrol._setmode /*String*/ (_js_mode);
 //BA.debugLineNum = 334;BA.debugLine="End Sub";
return "";
}
public static String  _tabhost1_tabchanged() throws Exception{
 //BA.debugLineNum = 205;BA.debugLine="Private Sub TabHost1_TabChanged";
 //BA.debugLineNum = 206;BA.debugLine="If TabHost1.CurrentTab = 2 Then 'Camera Page";
if (mostCurrent._tabhost1.getCurrentTab()==2) { 
 //BA.debugLineNum = 207;BA.debugLine="VideoCam.LoadUrl(\"http://192.168.3.107\")";
mostCurrent._videocam.LoadUrl("http://192.168.3.107");
 //BA.debugLineNum = 208;BA.debugLine="WebExtras.executeJavascript(VideoCam, \"document.";
mostCurrent._webextras.executeJavascript((android.webkit.WebView)(mostCurrent._videocam.getObject()),"document.body.style.transform = 'scale(0.65)';"+"document.body.style.transformOrigin = '0 0';");
 };
 //BA.debugLineNum = 211;BA.debugLine="End Sub";
return "";
}
public static String  _turn_phi_click() throws Exception{
 //BA.debugLineNum = 159;BA.debugLine="Private Sub Turn_phi_Click";
 //BA.debugLineNum = 160;BA.debugLine="Active_Button.Background = cd2";
mostCurrent._active_button.setBackground((android.graphics.drawable.Drawable)(mostCurrent._cd2.getObject()));
 //BA.debugLineNum = 162;BA.debugLine="js.SetMode(1)";
mostCurrent._js._setmode /*String*/ ((int) (1));
 //BA.debugLineNum = 163;BA.debugLine="Log(\"Turn(phi)\")";
anywheresoftware.b4a.keywords.Common.LogImpl("2524292","Turn(phi)",0);
 //BA.debugLineNum = 165;BA.debugLine="Turn_phi.Background = cd1";
mostCurrent._turn_phi.setBackground((android.graphics.drawable.Drawable)(mostCurrent._cd1.getObject()));
 //BA.debugLineNum = 166;BA.debugLine="Active_Button = Turn_phi";
mostCurrent._active_button = mostCurrent._turn_phi;
 //BA.debugLineNum = 167;BA.debugLine="End Sub";
return "";
}
public static String  _turn_thpsi_click() throws Exception{
 //BA.debugLineNum = 179;BA.debugLine="Private Sub Turn_thpsi_Click";
 //BA.debugLineNum = 180;BA.debugLine="Active_Button.Background = cd2";
mostCurrent._active_button.setBackground((android.graphics.drawable.Drawable)(mostCurrent._cd2.getObject()));
 //BA.debugLineNum = 182;BA.debugLine="js.SetMode(3)";
mostCurrent._js._setmode /*String*/ ((int) (3));
 //BA.debugLineNum = 183;BA.debugLine="Log(\"Turn(th/psi)\")";
anywheresoftware.b4a.keywords.Common.LogImpl("2655364","Turn(th/psi)",0);
 //BA.debugLineNum = 185;BA.debugLine="Turn_thpsi.Background = cd1";
mostCurrent._turn_thpsi.setBackground((android.graphics.drawable.Drawable)(mostCurrent._cd1.getObject()));
 //BA.debugLineNum = 186;BA.debugLine="Active_Button = Turn_thpsi";
mostCurrent._active_button = mostCurrent._turn_thpsi;
 //BA.debugLineNum = 187;BA.debugLine="End Sub";
return "";
}
public static String  _turnjs_value_changed(double _angle,double _angledegrees,double _powr) throws Exception{
 //BA.debugLineNum = 302;BA.debugLine="Private Sub turnJS_value_changed(angle As Double,";
 //BA.debugLineNum = 303;BA.debugLine="If walkControl.GetMode <> 4 Then";
if (mostCurrent._walkcontrol._getmode /*int*/ ()!=4) { 
 //BA.debugLineNum = 304;BA.debugLine="walkTimer.Enabled = False";
_walktimer.setEnabled(anywheresoftware.b4a.keywords.Common.False);
 //BA.debugLineNum = 305;BA.debugLine="walkControl.Joystick_Action(angle, powr)";
mostCurrent._walkcontrol._joystick_action /*String*/ (_angle,_powr);
 }else {
 //BA.debugLineNum = 307;BA.debugLine="walkControl.SetAngleSpeed(angle, powr)";
mostCurrent._walkcontrol._setanglespeed /*String*/ (_angle,_powr);
 };
 //BA.debugLineNum = 310;BA.debugLine="If walkControl.GetMode <> 4 Or walkControl.powr =";
if (mostCurrent._walkcontrol._getmode /*int*/ ()!=4 || mostCurrent._walkcontrol._powr /*double*/ ==0) { 
 //BA.debugLineNum = 311;BA.debugLine="If click_enabled Then";
if (_click_enabled) { 
 //BA.debugLineNum = 312;BA.debugLine="delayTimer.Enabled = True";
_delaytimer.setEnabled(anywheresoftware.b4a.keywords.Common.True);
 //BA.debugLineNum = 313;BA.debugLine="click_enabled = False";
_click_enabled = anywheresoftware.b4a.keywords.Common.False;
 //BA.debugLineNum = 315;BA.debugLine="pckt = walkControl.GetPckt()";
mostCurrent._pckt = mostCurrent._walkcontrol._getpckt /*String*/ ();
 //BA.debugLineNum = 317;BA.debugLine="If pckt <> \"\" Then";
if ((mostCurrent._pckt).equals("") == false) { 
 //BA.debugLineNum = 318;BA.debugLine="Log(pckt)";
anywheresoftware.b4a.keywords.Common.LogImpl("21376272",mostCurrent._pckt,0);
 //BA.debugLineNum = 319;BA.debugLine="Send_Packet(pckt)";
_send_packet(mostCurrent._pckt);
 };
 };
 };
 //BA.debugLineNum = 324;BA.debugLine="End Sub";
return "";
}
public static String  _udp_packetarrived(anywheresoftware.b4a.objects.SocketWrapper.UDPSocket.UDPPacket _packet) throws Exception{
String _reply_packet = "";
 //BA.debugLineNum = 244;BA.debugLine="Sub UDP_PacketArrived (Packet As UDPPacket)";
 //BA.debugLineNum = 245;BA.debugLine="Dim reply_packet As String = BytesToString(Packet";
_reply_packet = anywheresoftware.b4a.keywords.Common.BytesToString(_packet.getData(),(int) (0),_packet.getLength(),"UTF8");
 //BA.debugLineNum = 246;BA.debugLine="responseReceived = True";
_responsereceived = anywheresoftware.b4a.keywords.Common.True;
 //BA.debugLineNum = 248;BA.debugLine="If reply_packet.StartsWith(\"A\") Then";
if (_reply_packet.startsWith("A")) { 
 //BA.debugLineNum = 249;BA.debugLine="serverIp = ip.Text";
mostCurrent._serverip = mostCurrent._ip.getText();
 //BA.debugLineNum = 250;BA.debugLine="serverPort = port.Text";
mostCurrent._serverport = mostCurrent._port.getText();
 //BA.debugLineNum = 251;BA.debugLine="IP_info.Text = \"Connected to: \" & serverIp & \":\"";
mostCurrent._ip_info.setText(BA.ObjectToCharSequence("Connected to: "+mostCurrent._serverip+":"+mostCurrent._serverport));
 //BA.debugLineNum = 253;BA.debugLine="TabHost1.CurrentTab = 1";
mostCurrent._tabhost1.setCurrentTab((int) (1));
 //BA.debugLineNum = 254;BA.debugLine="Disconnect.Visible = True";
mostCurrent._disconnect.setVisible(anywheresoftware.b4a.keywords.Common.True);
 //BA.debugLineNum = 255;BA.debugLine="connect.Visible = False";
mostCurrent._connect.setVisible(anywheresoftware.b4a.keywords.Common.False);
 //BA.debugLineNum = 256;BA.debugLine="connected = True";
_connected = anywheresoftware.b4a.keywords.Common.True;
 }else if(_reply_packet.startsWith("R")) { 
 //BA.debugLineNum = 258;BA.debugLine="ToastMessageShow(\"Rejected connection to server\"";
anywheresoftware.b4a.keywords.Common.ToastMessageShow(BA.ObjectToCharSequence("Rejected connection to server"),anywheresoftware.b4a.keywords.Common.True);
 };
 //BA.debugLineNum = 260;BA.debugLine="End Sub";
return "";
}
public static String  _walk_click() throws Exception{
 //BA.debugLineNum = 189;BA.debugLine="Private Sub Walk_Click";
 //BA.debugLineNum = 190;BA.debugLine="Active_Button.Background = cd2";
mostCurrent._active_button.setBackground((android.graphics.drawable.Drawable)(mostCurrent._cd2.getObject()));
 //BA.debugLineNum = 192;BA.debugLine="js.SetMode(4)";
mostCurrent._js._setmode /*String*/ ((int) (4));
 //BA.debugLineNum = 193;BA.debugLine="Log(\"Walk\")";
anywheresoftware.b4a.keywords.Common.LogImpl("2720900","Walk",0);
 //BA.debugLineNum = 195;BA.debugLine="walk.Background = cd1";
mostCurrent._walk.setBackground((android.graphics.drawable.Drawable)(mostCurrent._cd1.getObject()));
 //BA.debugLineNum = 196;BA.debugLine="Active_Button = walk";
mostCurrent._active_button = mostCurrent._walk;
 //BA.debugLineNum = 197;BA.debugLine="End Sub";
return "";
}
public static String  _walkjs_value_changed(double _angle,double _angledegrees,double _powr) throws Exception{
 //BA.debugLineNum = 295;BA.debugLine="Private Sub walkJS_value_changed(angle As Double,";
 //BA.debugLineNum = 296;BA.debugLine="walkControl.SetMode(4)";
mostCurrent._walkcontrol._setmode /*String*/ ((int) (4));
 //BA.debugLineNum = 297;BA.debugLine="Spinner1.SelectedIndex = 0";
mostCurrent._spinner1.setSelectedIndex((int) (0));
 //BA.debugLineNum = 298;BA.debugLine="walkTimer.Enabled = True";
_walktimer.setEnabled(anywheresoftware.b4a.keywords.Common.True);
 //BA.debugLineNum = 299;BA.debugLine="walkControl.Joystick_Action(angle, powr)";
mostCurrent._walkcontrol._joystick_action /*String*/ (_angle,_powr);
 //BA.debugLineNum = 300;BA.debugLine="End Sub";
return "";
}
public static String  _walktimer_tick() throws Exception{
 //BA.debugLineNum = 128;BA.debugLine="Sub walkTimer_Tick";
 //BA.debugLineNum = 129;BA.debugLine="If TabHost1.CurrentTab = 1 Then";
if (mostCurrent._tabhost1.getCurrentTab()==1) { 
 //BA.debugLineNum = 130;BA.debugLine="pckt = js.GetPckt()";
mostCurrent._pckt = mostCurrent._js._getpckt /*String*/ ();
 //BA.debugLineNum = 131;BA.debugLine="If pckt <> \"\" Then";
if ((mostCurrent._pckt).equals("") == false) { 
 //BA.debugLineNum = 132;BA.debugLine="Log(pckt)";
anywheresoftware.b4a.keywords.Common.LogImpl("2393220",mostCurrent._pckt,0);
 //BA.debugLineNum = 133;BA.debugLine="Send_Packet(pckt)";
_send_packet(mostCurrent._pckt);
 };
 }else if(mostCurrent._tabhost1.getCurrentTab()==2) { 
 //BA.debugLineNum = 136;BA.debugLine="pckt = walkControl.GetPckt()";
mostCurrent._pckt = mostCurrent._walkcontrol._getpckt /*String*/ ();
 //BA.debugLineNum = 138;BA.debugLine="If pckt <> \"\" Then";
if ((mostCurrent._pckt).equals("") == false) { 
 //BA.debugLineNum = 139;BA.debugLine="Log(pckt)";
anywheresoftware.b4a.keywords.Common.LogImpl("2393227",mostCurrent._pckt,0);
 //BA.debugLineNum = 140;BA.debugLine="Send_Packet(pckt)";
_send_packet(mostCurrent._pckt);
 };
 };
 //BA.debugLineNum = 143;BA.debugLine="End Sub";
return "";
}
}
