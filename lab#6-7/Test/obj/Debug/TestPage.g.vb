﻿

#ExternalChecksum("C:\Users\lkjlj\documents\visual studio 2012\Projects\Test\Test\TestPage.xaml", "{406ea660-64cf-4c82-b6f0-42d48172a799}", "305F64386CD42BCC9D2D271701B0278B")
'------------------------------------------------------------------------------
' <auto-generated>
'     This code was generated by a tool.
'
'     Changes to this file may cause incorrect behavior and will be lost if
'     the code is regenerated.
' </auto-generated>
'------------------------------------------------------------------------------

Option Strict Off
Option Explicit On

Namespace Global.Test

    Partial Class TestPage
        Implements Global.Windows.UI.Xaml.Markup.IComponentConnector

        <Global.System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.Windows.UI.Xaml.Build.Tasks", "4.0.0.0")>  _
        <Global.System.Diagnostics.DebuggerNonUserCodeAttribute()>  _
        Public Sub Connect(ByVal connectionId As Integer, ByVal target As Global.System.Object) Implements Global.Windows.UI.Xaml.Markup.IComponentConnector.Connect
            If(connectionId = 1) Then
                #ExternalSource("..\..\TestPage.xaml",24)
                AddHandler CType(target,Global.Windows.UI.Xaml.FrameworkElement).Loaded, AddressOf Me.Load
                #End ExternalSource
            Else If(connectionId = 2) Then
                #ExternalSource("..\..\TestPage.xaml",58)
                AddHandler CType(target,Global.Windows.UI.Xaml.Controls.Primitives.ButtonBase).Click, AddressOf Me.Button_Click_1
                #End ExternalSource
            End If
            Me._contentLoaded = true
        End Sub
    End Class

End Namespace


