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
	Dim ang_speed As Double
	
	Dim x, y, z, thx, thy, thz As Double

	Dim mode As Int
End Sub

'Initializes the object. You can add parameters to this method if needed.
Public Sub Initialize(m As Int)
	SetValues(0,0)
	ang_speed = 0
	SetMode(m)
End Sub

Public Sub SetValues(ang As Double, pwr As Double)
	angle = ang - cPI / 2
	If angle < (-cPI) Then angle = angle + 2*cPI
	powr = pwr
End Sub

Public Sub SetMode(m As Int)
	mode = m
End Sub

Public Sub GetMode As Int
	Return mode
End Sub

Public Sub SetAngleSpeed(ang As Double, pwr As Double)
	ang = ang - cPI / 2
	If ang < (-cPI) Then ang = ang + 2*cPI
	ang_speed = pwr / 100 * Sin (ang) * (cPI / 180)
End Sub

Public Sub Joystick_Action(ang As Double, pwr As Double)
	SetValues(ang, pwr)
	radio = powr / 100
	Dim radlmt As Float= 15 * (cPI / 180)
	
	If mode = 0 Then 'Mov(x/z)
		x = radio * Sin(angle) * 2
		z = radio * Cos(angle) * 2
		
	Else If mode = 1 Then 'Turn(phi)
		thy = radio * Sin (angle) * radlmt
		
	Else If mode = 2 Then 'Mov(y)
		y = radio * Cos (angle) * 3
		
	Else If mode = 3 Then 'Turn(th/psi)
		thx = radio * Sin (angle) * radlmt
		thz = radio * Cos (angle) * radlmt
	Else If mode = 4 Then 'Turn
		radio = radio * 0.5
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

Public Sub GetPckt() As String
	Dim pckt As String = ""
	If mode = 0 And (x <> 0 Or z <> 0) Or mode = 1 And thy <> 0 Or mode = 2 And y <> 0 Or mode = 3 And (thx <> 0 Or thz <> 0) Then
		pckt = "A" & Round2(x, 2) & "," & Round2(y, 2) & "," & Round2(z, 2) & "," & Round2(thx, 2) & "," & Round2(thy, 2) & "," & Round2(thz, 2)
	Else If mode = 4 Then
		pckt = "W" & Round2(angle, 2) & "," & Round2(radio, 2) & "," & Round2(ang_speed, 2)
	End If
	Return pckt
End Sub