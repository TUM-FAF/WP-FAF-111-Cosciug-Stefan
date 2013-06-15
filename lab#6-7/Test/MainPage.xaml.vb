
Public NotInheritable Class MainPage
    Inherits Common.LayoutAwarePage

        Protected Overrides Sub LoadState(navigationParameter As Object, pageState As Dictionary(Of String, Object))
        If pageState IsNot Nothing AndAlso pageState.ContainsKey("P1_NameInputBox") Then
            Name_Box.Text = pageState("P1_NameInputBox").ToString()
        End If
    End Sub

    Protected Overrides Sub SaveState(pageState As Dictionary(Of String, Object))
        pageState("P1_NameInputBox") = Name_Box.Text
    End Sub

    Private Sub Submit_But_Click(sender As Object, e As RoutedEventArgs) Handles Submit_But.Click
        If (Name_Box.Text = "") Then
            UserName = "User"
        Else
            UserName = Name_Box.Text
        End If
        ' Navigate to the second page
        Me.Frame.Navigate(GetType(TestPage))
    End Sub
End Class
