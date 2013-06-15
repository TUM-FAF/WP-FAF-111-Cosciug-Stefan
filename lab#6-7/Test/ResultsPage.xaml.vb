
Public NotInheritable Class ResultsPage
    Inherits Common.LayoutAwarePage

    Protected Overrides Sub LoadState(navigationParameter As Object, pageState As Dictionary(Of String, Object))

        If pageState IsNot Nothing Then
            ' Restoring the chosen complexity level
            If (pageState.ContainsKey("P3_Complexity")) Then
                Complexity = pageState("P3_Complexity")
            End If
            ' Restoring the user name
            If (pageState.ContainsKey("P3_UserName")) Then
                UserName = pageState("P3_UserName")
            End If
            ' Restoring the question number
            If (pageState.ContainsKey("P3_QuestionNr")) Then
                CurrentQuestion = pageState("P3_QuestionNr")
            End If
            ' Restoring the answers
            If (pageState.ContainsKey("P3_Answers")) Then
                Dim Str As String
                Str = pageState("P3_Answers")
                Dim i
                For i = 0 To 9
                    Answers(i) = System.Int32.Parse(Str(i))
                Next
            End If
            ' Restoring the number of correct answers
            If (pageState.ContainsKey("P3_NrOfCorrectAnswers")) Then
                NrOfCorrectAnswers = pageState("P3_NrOfCorrectAnswers")
            End If
        End If
    End Sub

    Protected Overrides Sub SaveState(pageState As Dictionary(Of String, Object))
        ' Saving the chosen complexity level
        pageState("P3_Complexity") = Complexity
        ' Saving the user name
        pageState("P3_UserName") = UserName
        ' Saving the current question number
        pageState("P3_QuestionNr") = CurrentQuestion
        ' Saving the answers
        Dim Str As String = String.Join("", Answers.ToArray())
        pageState("P3_Answers") = Str
        pageState("P3_NrOfCorrectAnswers") = NrOfCorrectAnswers
    End Sub

    Private Sub Load(sender As Object, e As RoutedEventArgs)
        Name_box.Text = UserName
        Ans_box.Text = NrOfCorrectAnswers & " / 10"
        Perc_box.Text = NrOfCorrectAnswers * 10 & " %"
        Select Case Complexity
            Case 1
                Diff_box.Text = "Easy"
            Case 2
                Diff_box.Text = "Medium"
            Case 3
                Diff_box.Text = "Advanced"
        End Select
    End Sub

    Private Sub Exit_but_Click(sender As Object, e As RoutedEventArgs) Handles Exit_but.Click
        App.Current.Exit()
    End Sub
End Class
