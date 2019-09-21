// main.js

// 'use strict;' turns on JavaScripts strict mode, which helps catch errors

// all p5.js programs should have a setup() function and draw() function

'use strict';

function setup() {
	// create the p5.js canvas and attach it to the HTML element whose id is
	// p5canvas
    createCanvas(500, 500).parent('p5canvas');
}

function draw() {
	// draw an ellipse
	// with center=(250, 250)
	// width=200, height=80
	background(200);
	fill(255,235,205);
    ellipse(mouseX, mouseY, 150, 200);
    triangle(mouseX,mouseY,mouseX-5,mouseY+5,mouseX+5,mouseY+5);
    arc(mouseX-75,mouseY-10,15,30,-3.14*1.5,-2.8/2)
    arc(mouseX+75,mouseY-10,15,30,3.1*1.5,3/2)


    fill(255);
    rect(mouseX-60,mouseY-40,40,20);

    rect(mouseX+15,mouseY-40,40,20);

    fill(0);
    ellipse(mouseX-40,mouseY-30,10,15);
    ellipse(mouseX+35,mouseY-30,10,15);
    line(mouseX-25,mouseY+40,mouseX+25,mouseY+40)
    line(mouseX-20,mouseY-30,mouseX+15,mouseY-30)
    line(mouseX-60,mouseY-30,mouseX-70,mouseY-30)
    line(mouseX+55,mouseY-30,mouseX+70,mouseY-30)
 
    arc(mouseX,mouseY-50,150,120,-3.14,0)

	// draw a rectangle
	// with upper left corner=(250, 250)
	// width=200, height=80
   
}
