// main.js

// 'use strict;' turns on JavaScripts strict mode, which helps catch errors

// $ is the main jQuery function used for calling jQuery functions

// document is a pre-defined JavaScript variable that refers to the entire web
// page

// ready is a jQuery function that is not called until the page's DOM is fully
// loaded and the page is ready --- you should always use it!

'use strict';

$(document).ready(function() {
	let Text = $('#sample');
	

		let UpperCheckbox = $('#upper');
	let upperclickfunction = function(event){
		if (UpperCheckbox.prop('checked')){
			Text.css('text-transform','uppercase');
		}else{
			Text.css('text-transform','none');
		}
	}
	UpperCheckbox.click(upperclickfunction);



		let BorderCheckbox = $('#border');
	let borderclickfunction = function(event){
		if (BorderCheckbox.prop('checked')){
			Text.css('border','2px solid red');
		}else{
			Text.css('border','none');
		}
	}
	BorderCheckbox.click(borderclickfunction);


	let radioButtons = $('input[name="font"]')

	let fontClickFunction = function(event){
		let fontName = $('input[name="font"]:checked').val()
		Text.css('font-family', fontName)
	}
	radioButtons.click(fontClickFunction)
    // always call your code through ready to ensure the DOM is fully loaded

});
