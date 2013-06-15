
Public NotInheritable Class TestPage
    Inherits Common.LayoutAwarePage

    Protected Overrides Sub LoadState(navigationParameter As Object, pageState As Dictionary(Of String, Object))
        If pageState IsNot Nothing Then
            ' Restoring the chosen complexity level
            If (pageState.ContainsKey("P2_Complexity")) Then
                Complexity = pageState("P2_Complexity")
            End If
            ' Restoring the user name
            If (pageState.ContainsKey("P2_UserName")) Then
                UserName = pageState("P2_UserName")
            End If
            ' Restoring the question number
            If (pageState.ContainsKey("P2_QuestionNr")) Then
                CurrentQuestion = pageState("P2_QuestionNr")
            End If
            ' Restoring the answers
            If (pageState.ContainsKey("P2_Answers")) Then
                Dim Str As String
                Str = pageState("P2_Answers")
                Dim i
                For i = 0 To 9
                    Answers(i) = System.Int32.Parse(Str(i))
                Next
            End If
            ' Restoring the number of correct answers
            If (pageState.ContainsKey("P2_NrOfCorrectAnswers")) Then
                NrOfCorrectAnswers = pageState("P2_NrOfCorrectAnswers")
            End If
        End If
    End Sub

    Protected Overrides Sub SaveState(pageState As Dictionary(Of String, Object))

        ' Case when a pause has been scheduled or the timer is currently active but no choice was made
        If (CurrentQuestion < 10 And (TimeButtonState = True Or Time_but.IsEnabled = False)) Then
            Answers(CurrentQuestion) = 8
            CurrentQuestion = CurrentQuestion + 1
        End If

        ' Saving the chosen complexity level
        pageState("P2_Complexity") = Complexity
        ' Saving the user name
        pageState("P2_UserName") = UserName
        ' Saving the current question number
        pageState("P2_QuestionNr") = CurrentQuestion
        ' Saving the answers
        Dim Str As String = String.Join("", Answers.ToArray())
        pageState("P2_Answers") = Str
        pageState("P2_NrOfCorrectAnswers") = NrOfCorrectAnswers
    End Sub

    Private Sub Load(sender As Object, e As RoutedEventArgs)
        ' Setting the complexity level
        Select Case Complexity
            Case 1
                Complexity1.IsChecked = True
            Case 2
                Complexity2.IsChecked = True
            Case 3
                Complexity3.IsChecked = True
            Case Else
                Complexity2.IsChecked = True
                Complexity = 2
        End Select
        ' Setting initial values
        TimeButtonState = False
        ' Creating a pointer to an array of Push Buttons
        Q_but = New Button() {Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9, Q10}
        ' Creating a pointer to an array of Radio Buttons
        A_rad = New RadioButton() {A1, A2, A3, A4}
        ' Creating a pointer to the question box
        Q_box = New TextBox
        Q_box = Question_box
        ' Creating a pointer to the 'CorrectAnswer' box
        CA_box = New TextBlock
        CA_box = CorrectAns_box
        ' Creating a timer
        Timer = New DispatcherTimer
        AddHandler Timer.Tick, AddressOf Timer_Tick
        Timer.Interval = New TimeSpan(0, 0, 1)

        ' Restoring the window state after suspension
        If (CurrentQuestion > 0) Then
            ' Disabling the "complexity level" radio buttons
            Complexity1.IsEnabled = False
            Complexity2.IsEnabled = False
            Complexity3.IsEnabled = False
            ' Restoring the window state
            Update()
        End If

    End Sub

    Private Sub Timer_Tick(ByVal sender As Object, ByVal e As EventArgs)

        TimerValue = TimerValue - 1
        If (TimerValue = 0) Then
            ' If a "Pause" was scheduled and the user didn't choose anything
            If (TimeButtonState = False) Then
                ' Marking the question as 'wrong". Blocking the Radio Buttons. Scheduling the next question
                ShowQuestion(Complexity, CurrentQuestion)
                Answers(CurrentQuestion) = 8
                CheckAnswer(Complexity, CurrentQuestion)
                Dim i
                For i = 0 To 3
                    A_rad(i).IsEnabled = False
                Next
                CurrentQuestion = CurrentQuestion + 1
                ' Stopping the timer. Enabling the button
                Timer.Stop()
                Time_but.IsEnabled = True
                ' Cleaning the 'Notification_box'
                Notification_box.Text = ""
                ' If current question is beyond the last question - Navigate to the 'ResultsPage'
                If (CurrentQuestion = 10) Then
                    ' Navigate to the 'ResultsPage'
                    Me.Frame.Navigate(GetType(ResultsPage))
                End If
            Else
                TimerValue = 60
                ' Marking the question as 'wrong". Scheduling the next question
                Answers(CurrentQuestion) = 8
                CheckAnswer(Complexity, CurrentQuestion)
                CurrentQuestion = CurrentQuestion + 1
                ' If current question is the last question - Navigate to the 'ResultsPage'
                If (CurrentQuestion = 10) Then
                    ' Navigate to the 'ResultsPage'
                    Me.Frame.Navigate(GetType(ResultsPage))
                Else
                    ShowQuestion(Complexity, CurrentQuestion)
                End If
            End If
        End If
        ' Updating the current timer value on the display
        Time_disp.Text = TimerValue
    End Sub

    Private Sub Button_Click_1(sender As Object, e As RoutedEventArgs)

        ' If current question is beyond the last question - Navigate to the 'ResultsPage'
        If (CurrentQuestion = 10) Then
            ' Navigate to the 'ResultsPage'
            Me.Frame.Navigate(GetType(ResultsPage))
        Else
            If (TimeButtonState = False) Then   ' Start request
                ' Setting the current state to "Started"
                TimeButtonState = True
                ' Printing the current question and enabling the button linked to it
                ShowQuestion(Complexity, CurrentQuestion)
                'Disabling the Radio Buttons which define complexity
                Complexity1.IsEnabled = False
                Complexity2.IsEnabled = False
                Complexity3.IsEnabled = False
                ' Defining the initial timer value
                TimerValue = 60
                ' Switching the button functionality to allow pause between questions
                Time_but.Content = "Pause"
                ' Updating the value on the display
                Time_disp.Text = TimerValue
                ' Starting the timer
                Timer.Start()
            Else    ' Pause request
                ' Scheduling a "Pause" state
                TimeButtonState = False
                ' Switching the button functionality to allow resuming
                Time_but.Content = "Start"
                ' Pause state was scheduled. Disabling the button
                Time_but.IsEnabled = False
                Notification_box.Text = "Pause will be set after the current question"
            End If
        End If
    End Sub

    Private Sub Complexity1_Click(sender As Object, e As RoutedEventArgs) Handles Complexity1.Click
        Complexity = 1
    End Sub
    Private Sub Complexity2_Click(sender As Object, e As RoutedEventArgs) Handles Complexity2.Click
        Complexity = 2
    End Sub
    Private Sub Complexity3_Click(sender As Object, e As RoutedEventArgs) Handles Complexity3.Click
        Complexity = 3
    End Sub

    Private Sub Q1_Click(sender As Object, e As RoutedEventArgs) Handles Q1.Click
        ' Printing the answer
        CheckAnswer(Complexity, 0)
        'Printing the question
        ShowQuestion(Complexity, 0)
    End Sub

    Private Sub Q2_Click(sender As Object, e As RoutedEventArgs) Handles Q2.Click
        ' Printing the answer
        CheckAnswer(Complexity, 1)
        'Printing the question
        ShowQuestion(Complexity, 1)
    End Sub

    Private Sub Q3_Click(sender As Object, e As RoutedEventArgs) Handles Q3.Click
        ' Printing the answer
        CheckAnswer(Complexity, 2)
        'Printing the question
        ShowQuestion(Complexity, 2)
    End Sub

    Private Sub Q4_Click(sender As Object, e As RoutedEventArgs) Handles Q4.Click
        ' Printing the answer
        CheckAnswer(Complexity, 3)
        'Printing the question
        ShowQuestion(Complexity, 3)
    End Sub

    Private Sub Q5_Click(sender As Object, e As RoutedEventArgs) Handles Q5.Click
        ' Printing the answer
        CheckAnswer(Complexity, 4)
        'Printing the question
        ShowQuestion(Complexity, 4)
    End Sub

    Private Sub Q6_Click(sender As Object, e As RoutedEventArgs) Handles Q6.Click
        ' Printing the answer
        CheckAnswer(Complexity, 5)
        'Printing the question
        ShowQuestion(Complexity, 5)
    End Sub

    Private Sub Q7_Click(sender As Object, e As RoutedEventArgs) Handles Q7.Click
        ' Printing the answer
        CheckAnswer(Complexity, 6)
        'Printing the question
        ShowQuestion(Complexity, 6)
    End Sub

    Private Sub Q8_Click(sender As Object, e As RoutedEventArgs) Handles Q8.Click
        ' Printing the answer
        CheckAnswer(Complexity, 7)
        'Printing the question
        ShowQuestion(Complexity, 7)
    End Sub

    Private Sub Q9_Click(sender As Object, e As RoutedEventArgs) Handles Q9.Click
        ' Printing the answer
        CheckAnswer(Complexity, 8)
        'Printing the question
        ShowQuestion(Complexity, 8)
    End Sub

    Private Sub Q10_Click(sender As Object, e As RoutedEventArgs) Handles Q10.Click
        ' Printing the answer
        CheckAnswer(Complexity, 9)
        'Printing the question
        ShowQuestion(Complexity, 9)
    End Sub

    Private Sub Choice1_Click(sender As Object, e As RoutedEventArgs) Handles A1.Click

        ' Ordinary case, when the timer is running
        If (TimeButtonState = True) Then
            ' Recording the choice
            Answers(CurrentQuestion) = 0
            ' Checking the answer
            CheckAnswer(Complexity, CurrentQuestion)
            ' Reseting the timer
            TimerValue = 60
            Time_disp.Text = TimerValue
            ' Scheduling the next question
            CurrentQuestion = CurrentQuestion + 1
            ' If current question is beyond the last question - Navigate to the 'ResultsPage'
            If (CurrentQuestion = 10) Then
                ' Setting the 'TimeButtonState' to false. The timer state is not recorded 
                ' when navigating to another page or suspending the application
                TimeButtonState = False
                ' Navigate to the second page
                Me.Frame.Navigate(GetType(ResultsPage))
            Else
                ShowQuestion(Complexity, CurrentQuestion)
            End If
        Else ' Exception case when a pause was scheduled (TimeButtonState = False)
            ' Recording the choice
            Answers(CurrentQuestion) = 0
            ' Checking the answer
            CheckAnswer(Complexity, CurrentQuestion)
            ' Blocking the Radio Buttons
            Dim i
            For i = 0 To 3
                A_rad(i).IsEnabled = False
            Next
            ' Scheduling the next question
            CurrentQuestion = CurrentQuestion + 1
            ' Stopping the timer. Enabling the button
            Timer.Stop()
            Time_but.IsEnabled = True
            ' Nulling the TimerValue variable
            TimerValue = 0
            ' Updating the current timer value on the display
            Time_disp.Text = TimerValue
            ' Cleaning the 'Notification_box'
            Notification_box.Text = ""
            ' Reseting the timer
            TimerValue = 60
            Time_disp.Text = TimerValue
            ' If current question is beyond the last question - Navigate to the 'ResultsPage'
            If (CurrentQuestion = 10) Then
                ' Navigate to the second page
                Me.Frame.Navigate(GetType(ResultsPage))
            End If
        End If
    End Sub

    Private Sub Choice2_Click(sender As Object, e As RoutedEventArgs) Handles A2.Click
        ' Ordinary case, when the timer is running
        If (TimeButtonState = True) Then
            ' Recording the choice
            Answers(CurrentQuestion) = 1
            ' Checking the answer
            CheckAnswer(Complexity, CurrentQuestion)
            ' Reseting the timer
            TimerValue = 60
            Time_disp.Text = TimerValue
            ' Scheduling the next question
            CurrentQuestion = CurrentQuestion + 1
            ' If current question is beyond the last question - Navigate to the 'ResultsPage'
            If (CurrentQuestion = 10) Then
                ' Setting the 'TimeButtonState' to false. The timer state is not recorded 
                ' when navigating to another page or suspending the application
                TimeButtonState = False
                ' Navigate to the second page
                Me.Frame.Navigate(GetType(ResultsPage))
            Else
                ShowQuestion(Complexity, CurrentQuestion)
            End If
        Else ' Exception case when a pause was scheduled (TimeButtonState = False)
            ' Recording the choice
            Answers(CurrentQuestion) = 1
            ' Checking the answer
            CheckAnswer(Complexity, CurrentQuestion)
            ' Blocking the Radio Buttons
            Dim i
            For i = 0 To 3
                A_rad(i).IsEnabled = False
            Next
            ' Scheduling the next question
            CurrentQuestion = CurrentQuestion + 1
            ' Stopping the timer. Enabling the button
            Timer.Stop()
            Time_but.IsEnabled = True
            ' Nulling the TimerValue variable
            TimerValue = 0
            ' Updating the current timer value on the display
            Time_disp.Text = TimerValue
            ' Cleaning the 'Notification_box'
            Notification_box.Text = ""
            ' If current question is beyond the last question - Navigate to the 'ResultsPage'
            If (CurrentQuestion = 10) Then
                ' Navigate to the second page
                Me.Frame.Navigate(GetType(ResultsPage))
            End If
        End If
    End Sub

    Private Sub Choice3_Click(sender As Object, e As RoutedEventArgs) Handles A3.Click
        ' Ordinary case, when the timer is running
        If (TimeButtonState = True) Then
            ' Recording the choice
            Answers(CurrentQuestion) = 2
            ' Checking the answer
            CheckAnswer(Complexity, CurrentQuestion)
            ' Reseting the timer
            TimerValue = 60
            Time_disp.Text = TimerValue
            ' Scheduling the next question
            CurrentQuestion = CurrentQuestion + 1
            ' If current question is beyond the last question - Navigate to the 'ResultsPage'
            If (CurrentQuestion = 10) Then
                ' Setting the 'TimeButtonState' to false. The timer state is not recorded 
                ' when navigating to another page or suspending the application
                TimeButtonState = False
                ' Navigate to the second page
                Me.Frame.Navigate(GetType(ResultsPage))
            Else
                ShowQuestion(Complexity, CurrentQuestion)
            End If
        Else ' Exception case when a pause was scheduled (TimeButtonState = False)
            ' Recording the choice
            Answers(CurrentQuestion) = 2
            ' Checking the answer
            CheckAnswer(Complexity, CurrentQuestion)
            ' Blocking the Radio Buttons
            Dim i
            For i = 0 To 3
                A_rad(i).IsEnabled = False
            Next
            ' Scheduling the next question
            CurrentQuestion = CurrentQuestion + 1
            ' Stopping the timer. Enabling the button
            Timer.Stop()
            Time_but.IsEnabled = True
            ' Nulling the TimerValue variable
            TimerValue = 0
            ' Updating the current timer value on the display
            Time_disp.Text = TimerValue
            ' Cleaning the 'Notification_box'
            Notification_box.Text = ""
            ' If current question is beyond the last question - Navigate to the 'ResultsPage'
            If (CurrentQuestion = 10) Then
                ' Navigate to the second page
                Me.Frame.Navigate(GetType(ResultsPage))
            End If
        End If
    End Sub

    Private Sub Choice4_Click(sender As Object, e As RoutedEventArgs) Handles A4.Click
        ' Ordinary case, when the timer is running
        If (TimeButtonState = True) Then
            ' Recording the choice
            Answers(CurrentQuestion) = 3
            ' Checking the answer
            CheckAnswer(Complexity, CurrentQuestion)
            ' Reseting the timer
            TimerValue = 60
            Time_disp.Text = TimerValue
            ' Scheduling the next question
            CurrentQuestion = CurrentQuestion + 1
            ' If current question is beyond the last question - Navigate to the 'ResultsPage'
            If (CurrentQuestion = 10) Then
                ' Setting the 'TimeButtonState' to false. The timer state is not recorded 
                ' when navigating to another page or suspending the application
                TimeButtonState = False
                ' Navigate to the second page
                Me.Frame.Navigate(GetType(ResultsPage))
            Else
                ShowQuestion(Complexity, CurrentQuestion)
            End If
        Else ' Exception case when a pause was scheduled (TimeButtonState = False)
            ' Recording the choice
            Answers(CurrentQuestion) = 3
            ' Checking the answer
            CheckAnswer(Complexity, CurrentQuestion)
            ' Blocking the Radio Buttons
            Dim i
            For i = 0 To 3
                A_rad(i).IsEnabled = False
            Next
            ' Scheduling the next question
            CurrentQuestion = CurrentQuestion + 1
            ' Stopping the timer. Enabling the button
            Timer.Stop()
            Time_but.IsEnabled = True
            ' Nulling the TimerValue variable
            TimerValue = 0
            ' Updating the current timer value on the display
            Time_disp.Text = TimerValue
            ' Cleaning the 'Notification_box'
            Notification_box.Text = ""
            ' If current question is beyond the last question - Navigate to the 'ResultsPage'
            If (CurrentQuestion = 10) Then
                ' Navigate to the second page
                Me.Frame.Navigate(GetType(ResultsPage))
            End If
        End If
    End Sub

End Class
