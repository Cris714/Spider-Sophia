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
public double _x = 0;
public double _y = 0;
public double _z = 0;
public double _thx = 0;
public double _thy = 0;
public double _thz = 0;
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
 //BA.debugLineNum = 6;BA.debugLine="Dim x, y, z, thx, thy, thz As Double";
_x = 0;
_y = 0;
_z = 0;
_thx = 0;
_thy = 0;
_thz = 0;
 //BA.debugLineNum = 7;BA.debugLine="End Sub";
return "";
}
public String  _getinfo() throws Exception{
 //BA.debugLineNum = 46;BA.debugLine="Public Sub GetInfo As String";
 //BA.debugLineNum = 47;BA.debugLine="Return \"Ang = \" & Round2(angle, 2) & CRLF & _";
if (true) return "Ang = "+BA.NumberToString(__c.Round2(_angle,(int) (2)))+__c.CRLF+"Pwr = "+BA.NumberToString(__c.Round2(_powr,(int) (2)))+__c.CRLF+"x   = "+BA.NumberToString(__c.Round2(_x,(int) (2)))+__c.CRLF+"y   = "+BA.NumberToString(__c.Round2(_y,(int) (2)))+__c.CRLF+"z   = "+BA.NumberToString(__c.Round2(_z,(int) (2)))+__c.CRLF+"th  = "+BA.NumberToString(__c.Round2(_thx,(int) (2)))+__c.CRLF+"phi = "+BA.NumberToString(__c.Round2(_thy,(int) (2)))+__c.CRLF+"psi = "+BA.NumberToString(__c.Round2(_thz,(int) (2)));
 //BA.debugLineNum = 55;BA.debugLine="End Sub";
return "";
}
public String  _getpckt(int _mode) throws Exception{
String _pckt = "";
 //BA.debugLineNum = 57;BA.debugLine="Public Sub GetPckt(mode As Int) As String";
 //BA.debugLineNum = 58;BA.debugLine="Dim pckt As String = \"\"";
_pckt = "";
 //BA.debugLineNum = 59;BA.debugLine="If mode = 0 And (x <> 0 Or z <> 0) Or mode = 1 An";
if (_mode==0 && (_x!=0 || _z!=0) || _mode==1 && _thy!=0 || _mode==2 && _y!=0 || _mode==3 && (_thx!=0 || _thz!=0)) { 
 //BA.debugLineNum = 60;BA.debugLine="pckt = \"A\" & Round2(x, 2) & \",\" & Round2(y, 2) &";
_pckt = "A"+BA.NumberToString(__c.Round2(_x,(int) (2)))+","+BA.NumberToString(__c.Round2(_y,(int) (2)))+","+BA.NumberToString(__c.Round2(_z,(int) (2)))+","+BA.NumberToString(__c.Round2(_thx,(int) (2)))+","+BA.NumberToString(__c.Round2(_thy,(int) (2)))+","+BA.NumberToString(__c.Round2(_thz,(int) (2)));
 };
 //BA.debugLineNum = 63;BA.debugLine="Return pckt";
if (true) return _pckt;
 //BA.debugLineNum = 64;BA.debugLine="End Sub";
return "";
}
public String  _getwalkpckt(int _mode) throws Exception{
String _pckt = "";
 //BA.debugLineNum = 66;BA.debugLine="Public Sub GetWalkPckt(mode As Int) As String";
 //BA.debugLineNum = 67;BA.debugLine="Dim pckt As String = \"\"";
_pckt = "";
 //BA.debugLineNum = 68;BA.debugLine="If mode = 4 And powr <> 0 Then";
if (_mode==4 && _powr!=0) { 
 //BA.debugLineNum = 69;BA.debugLine="radio = Max( powr / 100 * 1.2, 0.2)";
_radio = __c.Max(_powr/(double)100*1.2,0.2);
 //BA.debugLineNum = 70;BA.debugLine="pckt = \"W\" & Round2(angle, 2) & \",\" & Round2(rad";
_pckt = "W"+BA.NumberToString(__c.Round2(_angle,(int) (2)))+","+BA.NumberToString(__c.Round2(_radio,(int) (2)));
 };
 //BA.debugLineNum = 73;BA.debugLine="Return pckt";
if (true) return _pckt;
 //BA.debugLineNum = 74;BA.debugLine="End Sub";
return "";
}
public String  _initialize(anywheresoftware.b4a.BA _ba) throws Exception{
innerInitialize(_ba);
 //BA.debugLineNum = 10;BA.debugLine="Public Sub Initialize";
 //BA.debugLineNum = 11;BA.debugLine="SetValues(0,0)";
_setvalues(0,0);
 //BA.debugLineNum = 12;BA.debugLine="End Sub";
return "";
}
public String  _joystick_action(int _mode) throws Exception{
float _radlmt = 0f;
 //BA.debugLineNum = 20;BA.debugLine="Public Sub Joystick_Action(mode As Int)";
 //BA.debugLineNum = 21;BA.debugLine="If mode = 0 Then 'Mov(x/z)";
if (_mode==0) { 
 //BA.debugLineNum = 22;BA.debugLine="radio = powr / 100 * 4";
_radio = _powr/(double)100*4;
 //BA.debugLineNum = 24;BA.debugLine="x = Min( Max(radio * Sin(angle), -4), 4 )";
_x = __c.Min(__c.Max(_radio*__c.Sin(_angle),-4),4);
 //BA.debugLineNum = 25;BA.debugLine="z = Min( Max(radio * Cos(angle), -4), 4 )";
_z = __c.Min(__c.Max(_radio*__c.Cos(_angle),-4),4);
 }else if(_mode==1) { 
 //BA.debugLineNum = 28;BA.debugLine="Dim radlmt As Float= 20 * (cPI / 180)";
_radlmt = (float) (20*(__c.cPI/(double)180));
 //BA.debugLineNum = 29;BA.debugLine="radio = powr / 100 * radlmt";
_radio = _powr/(double)100*_radlmt;
 //BA.debugLineNum = 31;BA.debugLine="thy = Min( Max(radio * Sin (angle), -radlmt), ra";
_thy = __c.Min(__c.Max(_radio*__c.Sin(_angle),-_radlmt),_radlmt);
 }else if(_mode==2) { 
 //BA.debugLineNum = 34;BA.debugLine="radio = powr / 100 * 4 '( 5 - (-3) ) / 2";
_radio = _powr/(double)100*4;
 //BA.debugLineNum = 35;BA.debugLine="y = Min( Max(radio * Cos (angle), -4), 4 )";
_y = __c.Min(__c.Max(_radio*__c.Cos(_angle),-4),4);
 }else if(_mode==3) { 
 //BA.debugLineNum = 38;BA.debugLine="Dim radlmt As Float= 20 * (cPI / 180)";
_radlmt = (float) (20*(__c.cPI/(double)180));
 //BA.debugLineNum = 39;BA.debugLine="radio = powr / 100 * radlmt";
_radio = _powr/(double)100*_radlmt;
 //BA.debugLineNum = 41;BA.debugLine="thx = Min( Max(radio * Sin (angle), -radlmt), ra";
_thx = __c.Min(__c.Max(_radio*__c.Sin(_angle),-_radlmt),_radlmt);
 //BA.debugLineNum = 42;BA.debugLine="thz = Min( Max(radio * Cos (angle), -radlmt), ra";
_thz = __c.Min(__c.Max(_radio*__c.Cos(_angle),-_radlmt),_radlmt);
 };
 //BA.debugLineNum = 44;BA.debugLine="End Sub";
return "";
}
public String  _setvalues(double _ang,double _pwr) throws Exception{
 //BA.debugLineNum = 14;BA.debugLine="Public Sub SetValues(ang As Double, pwr As Double)";
 //BA.debugLineNum = 15;BA.debugLine="angle = ang - cPI / 2";
_angle = _ang-__c.cPI/(double)2;
 //BA.debugLineNum = 16;BA.debugLine="If angle < (-cPI) Then angle = angle + 2*cPI";
if (_angle<(-__c.cPI)) { 
_angle = _angle+2*__c.cPI;};
 //BA.debugLineNum = 17;BA.debugLine="powr = pwr";
_powr = _pwr;
 //BA.debugLineNum = 18;BA.debugLine="End Sub";
return "";
}
public Object callSub(String sub, Object sender, Object[] args) throws Exception {
BA.senderHolder.set(sender);
return BA.SubDelegator.SubNotFound;
}
}
