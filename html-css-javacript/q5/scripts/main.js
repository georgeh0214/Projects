// main.js

// 'use strict;' turns on JavaScripts strict mode, which helps catch errors

// $ is the main jQuery function used for calling jQuery functions

// document is a pre-defined JavaScript variable that refers to the entire web
// page

// ready is a jQuery function that is not called until the page's DOM is fully
// loaded and the page is ready --- you should always use it!

'use strict';

$(document).ready(function() {
	
	let clickfunction = function(event){
		let name = $('#name').val()
		let thing = $('#thing').val()
		$('#Name').text(name)
		$('#Thing').text(thing)

		let r = Math.floor(5*Math.random()+1)
		let person = ''
		if (r===1){
			person = 'Donald Duck';
		} else if (r===2){
			person='My girlfriend';
		} else if (r===3){
			person='Micky Mouse';
		} else if (r===4){
			person='Tom';
		} else if (r===5){
			person='Jerry';
		}
		$('#fperson').text(person);
		}
		$('#go').click(clickfunction)
	})
	   
		
$(document).ready(function() {
	
	let locationclickfunction = function(event){

		let t = Math.floor(5*Math.random()+1)
		let location = ''
		if (t===1){
			location = 'toilet';
		} else if (t===2){
			location='desk';
		} else if (t===3){
			location='head';
		} else if (t===4){
			location='finger';
		} else if (t===5){
			location='bed';
		}
		$('#loca').text(location);}
		$('#go').click(locationclickfunction)
	})



$(document).ready(function() {
	
	let whenclickfunction = function(event){

		let s = Math.floor(5*Math.random()+1)
		let time = ''
		if (s===1){
			time = 'everyday';
		} else if (s===2){
			time='every night';
		} else if (s===3){
			time="when I'm sad";
		} else if (s===4){
			time="when I'm hungry";
		} else if (s===5){
			time='the sun rises';
		}
		$('#ti').text(time);}
		$('#go').click(whenclickfunction)
	})




$(document).ready(function() {
	
	let pictureclickfunction = function(event){
		let g=Math.floor(3*Math.random()+1);
		let image = $('#funnypicture');
		let filename =""
		let altText=""
		if (g===1){
			filename = '1.jpg';
			altText="Mr.Bean"
		} else if (g===2){
			filename='2.jpg';
			altText="a squirrel"
		} else if (g===3){
			filename='3.jpg';
			altText="a nerd"
		}
		image.hide();
        image.attr('src', 'images/' + filename);
        image.attr('alt', altText);
        image.fadeIn('slow');
    }
    $('#go').click(pictureclickfunction)
	})

		


$(document).ready(function() {
	
	let stuffclickfunction = function(event){

		let n = Math.floor(5*Math.random()+1);
		let stuff = '';
		if (n===1){
			stuff = '$4.8 million Koenigsegg CCXR Trevita';
		} else if (n===2){
			stuff='Fidget Spinner';
		} else if (n===3){
			stuff='Toilet';
		} else if (n===4){
			stuff='plate';
		} else if (n===5){
			stuff='house';
		}
		$('#stuff').text(stuff);
		}
 $('#go').click(stuffclickfunction)
	})



