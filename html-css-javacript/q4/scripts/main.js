// main.js

// 'use strict;' turns on JavaScripts strict mode, which helps catch errors

// $ is the main jQuery function used for calling jQuery functions

// document is a pre-defined JavaScript variable that refers to the entire web
// page

// ready is a jQuery function that is not called until the page's DOM is fully
// loaded and the page is ready --- you should always use it!

'use strict';

$(document).ready(function() {
	let clickfunction = function(evenr){
		let n = Math.floor(5*Math.random()+1)
		let result=''
		if (n===1){
			result = 'Yes';
		} else if (n===2){
			result = 'No';
		} else if (n===3){
			result = 'Absolutely!';
		} else if (n===4){
			result = 'Definitely not!';
		} else if (n===5){
			result = 'Maybe';
		}
		$('#result').hide();
		$('#result').attr(result);
		$('#result').fadeIn('slow');
		$('#result').text(result)
	}
	$('#go').click(clickfunction)

    // always call your code through ready to ensure the DOM is fully loaded

});
