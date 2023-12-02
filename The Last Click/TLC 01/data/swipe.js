///////////////////////////////////////////////////////////////////////
//     This swipe library was designed by Erik Arvidsson for WebFX   //
//                                                                   //
//     For more info and examples see: http://www.eae.net/webfx/     //
//     or send mail to erik@eae.net                                  //
//                                                                   //
//     Feel free to use this code as lomg as this disclaimer is      //
//     intact.                                                       //
///////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
// The following lines makes the animation of the menus.
//////////////////////////////////////////////////////////////////////////////////////////

var swipeSteps = 4;
var swipemsec = 25;

var swipeArray = new Array();	// Needed to keep track of wich elements are animating

//////////////////////////////////////////////////////////////////////////////////////////
// the dir parameter is the direction read from the NumPad on your keyboard

function swipe(el, dir, steps, msec) {

	if (steps == null) steps = swipeSteps;
	if (msec == null) msec = swipemsec;

	if (el.swipeIndex == null)
		el.swipeIndex = swipeArray.length;
	if (el.swipeTimer != null)
		window.clearTimeout(el.swipeTimer);
		
	swipeArray[el.swipeIndex] = el;

	el.style.clip = "rect(-99999, 99999, 99999, -99999)";

	if (el.swipeCounter == null) {		// No animation yet!
		el.orgLeft  = el.offsetLeft;
		el.orgTop  = el.offsetTop;
		el.orgWidth = el.offsetWidth;
		el.orgHeight  = el.offsetHeight;
	}
	else if (el.swipeCounter == 0) {	// The Animation has stopped! It's now safe to update the position.
		el.orgLeft  = el.offsetLeft;
		el.orgTop  = el.offsetTop;
		el.orgWidth = el.offsetWidth;
		el.orgHeight  = el.offsetHeight;
	}
	
	el.style.left = el.orgLeft;
	el.style.top  = el.orgTop;
	
	el.swipeCounter = steps;
	el.style.clip = "rect(0,0,0,0)";
			
	window.setTimeout("repeat(" + dir + "," + el.swipeIndex + "," + steps + "," + msec + ")", msec);
}


function repeat(dir, index, steps, msec) {
	el = swipeArray[index];
	var left   = el.orgLeft;
	var top    = el.orgTop;
	var width  = el.orgWidth;
	var height = el.orgHeight;
	
	if (el.swipeCounter == 0) {
		el.style.clip = "rect(-99999, 99999, 99999, -99999)";
		return;
	}
	else {
		el.swipeCounter--;
		el.style.visibility = "visible";
		switch (dir) {
			case 2:		//down (see the numpad)
				el.style.clip = "rect(" + height*el.swipeCounter/steps + "," + width + "," + height + "," + 0 + ")";
				el.style.top  = top - height*el.swipeCounter/steps;
				break;
			case 8:
				el.style.clip = "rect(" + 0 + "," + width + "," + height*(steps-el.swipeCounter)/steps + "," + 0 + ")";
				el.style.top  = top + height*el.swipeCounter/steps;
				break;
			case 6:
				el.style.clip = "rect(" + 0 + "," + width + "," + height + "," + width*(el.swipeCounter)/steps + ")";
				el.style.left  = left - width*el.swipeCounter/steps;
				break;
			case 4:
				el.style.clip = "rect(" + 0 + "," + width*(swipeSteps - el.swipeCounter)/steps + "," + height + "," + 0 + ")";
				el.style.left  = left + width*el.swipeCounter/steps;
				break;
		}
		
		el.swipeTimer = window.setTimeout("repeat(" + dir + "," + index + "," + steps + "," + msec + ")", msec);
	}
}

function hideSwipe(el) {
	window.clearTimeout(el.swipeTimer);
	el.style.visibility = "hidden";
	el.style.clip = "rect(-99999, 99999, 99999, -99999)";
	el.swipeCounter = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
