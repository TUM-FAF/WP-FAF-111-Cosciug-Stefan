# Windows Programming
###### student gr. FAF-111: Cosciug Stefan

# Topic: WinRT API
## Intro
### Prerequisites
* IDEs: Visual Studio 2012
* Languages: VB.Net
* OS: Windows 8

## Completed Mandatory Objectives
* Make an intelligence test application with at least 3 levels of complexity. Each level should have at least 10 questions. It may have a timer.

## Completed Objectives With Points
* Your application should have at least 3 screens/windows (ex: main,settings,login...) (3 pt) 
* Intuitive design (2 pt) 
* Follow WinRT applications life cycle (2 pt) 

## Description
This application is a multiple choice questionnaire providing computer-related questions. It has 3 pages (MainPage, TestPage, ResultsPage), where:
* "MainPage" - Provides the ability to input the user name. After navigating away from this page it won't be possible to return to it during the current session.
* "TestPage" - The test page. After navigating to the "ResultsPage", it's possible to return back to review the answered questions.
* "ResultsPage" - A page that will present the outcome, that's (UserName, Difficulty, Nr of correct answers, Percentage)

The test page features 3 levels of complexity and a set of 10 questions for each level. Output is provided in terms of text and colour. 
After starting the timer it will run continuously having allocated 60 seconds / question, unless a pause is requested.
Pause is set only after having completed the current question. There are 2 possible outcomes:
* "The user requested a pause and made a choice" - The choice will be recorded and the timer will halt.
* "The user requested a pause and didn't choose anything" - The answer to the current question will be marked as "wrong" and the timer will halt.

The test page also features an array of 10 buttons linked to 10 questions. They become available as questions are being answered and navigation between them is allowed at any time.


This application follows the WinRT life cycle, which allows the application to suspend. Suspending the application while the timer is active will mark the answer to the current question as "wrong".
 



![Result](https://raw.github.com/TUM-FAF/WP-FAF-111-Cosciug-Stefan/master/lab%236-7/SplashScreen.png)
![Result](https://raw.github.com/TUM-FAF/WP-FAF-111-Cosciug-Stefan/master/lab%236-7/MainWindow.png)
![Result](https://raw.github.com/TUM-FAF/WP-FAF-111-Cosciug-Stefan/master/lab%236-7/TestWindow.png)
![Result](https://raw.github.com/TUM-FAF/WP-FAF-111-Cosciug-Stefan/master/lab%236-7/ResultsWindow.png)


