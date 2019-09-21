// main.js

// 'use strict;' turns on JavaScripts strict mode, which helps catch errors

// all p5.js programs should have a setup() function and draw() function

'use strict';

let x, y;    // position of the ball
let dx, dy;  // velocity of the ball
let x1,y1;
let dx1,dy1;
let x2, y2;
let dx2,dy2;
let Gameover =false





function setup() {


    createCanvas(800, 600).parent('p5canvas');

    // set the initial value of the ball's position and velocity
    x = random(75,700);
    y = random(75,500);
    dx = random(5,15);  // try setting these
    dy = random(5,15);   // randomly
    x1 = random(75,700);
    y1 = random(75,500);
    dx1 = random(5,15);  // try setting these
    dy1 = random(5,15);
    x2 = random(75,700);
    y2 = random(75,500);
    dx2 = random(5,15);  // try setting these
    dy2 = random(5,15);

}













function draw() {
    background(220);
	noStroke();
    
    if (Gameover===true){
    	textSize(100);
    	text('Gameover',200,300)
    	return;
    }

    fill(255, 255, 0); 
    ellipse(x, y, 100, 100);
    x+=dx;
    y+=dy;

    if (x < 50) {
        dx = -dx;
    }

    // gone off right?
    if (x >= 750) {
        dx = -dx;
    }

    // gone off top?
    if (y < 50) {
        dy = -dy;
    }

    if (y >= 550) {
        dy = -dy;
    }



    fill(0,255,255);
    ellipse(x1,y1,100,100);

    x1+=dx1;
    y1+=dy1;

    if (x1 < 50) {
        dx1 = -dx1;
    }

    // gone off right?
    if (x1 >= 750) {
        dx1 = -dx1;
    }

    // gone off top?
    if (y1 < 50) {
        dy1 = -dy1;
    }

    if (y1 >= 550) {
        dy1 = -dy1;
    }



    fill(255,0,255);
    ellipse(x2,y2,100,100)

    x2+=dx2;
    y2+=dy2;

    if (x2 < 50) {
        dx2 = -dx2;
    }

    // gone off right?
    if (x2 >= 750) {
        dx2 = -dx2;
    }

    // gone off top?
    if (y2 < 50) {
        dy2 = -dy2;
    }

    if (y2 >= 550) {
        dy2 = -dy2;
    }

fill(0);
textSize(30);
textFont('Georgia');
text('score '+ Math.floor(millis()/1000),50,50)



if ((dist(mouseX,mouseY,x,y)<=50)|| (dist(mouseX,mouseY,x1,y1)<=50)|| (dist(mouseX,mouseY,x2,y2)<=50)){
	dx=0
	dy=0
	dx1=0
	dy1=0
	dx2=0
	dy2=0
	Gameover = true
}




}



