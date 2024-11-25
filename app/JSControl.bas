B4A=true
Group=Default Group
ModulesStructureVersion=1
Type=Class
Version=12.8
@EndOfDesignText@
Sub Class_Globals
	Dim angle As Double
	Dim powr As Double
	Dim radio As Double
	
	Dim x, y, z, thx, thy, thz As Double
End Sub

'Initializes the object. You can add parameters to this method if needed.
Public Sub Initialize
	SetValues(0,0)
End Sub

Public Sub SetValues(ang As Double, pwr As Double)
	angle = ang - cPI / 2
	If angle < (-cPI) Then angle = angle + 2*cPI
	powr = pwr
End Sub

Public Sub Joystick_Action(mode As Int)
	If mode = 0 Then 'Mov(x/z)
		radio = powr / 100 * 4
	
		x = Min( Max(radio * Sin(angle), -4), 4 )
		z = Min( Max(radio * Cos(angle), -4), 4 )
		
	Else If mode = 1 Then 'Turn(phi)
		Dim radlmt As Float= 20 * (cPI / 180)
		radio = powr / 100 * radlmt
		
		thy = Min( Max(radio * Sin (angle), -radlmt), radlmt )
		
	Else If mode = 2 Then 'Mov(y)
		radio = powr / 100 * 4 '( 5 - (-3) ) / 2
		y = Min( Max(radio * Cos (angle), -4), 4 )
		
	Else If mode = 3 Then 'Turn(th/psi)
		Dim radlmt As Float= 20 * (cPI / 180)
		radio = powr / 100 * radlmt
		
		thx = Min( Max(radio * Sin (angle), -radlmt), radlmt )
		thz = Min( Max(radio * Cos (angle), -radlmt), radlmt )
	End If
End Sub

Public Sub GetInfo As String
	Return "Ang = " & Round2(angle, 2) & CRLF & _
					   "Pwr = " & Round2(powr, 2) & CRLF & _
					   "x   = " & Round2(x, 2) & CRLF & _
                       "y   = " & Round2(y, 2) & CRLF & _
                   	   "z   = " & Round2(z, 2) & CRLF & _
                       "th  = " & Round2(thx, 2) & CRLF & _
                       "phi = " & Round2(thy, 2) & CRLF & _
                       "psi = " & Round2(thz, 2)
End Sub

Public Sub GetPckt(mode As Int) As String
	Dim pckt As String = ""
	If mode = 0 And (x <> 0 Or z <> 0) Or mode = 1 And thy <> 0 Or mode = 2 And y <> 0 Or mode = 3 And (thx <> 0 Or thz <> 0) Then
		pckt = "A" & Round2(x, 2) & "," & Round2(y, 2) & "," & Round2(z, 2) & "," & Round2(thx, 2) & "," & Round2(thy, 2) & "," & Round2(thz, 2)
	End If
		
	Return pckt
End Sub

Public Sub GetWalkPckt(mode As Int) As String
	Dim pckt As String = ""
	If mode = 4 And powr <> 0 Then
		radio = Max( powr / 100 * 1.2, 0.2)
		pckt = "W" & Round2(angle, 2) & "," & Round2(radio, 2)
	End If
	
	Return pckt
End Sub