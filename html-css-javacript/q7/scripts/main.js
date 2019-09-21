// main.js

// 'use strict;' turns on JavaScripts strict mode, which helps catch errors

// all p5.js programs should have a setup() function and draw() function

'use strict';

function setup() {
    createCanvas(500, 500).parent('p5canvas');
    background(200);
}


function keyPressed() {

     if (key === 'r' || key === 'R'){
        // choose a random fill color
        stroke(255,0,0);
    } else if (key === 'b' || key === 'B'){
    	stroke(0,0,255);
    } else if (key === 'g' || key === 'G'){
    	stroke(0,255,0);
    } 
}


function draw() {

    
    if (mouseIsPressed) {
        cursor(ARROW);
        line(mouseX, mouseY, pmouseX, pmouseY);
    } else {
        cursor(CROSS);
    }
}