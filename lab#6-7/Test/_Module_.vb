Module _Module_

    ' Controls
    Public Q_but() As Button                ' Question Buttons array
    Public A_rad() As RadioButton           ' Answer Radio Buttons array
    Public Timer As DispatcherTimer         ' Timer
    Public Q_box As TextBox                 ' Question box
    Public CA_box As TextBlock              ' Correct answer box
    ' Variables
    Public UserName As String
    Public Complexity As Integer
    Public CurrentQuestion As Integer
    Public TimeButtonState As Boolean
    Public TimerValue As Integer
    Public Answers() As Integer = New Integer() {9, 9, 9, 9, 9, 9, 9, 9, 9, 9}
    Public NrOfCorrectAnswers As Integer
    ' Functions
    ' This function is used to display the requested question and to prepare the window for it
    Public Function ShowQuestion(ByVal Complexity As Integer, ByVal Index As Integer)

        ' Blocking the Radio Buttons if the question requested is not current. Recovering
        ' the choice when navigating to old questions. Nulling the Radio Buttons for the current question
        Dim i
        If (CurrentQuestion = Index) Then
            For i = 0 To 3
                A_rad(i).IsEnabled = True
                ' Nulling the Radio Button checkings
                A_rad(i).IsChecked = False
            Next
            ' Cleaning the CorrectAnswer box
            CA_box.Text = ""
            ' Enabling the button linked to the current question
            Q_but(CurrentQuestion).IsEnabled = True
        Else    ' Old questions
            For i = 0 To 3
                A_rad(i).IsEnabled = False
                ' Nulling the Radio Button checkings
                A_rad(i).IsChecked = False
            Next
            ' Recovering the choice
            If (Answers(Index) < 4) Then
                A_rad(Answers(Index)).IsChecked = True
            End If
        End If

        ' Printing the question
        Select Case Index
            Case 0
                Select Case Complexity
                    Case 1
                        Q_box.Text = "What does 'FPS' mean?"
                        A_rad(0).Content = "3 letters"
                        A_rad(1).Content = "Nothing"
                        A_rad(2).Content = "Music player"
                        A_rad(3).Content = "Frames Per Second"
                    Case 2
                        Q_box.Text = "DirectX is a __"
                        A_rad(0).Content = "PC Game"
                        A_rad(1).Content = "Library"
                        A_rad(2).Content = "Class for VB"
                        A_rad(3).Content = "Library for C++"
                    Case 3
                        Q_box.Text = "Which library doesn't exist in DirectX?"
                        A_rad(0).Content = "ddraw.dll"
                        A_rad(1).Content = "dsound.dll"
                        A_rad(2).Content = "d3d.dll"
                        A_rad(3).Content = "dmusic.dll"
                End Select
            Case 1
                Select Case Complexity
                    Case 1
                        Q_box.Text = "'FTP is a __?"
                        A_rad(0).Content = "Server"
                        A_rad(1).Content = "Library"
                        A_rad(2).Content = "Same as FPS"
                        A_rad(3).Content = "ISP"
                    Case 2
                        Q_box.Text = "What is 'ActiveX'?"
                        A_rad(0).Content = "DirectX Component"
                        A_rad(1).Content = "Control Elements"
                        A_rad(2).Content = "Progr. lang."
                        A_rad(3).Content = "Video Card"
                    Case 3
                        Q_box.Text = "The advapi32.dll library is used for working with __"
                        A_rad(0).Content = "Graphics"
                        A_rad(1).Content = "DOS"
                        A_rad(2).Content = "Registry"
                        A_rad(3).Content = "Windows"
                End Select
            Case 2
                Select Case Complexity
                    Case 1
                        Q_box.Text = "How to measure the length of a string in C++?"
                        A_rad(0).Content = "strlen(Str);"
                        A_rad(1).Content = "len(Str);"
                        A_rad(2).Content = "strlen(Str)"
                        A_rad(3).Content = "len(Str)"
                    Case 2
                        Q_box.Text = "What mouse event doesn't exist?"
                        A_rad(0).Content = "MouseOver"
                        A_rad(1).Content = "MousePress"
                        A_rad(2).Content = "MouseMove"
                        A_rad(3).Content = "MouseUp"
                    Case 3
                        Q_box.Text = "Why there are 7 laboratory works?"
                        A_rad(0).Content = "-"
                        A_rad(1).Content = "-"
                        A_rad(2).Content = "-"
                        A_rad(3).Content = "-"
                End Select
            Case 3
                Select Case Complexity
                    Case 1
                        Q_box.Text = "Which of the following resolutions isn't standard?"
                        A_rad(0).Content = "800x600"
                        A_rad(1).Content = "640x480"
                        A_rad(2).Content = "1024x768"
                        A_rad(3).Content = "1024x748"
                    Case 2
                        Q_box.Text = "'PCI' and 'ISA' are __"
                        A_rad(0).Content = "Ports"
                        A_rad(1).Content = "Slots"
                        A_rad(2).Content = "Chipsets"
                        A_rad(3).Content = "Memory modules"
                    Case 3
                        Q_box.Text = "'SDI' and 'MDI' are __"
                        A_rad(0).Content = "Ports"
                        A_rad(1).Content = "Slots"
                        A_rad(2).Content = "Interfaces"
                        A_rad(3).Content = "Protocols"
                End Select
            Case 4
                Select Case Complexity
                    Case 1
                        Q_box.Text = "Which of the following is NOT a type of motherboard expansion slot?"
                        A_rad(0).Content = "ISA"
                        A_rad(1).Content = "PCI"
                        A_rad(2).Content = "AGP"
                        A_rad(3).Content = "ATX"
                    Case 2
                        Q_box.Text = "What does the '.ocx' extension mean?"
                        A_rad(0).Content = "Library"
                        A_rad(1).Content = "ActiveX"
                        A_rad(2).Content = "Driver"
                        A_rad(3).Content = "C++ file"
                    Case 3
                        Q_box.Text = "Name a product made by the Kinetix corporation"
                        A_rad(0).Content = "AutoCad"
                        A_rad(1).Content = "3DStudioMax"
                        A_rad(2).Content = "VisualStudio"
                        A_rad(3).Content = "AGP Modelling"
                End Select
            Case 5
                Select Case Complexity
                    Case 1
                        Q_box.Text = "In what units disk space is measured?"
                        A_rad(0).Content = "Miles"
                        A_rad(1).Content = "Bytes"
                        A_rad(2).Content = "cm"
                        A_rad(3).Content = "Amperes"
                    Case 2
                        Q_box.Text = "What is 'AGP'?"
                        A_rad(0).Content = "Port"
                        A_rad(1).Content = "Slot"
                        A_rad(2).Content = "Video Card"
                        A_rad(3).Content = "Extension"
                    Case 3
                        Q_box.Text = "What type of connector is used to plug a telephone line into a modem?"
                        A_rad(0).Content = "RJ-11"
                        A_rad(1).Content = "RJ-45"
                        A_rad(2).Content = "RJ-10"
                        A_rad(3).Content = "COM1"
                End Select
            Case 6
                Select Case Complexity
                    Case 1
                        Q_box.Text = "'USB' is __"
                        A_rad(0).Content = "a Slot"
                        A_rad(1).Content = "a Port"
                        A_rad(2).Content = "Nothing"
                        A_rad(3).Content = "Same as UPS"
                    Case 2
                        Q_box.Text = "Which of the following languages isn't Object Oriented?"
                        A_rad(0).Content = "Visual C++"
                        A_rad(1).Content = "Delphi"
                        A_rad(2).Content = "Pascal"
                        A_rad(3).Content = "Visual Basic"
                    Case 3
                        Q_box.Text = "What is another term for turning off SSID broadcast?"
                        A_rad(0).Content = "SSID Stealth"
                        A_rad(1).Content = "SSID Unicast"
                        A_rad(2).Content = "SSID Sec"
                        A_rad(3).Content = "SSID Cloaking"
                End Select
            Case 7
                Select Case Complexity
                    Case 1
                        Q_box.Text = "What does 'OS' mean?"
                        A_rad(0).Content = "Object Selector"
                        A_rad(1).Content = "Object Switcher"
                        A_rad(2).Content = "Nothing"
                        A_rad(3).Content = "Operating System"
                    Case 2
                        Q_box.Text = "Which of the following is not a type of computer hard drive?"
                        A_rad(0).Content = "IDE"
                        A_rad(1).Content = "EIDE"
                        A_rad(2).Content = "SCSI"
                        A_rad(3).Content = "FDD"
                    Case 3
                        Q_box.Text = "What is the maximum length of an SSID?"
                        A_rad(0).Content = "8"
                        A_rad(1).Content = "16"
                        A_rad(2).Content = "32"
                        A_rad(3).Content = "64"
                End Select
            Case 8
                Select Case Complexity
                    Case 1
                        Q_box.Text = "What is 'PSD'?"
                        A_rad(0).Content = "Extension"
                        A_rad(1).Content = "Port"
                        A_rad(2).Content = "Slot"
                        A_rad(3).Content = "Video Card"
                    Case 2
                        Q_box.Text = "Which of the following is not a type of RAM?"
                        A_rad(0).Content = "DIMM"
                        A_rad(1).Content = "SIMM"
                        A_rad(2).Content = "ROM"
                        A_rad(3).Content = "SLIPP"
                    Case 3
                        Q_box.Text = "Which of the following commands doesn't reboot the system?"
                        A_rad(0).Content = "init 6"
                        A_rad(1).Content = "reboot"
                        A_rad(2).Content = "telinit 0"
                        A_rad(3).Content = "shutdown -r now"
                End Select
            Case 9
                Select Case Complexity
                    Case 1
                        Q_box.Text = "What is Unix?"
                        A_rad(0).Content = "Corporation"
                        A_rad(1).Content = "Nothing"
                        A_rad(2).Content = "3D Modelling App"
                        A_rad(3).Content = "Operating System"
                    Case 2
                        Q_box.Text = "The best time to influence the quality of a system design is in the ___"
                        A_rad(0).Content = "Design Phase"
                        A_rad(1).Content = "Testing Phase"
                        A_rad(2).Content = "Analysis Phase"
                        A_rad(3).Content = "Planning Phase"
                    Case 3
                        Q_box.Text = "Which IRQ does COM1 commonly use?"
                        A_rad(0).Content = "3"
                        A_rad(1).Content = "4"
                        A_rad(2).Content = "6"
                        A_rad(3).Content = "7"
                End Select
        End Select

        ' Return statement
        ShowQuestion = 0
    End Function

    ' This function is used to check the answers and provide output in means of color and text
    Public Function CheckAnswer(ByVal Complexity As Integer, ByVal Index As Integer)

        Dim i
        Select Case Complexity
            Case 1
                Dim Easy As Integer() = New Integer() {3, 0, 0, 3, 3, 1, 1, 3, 0, 3}
                If (Answers(Index) = Easy(Index)) Then
                    ' Printing "Correct!" in the 'CorrectAnswer' box
                    CA_box.Text = "Correct!"
                    ' If index is the current question 
                    If (Index = CurrentQuestion) Then
                        NrOfCorrectAnswers = NrOfCorrectAnswers + 1
                        ' Coloring the question button that is linked to the current question in green
                        Dim GreenBrush As New SolidColorBrush
                        GreenBrush.Color = Windows.UI.Colors.DarkGreen
                        Q_but(Index).BorderBrush = GreenBrush
                    End If
                ElseIf (Answers(Index) < 9) Then    ' If the Answer is incorrect
                    ' Printing the CorrectAnswer
                    CA_box.Text = "Correct Answer: " & Easy(Index) + 1
                    ' If index is the current question 
                    If (Index = CurrentQuestion) Then
                        ' Coloring the question button that is linked to the current question in red
                        Dim RedBrush As New SolidColorBrush
                        RedBrush.Color = Windows.UI.Colors.Red
                        Q_but(Index).BorderBrush = RedBrush
                    End If
                End If
            Case 2
                Dim Medium As Integer() = New Integer() {1, 1, 1, 1, 1, 1, 2, 3, 2, 3}
                If (Answers(Index) = Medium(Index)) Then
                    ' Printing "Correct!" in the 'CorrectAnswer' box
                    CA_box.Text = "Correct!"
                    ' If index is the current question 
                    If (Index = CurrentQuestion) Then
                        NrOfCorrectAnswers = NrOfCorrectAnswers + 1
                        ' Coloring the question button that is linked to the current question in green
                        Dim GreenBrush As New SolidColorBrush
                        GreenBrush.Color = Windows.UI.Colors.DarkGreen
                        Q_but(Index).BorderBrush = GreenBrush
                    End If
                ElseIf (Answers(Index) < 9) Then     ' If the Answer is incorrect
                    ' Printing the CorrectAnswer
                    CA_box.Text = "Correct Answer: " & Medium(Index) + 1
                    ' If index is the current question 
                    If (Index = CurrentQuestion) Then
                        ' Coloring the question button that is linked to the current question in red
                        Dim RedBrush As New SolidColorBrush
                        RedBrush.Color = Windows.UI.Colors.Red
                        Q_but(Index).BorderBrush = RedBrush
                    End If
                End If
            Case 3
                Dim Advanced As Integer() = New Integer() {2, 2, 2, 2, 1, 0, 3, 2, 2, 1}
                If (Answers(Index) = Advanced(Index)) Then
                    ' Printing "Correct!" in the 'CorrectAnswer' box
                    CA_box.Text = "Correct!"
                    ' If index is the current question 
                    If (Index = CurrentQuestion) Then
                        NrOfCorrectAnswers = NrOfCorrectAnswers + 1
                        ' Coloring the question button that is linked to the current question in green
                        Dim GreenBrush As New SolidColorBrush
                        GreenBrush.Color = Windows.UI.Colors.DarkGreen
                        Q_but(Index).BorderBrush = GreenBrush
                    End If
                ElseIf (Answers(Index) < 9) Then    ' If the Answer is incorrect
                    ' Printing the CorrectAnswer
                    CA_box.Text = "Correct Answer: " & Advanced(Index) + 1
                    ' If index is the current question 
                    If (Index = CurrentQuestion) Then
                        ' Coloring the question button that is linked to the current question in red
                        Dim RedBrush As New SolidColorBrush
                        RedBrush.Color = Windows.UI.Colors.Red
                        Q_but(Index).BorderBrush = RedBrush
                    End If
                End If
        End Select

        'Return statement
        CheckAnswer = 0
    End Function

    ' This function is used to restore the state of the window after suspension
    Public Function Update()

        Dim i
        Dim ActualCurrentQuestion As Integer = CurrentQuestion
        Dim ActualNrOfCorrectAnswers As Integer = NrOfCorrectAnswers
        For i = 0 To ActualCurrentQuestion - 1
            ' CurrentQuestion variable will be incremented in increasing order so as to simulate actual progress
            CurrentQuestion = i
            ' Enabling the buttons linked to the questions
            Q_but(i).IsEnabled = True
            ' Restoring state of the answers
            CheckAnswer(Complexity, CurrentQuestion)
        Next
        ' Restoring the actual value in the 'CurrentQuestion' variable
        CurrentQuestion = ActualCurrentQuestion
        ' Restoring the actual value in the 'NrOfCorrectAnswers' variable
        NrOfCorrectAnswers = ActualNrOfCorrectAnswers
        ' Displaying the current question
        ShowQuestion(Complexity, CurrentQuestion - 1)

        'Return statement
        Update = 0
    End Function
End Module
