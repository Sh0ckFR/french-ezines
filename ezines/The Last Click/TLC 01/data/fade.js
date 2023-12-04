///////////////////////////////////////////////////////////////////////
//     This fade library was designed by Erik Arvidsson for WebFX    //
//                                                                   //
//     For more info and examples see: http://www.eae.net/webfx/     //
//     or send mail to erik@eae.net                                  //
//                                                                   //
//     Feel free to use this code as lomg as this disclaimer is      //
//     intact.                                                       //
///////////////////////////////////////////////////////////////////////

window.status = "Loading fade package...";

var fadeSteps = 4;				// Number of steps to loop
var fademsec = 25;				// The time between each step (note that most computer have problem
								// handling to small values due to hardware limitations)


var fadeArray = new Array();	// Needed to keep track of wich elements are animating

//////////////////  fade  ////////////////////////////////////////////////////////////
//                                                                                  //
//   parameter: fadeIn                                                              //
// description: A boolean value. If true the element fades in, otherwise fades out  //
//              The steps and msec are optional. If not provided the default        //
//              values are used                                                     //
//                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////

function fade(el, fadeIn, steps, msec) {

	if (steps == null) steps = fadeSteps;
	if (msec == null) msec = fademsec;
	
	if (el.fadeIndex == null)
		el.fadeIndex = fadeArray.length;
	fadeArray[el.fadeIndex] = el;
	
	if (el.fadeStepNumber == null) {
		if (el.style.visibility == "hidden")
			el.fadeStepNumber = 0;
		else
			el.fadeStepNumber = steps;
		if (fadeIn)
			el.style.filter = "Alpha(Opacity=0)";
		else
			el.style.filter = "Alpha(Opacity=100)";
	}
			
	window.setTimeout("repeatFade(" + fadeIn + "," + el.fadeIndex + "," + steps + "," + msec + ")", msec);
}

//////////////////////////////////////////////////////////////////////////////////////
//  Used to iterate the fading

function repeatFade(fadeIn, index, steps, msec) {	
	el = fadeArray[index];
	
	c = el.fadeStepNumber;
	if (el.fadeTimer != null)
		window.clearTimeout(el.fadeTimer);
	if ((c == 0) && (!fadeIn)) {			//Done fading out!
		el.style.visibility = "hidden";		// If the platform doesn't support filter it will hide anyway
//		el.style.filter = "";
		return;
	}
	else if ((c==steps) && (fadeIn)) {	//Done fading in!
		el.style.filter = "";
		el.style.visibility = "visible";
		return;
	}
	else {
		(fadeIn) ? 	c++ : c--;
		el.style.visibility = "visible";
		el.style.filter = "Alpha(Opacity=" + 100*c/steps + ")";

		el.fadeStepNumber = c;
		el.fadeTimer = window.setTimeout("repeatFade(" + fadeIn + "," + index + "," + steps + "," + msec + ")", msec);
	}
}

window.status = "";