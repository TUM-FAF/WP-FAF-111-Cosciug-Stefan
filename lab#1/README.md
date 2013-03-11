# Windows Programming
###### student gr. FAF-111: Cosciug Stefan

# Topic: WIN32 API programming basics
## Intro
### Purpose
* Event-driven programming (message-oriented)
* API
* Windows applications development

### Additional Purposes
* Version Control Systems (GIT)
* Programming Style Guidelines
* Debugging
* Testing

## Completed Mandatory Objectives
* Create a Windows application
* Choose Programming Style Guidelines that you'll follow
* Add 2 buttons to window: one with default styles, one with custom styles (size, background, text color, font family, font size)
* Add 2 text inputs to window: one with default styles, one with custom styles (size, background, text color, font family, font size)
* Add 2 text elements to window: one with default styles, one with custom styles (size, background, text color, font family, font size)

## Completed Objectives With Points
* Make elements to fit window on resize **(1 pt)**
* Make elements to interact or change other elements (1 pt for 2 different interactions) **(2 pt)**
* Change behavior of different window actions (at least 3) **(1pt)**

## The application creation steps

Being the first laboratory work, the process of creating this software was more oriented toward my learning and understanding of basic parameters, functions and object interactions.

### Basically the software provides the following functionality:
* 1. Left Button (Activate) - Interacts with the Right Button "Enabling it", disables itself, changes the TEXT "Status:" to "Enabled", moves the window one pixel to the Left/Right.
* 2. Right Button (Deactivate) - Interacts with the Left Button "Enabling it", disables itself, changes the TEXT "Status:" to "Disabled", moves the window one pixel to the Left/Right.
* 3. Right Button (Enter) - {Appears only when user clicks on 'console edit box' to input text.} Executes the command written in 'console edit box', disables itself, enables the Left Button, may output a message, removes focus from the 'console edit box', moves the window one pixel to the Left/Right.
* 4. Edit Box (Console: ) - Allows to enter the following commands: { [-quit, -create, -destroy], where:	
-quit (Exits the program); -create (Creates the second Edit Box) -destroy (Destroys the created Edit Box) }, moves the carret beyond the last character.							
* 5. On window resize, all objects change with respect to Width/Height ratio.
* 6. Default window system commands (maximize, minimize, Title bar double click) are changed to provide message output.
    


### The PSG I used was the "C code style", with some slight adjustments: 		
* Curly brackets are placed symmetric.													
* Some comments are in line.


![Result](https://raw.github.com/TUM-FAF/WP-FAF-111-Cosciug-Stefan/master/lab%231/img1.png)
![Result](https://raw.github.com/TUM-FAF/WP-FAF-111-Cosciug-Stefan/master/lab%231/img2.png)

## Outro
From my point of view, this laboratory work was a good introduction to Win32 programming.