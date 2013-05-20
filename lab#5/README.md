# Windows Programming
###### student gr. FAF-111: Cosciug Stefan

# Topic: Collaboration. Complex application
## Intro
### Purpose
* Collaboration
* All previous studied things

## Completed Mandatory Objectives
* Create an application which has GDI animation
* Animation should be controlled by controls

## Completed Objectives With Points
* Work on this project in a team of 2-4 (2 pt) 
* Divide tasks and describe them in readme (for each task indicate who is responsible for it) (1 pt) 
* Make it useful(you may ask me to evaluate usefulness of your application idea) (0-3 pt) 
* Following clear and nice code style (1 pt)
* Working on laboratory work using GIT and using as remote repository provided github repo (1 pt)


## Application Creation steps

As template for this laboratory work, the laboratory work #4 was used.
We updated the class and also changed its name from “Ring” to “Ball”.

In order to compute motion dynamics between balls, we had to introduce their impulse as a parameter. It’s computed using the ball velocity and speed, and vice versa. 
Impulse is introduced as sum of two vectors (horizontal and vertical). After interaction, the impulse is distributed among balls based on their size and direction. After distributing impulse, the angle is recalculated for each ball.
Balls also change their colour on each interaction.

A score board was created. “Goal” is counted when ball completely quits the window frame.
Then we added two gates for rejecting balls, in code we called them “docks”.
One gate is user guided by using the “up/down” arrow keys and the “space bar” for their acceleration in the range of 100px from the gate’s front side.
The other gate is computer guided. It follows the following rules:
* Focus on the closest ball if there is at least a distance of 10px between the gate and the ball.
* After rejecting the ball on which focus was set previously, move to the next one.
The speed of the computer guided dock is self-adjusted according to the speed of the incoming ball.

And in the end we introduced motion dynamics between the ball and gates shape.
Due to their non-symmetric shape and difference in size and speed for each ball, sometimes it goes beyond the margin curve where calculations are done, and that causes errors. See “Known Issues for more information”

### Known Issues:

* If a ball is coming at a high speed, the computer guided gate will also increase speed so as to intercept the ball, which may cause overlapping between the gate and the balls that get on its way.
* There may be some points on the gate where balls are not properly rejected.
* If the game is running in slow motion, and you pass a high impulse to a ball near your gate by holding down the “space bar”, the speed of the ball will increase greatly making it to skip over many pixels which will cause unexpected behaviour.


## Tasks Distribution
### Cosciug Stefan
* Dynamics
* Score board
* Computer gate self-guidance

### Mardari Vasile
* Created a class for gates, their control and shape




![Result](https://raw.github.com/TUM-FAF/WP-FAF-111-Cosciug-Stefan/master/lab%235/img1.png)


