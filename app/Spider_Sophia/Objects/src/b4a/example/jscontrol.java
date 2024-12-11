package b4a.example;


import anywheresoftware.b4a.BA;
import anywheresoftware.b4a.B4AClass;
import anywheresoftware.b4a.BALayout;
import anywheresoftware.b4a.debug.*;

public class jscontrol extends B4AClass.ImplB4AClass implements BA.SubDelegator{
    private static java.util.HashMap<String, java.lang.reflect.Method> htSubs;
    private void innerInitialize(BA _ba) throws Exception {
        if (ba == null) {
            ba = new BA(_ba, this, htSubs, "b4a.example.jscontrol");
            if (htSubs == null) {
                ba.loadHtSubs(this.getClass());
                htSubs = ba.htSubs;
            }
            
        }
        if (BA.isShellModeRuntimeCheck(ba)) 
			   this.getClass().getMethod("_class_globals", b4a.example.jscontrol.class).invoke(this, new Object[] {null});
        else
            ba.raiseEvent2(null, true, "class_globals", false);
    }

 public anywheresoftware.b4a.keywords.Common __c = null;
public double _angle = 0;
public double _powr = 0;
public double _radio = 0;
public double _ang_speed = 0;
public double _x = 0;
public double _y = 0;
public double _z = 0;
public double _thx = 0;
public double _thy = 0;
public double _thz = 0;
public int _mode = 0;
public b4a.example.main _main = null;
public b4a.example.starter _starter = null;
public String  _class_globals() throws Exception{
 //BA.debugLineNum = 1;BA.debugLine="Sub Class_Globals";
 //BA.debugLineNum = 2;BA.debugLine="Dim angle As Double";
_angle = 0;
 //BA.debugLineNum = 3;BA.debugLine="Dim powr As Double";
_powr = 0;
 //BA.debugLineNum = 4;BA.debugLine="Dim radio As Double";
_radio = 0;
 //BA.debugLineNum = 5;BA.debugLine="Dim ang_speed As Double";
_ang_speed = 0;
 //BA.debugLineNum = 7;BA.debugLine="Dim x, y, z, thx, thy, thz As Double";
_x = 0;
_y = 0;
_z = 0;
_thx = 0;
_thy = 0;
_thz = 0;
 //BA.debugLineNum = 9;BA.debugLine="Dim mode As Int";
_mode = 0;
 //BA.debugLineNum = 10;BA.debugLine="End Sub";
return "";
}
public String  _getinfo() throws Exception{
 //BA.debugLineNum = 62;BA.debugLine="Public Sub GetInfo As String";
 //BA.debugLineNum = 63;BA.debugLine="Return \"Ang = \" & Round2(angle, 2) & CRLF & _";
if (true) return "Ang = "+BA.NumberToString(__c.Round2(_angle,(int) (2)))+__c.CRLF+"Pwr = "+BA.NumberToString(__c.Round2(_powr,(int) (2)))+__c.CRLF+"x   = "+BA.NumberToString(__c.Round2(_x,(int) (2)))+__c.CRLF+"y   = "+BA.NumberToString(__c.Round2(_y,(int) (2)))+__c.CRLF+"z   = "+BA.NumberToString(__c.Round2(_z,(int) (2)))+__c.CRLF+"th  = "+BA.NumberToString(__c.Round2(_thx,(int) (2)))+__c.CRLF+"phi = "+BA.NumberToString(__c.Round2(_thy,(int) (2)))+__c.CRLF+"psi = "+BA.NumberToString(__c.Round2(_thz,(int) (2)));
 //BA.debugLineNum = 71;BA.debugLine="End Sub";
return "";
}
public int  _getmode() throws Exception{
 //BA.debugLineNum = 29;BA.debugLine="Public Sub GetMode As Int";
 //BA.debugLineNum = 30;BA.debugLine="Return mode";
if (true) return _mode;
 //BA.debugLineNum = 31;BA.debugLine="End Sub";
return 0;
}
public String  _getpckt() throws Exception{
String _pckt = "";
 //BA.debugLineNum = 73;BA.debugLine="Public Sub GetPckt() As String";
 //BA.debugLineNum = 74;BA.debugLine="Dim pckt As String = \"\"";
_pckt = "";
 //BA.debugLineNum = 75;BA.debugLine="If mode = 0 And (x <> 0 Or z <> 0) Or mode = 1 An";
if (_mode==0 && (_x!=0 || _z!=0) || _mode==1 && _thy!=0 || _mode==2 && _y!=0 || _mode==3 && (_thx!=0 || _thz!=0)) { 
 //BA.debugLineNum = 76;BA.debugLine="pckt = \"A\" & Round2(x, 2) & \",\" & Round2(y, 2) &";
_pckt = "A"+BA.NumberToString(__c.Round2(_x,(int) (2)))+","+BA.NumberToString(__c.Round2(_y,(int) (2)))+","+BA.NumberToString(__c.Round2(_z,(int) (2)))+","+BA.NumberToString(__c.Round2(_thx,(int) (2)))+","+BA.NumberToString(__c.Round2(_thy,(int) (2)))+","+BA.NumberToString(__c.Round2(_thz,(int) (2)));
 }else if(_mode==4) { 
 //BA.debugLineNum = 78;BA.debugLine="pckt = \"W\" & Round2(angle, 2) & \",\" & Round2(rad";
_pckt = "W"+BA.NumberToString(__c.Round2(_angle,(int) (2)))+","+BA.NumberToString(__c.Round2(_radio,(int) (2)))+","+BA.NumberToString(__c.Round2(_ang_speed,(int) (2)));
 };
 //BA.debugLineNum = 80;BA.debugLine="Return pckt";
if (true) return _pckt;
 //BA.debugLineNum = 81;BA.debugLine="End Sub";
return "";
}
public String  _initialize(anywheresoftware.b4a.BA _ba,int _m) throws Exception{
innerInitialize(_ba);
 //BA.debugLineNum = 13;BA.debugLine="Public Sub Initialize(m As Int)";
 //BA.debugLineNum = 14;BA.debugLine="SetValues(0,0)";
_setvalues(0,0);
 //BA.debugLineNum = 15;BA.debugLine="ang_speed = 0";
_ang_speed = 0;
 //BA.debugLineNum = 16;BA.debugLine="SetMode(m)";
_setmode(_m);
 //BA.debugLineNum = 17;BA.debugLine="End Sub";
return "";
}
public String  _joystick_action(double _ang,double _pwr) throws Exception{
float _radlmt = 0f;
 //BA.debugLineNum = 39;BA.debugLine="Public Sub Joystick_Action(ang As Double, pwr As D";
 //BA.debugLineNum = 40;BA.debugLine="SetValues(ang, pwr)";
_setvalues(_ang,_pwr);
 //BA.debugLineNum = 41;BA.debugLine="radio = powr / 100";
_radio = _powr/(double)100;
 //BA.debugLineNum = 42;BA.debugLine="Dim radlmt As Float= 15 * (cPI / 180)";
_radlmt = (float) (15*(__c.cPI/(double)180));
 //BA.debugLineNum = 44;BA.debugLine="If mode = 0 Then 'Mov(x/z)";
if (_mode==0) { 
 //BA.debugLineNum = 45;BA.debugLine="x = radio * Sin(angle) * 2";
_x = _radio*__c.Sin(_angle)*2;
 //BA.debugLineNum = 46;BA.debugLine="z = radio * Cos(angle) * 2";
_z = _radio*__c.Cos(_angle)*2;
 }else if(_mode==1) { 
 //BA.debugLineNum = 49;BA.debugLine="thy = radio * Sin (angle) * radlmt";
_thy = _radio*__c.Sin(_angle)*_radlmt;
 }else if(_mode==2) { 
 //BA.debugLineNum = 52;BA.debugLine="y = radio * Cos (angle) * 3";
_y = _radio*__c.Cos(_angle)*3;
 }else if(_mode==3) { 
 //BA.debugLineNum = 55;BA.debugLine="thx = radio * Sin (angle) * radlmt";
_thx = _radio*__c.Sin(_angle)*_radlmt;
 //BA.debugLineNum = 56;BA.debugLine="thz = radio * Cos (angle) * radlmt";
_thz = _radio*__c.Cos(_angle)*_radlmt;
 }else if(_mode==4) { 
 //BA.debugLineNum = 58;BA.debugLine="radio = radio * 0.5";
_radio = _radio*0.5;
 };
 //BA.debugLineNum = 60;BA.debugLine="End Sub";
return "";
}
public String  _setanglespeed(double _ang,double _pwr) throws Exception{
 //BA.debugLineNum = 33;BA.debugLine="Public Sub SetAngleSpeed(ang As Double, pwr As Dou";
 //BA.debugLineNum = 34;BA.debugLine="ang = ang - cPI / 2";
_ang = _ang-__c.cPI/(double)2;
 //BA.debugLineNum = 35;BA.debugLine="If ang < (-cPI) Then ang = ang + 2*cPI";
if (_ang<(-__c.cPI)) { 
_ang = _ang+2*__c.cPI;};
 //BA.debugLineNum = 36;BA.debugLine="ang_speed = pwr / 100 * Sin (ang) * (cPI / 180)";
_ang_speed = _pwr/(double)100*__c.Sin(_ang)*(__c.cPI/(double)180);
 //BA.debugLineNum = 37;BA.debugLine="End Sub";
return "";
}
public String  _setmode(int _m) throws Exception{
 //BA.debugLineNum = 25;BA.debugLine="Public Sub SetMode(m As Int)";
 //BA.debugLineNum = 26;BA.debugLine="mode = m";
_mode = _m;
 //BA.debugLineNum = 27;BA.debugLine="End Sub";
return "";
}
public String  _setvalues(double _ang,double _pwr) throws Exception{
 //BA.debugLineNum = 19;BA.debugLine="Public Sub SetValues(ang As Double, pwr As Double)";
 //BA.debugLineNum = 20;BA.debugLine="angle = ang - cPI / 2";
_angle = _ang-__c.cPI/(double)2;
 //BA.debugLineNum = 21;BA.debugLine="If angle < (-cPI) Then angle = angle + 2*cPI";
if (_angle<(-__c.cPI)) { 
_angle = _angle+2*__c.cPI;};
 //BA.debugLineNum = 22;BA.debugLine="powr = pwr";
_powr = _pwr;
 //BA.debugLineNum = 23;BA.debugLine="End Sub";
return "";
}
public Object callSub(String sub, Object sender, Object[] args) throws Exception {
BA.senderHolder.set(sender);
return BA.SubDelegator.SubNotFound;
}
}
