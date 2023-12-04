var overOptionCss = "background: highlight; color: highlighttext";
var sizedBorderCss = "2 inset buttonhighlight";

var globalSelect;	//This is used when calling an unnamed selectbox with onclick="this.PROPERTY"

var ie4 = (document.all != null);

var q = 0;


function initSelectBox(el) {
	copySelected(el);
	
	var size = el.getAttribute("size");

// These two lines combined with execution in optionClick() allow you to write:
//		onchange="alert(this.options[this.selectedIndex].value)"
	el.options = el.children[1].children;
	el.selectedIndex = findSelected(el);	//Set the index now!

	dropdown = el.children[1];

	if (size != null) {
		if (size > 1) {
			el.size = size;
			dropdown.style.zIndex = 0;
			initSized(el);
//			window.setTimeout("initSized(" + el.id + ")", 1000);
		}
		else {
			el.size = 1;
			dropdown.style.zIndex = 99;
			if (dropdown.offsetHeight > 200) {
				dropdown.style.height = "200";
				dropdown.style.overflow = "auto";
			}
		}
	}
	
	highlightSelected(el,true);
}

function initSized(el) {
//alert("initSized -------->");
	var h = 0;
	el.children[0].style.display = "none";

	dropdown = el.children[1];
	dropdown.style.visibility = "visible";

	if (dropdown.children.length > el.size) {
		dropdown.style.overflow = "auto";
		for (var i=0; i<el.size; i++) {
			h += dropdown.children[i].offsetHeight;
		}

		if (dropdown.style.borderWidth != null) {
			dropdown.style.pixelHeight = h + 4; //2 * parseInt(dropdown.style.borderWidth);
		}

		else
			dropdown.style.height = h;

	}

	dropdown.style.border = sizedBorderCss;


	el.style.height = dropdown.style.pixelHeight;
}

function copySelected(el) {
	var selectedIndex = findSelected(el);

	selectedCell = el.children[0].rows[0].cells[0];
	selectedDiv  = 	el.children[1].children[selectedIndex];
	
	selectedCell.innerHTML = selectedDiv.outerHTML;
}

// This function returns the first selected option and resets the rest
// in case some idiot has set more than one to selcted :-)
function findSelected(el) {
	var selected = null;


	ec = el.children[1].children;	//the table is the first child
	var ecl = ec.length;
	
	for (var i=0; i<ecl; i++) {
		if (ec[i].getAttribute("selected") != null) {
			if (selected == null)	// Found first selected
				selected = i;
			else
				ec[i].removeAttribute("selected");	//Like I said. Only one selected!
		}
	}
	if (selected == null)
		selected = 0;	//When starting this is the most logic start value if none is present
		
	return selected;
}

function toggleDropDown(el) {
	if (el.size == 1) {
		dropDown = el.children[1];
		
		if (dropDown.style.visibility == "")
			dropDown.style.visibility = "hidden";
			
		if (dropDown.style.visibility == "hidden")
			showDropDown(dropDown);
		else
			hideDropDown(dropDown);
	}
}

function optionClick() {
	el = getReal(window.event.srcElement, "className", "option");

	if (el.className == "option") {
		dropdown  = el.parentElement;
		selectBox = dropdown.parentElement;
		
		oldSelected = dropdown.children[findSelected(selectBox)]

		if(oldSelected != el) {
			oldSelected.removeAttribute("selected");
			el.setAttribute("selected", 1);
			selectBox.selectedIndex = findSelected(selectBox);
		}

		if (selectBox.onchange != null) {	// This executes the onchange when you chnage the option
			if (selectBox.id != "") {		// For this to work you need to replace this with an ID or name
				eval(selectBox.onchange.replace(/this/g, selectBox.id));
			}
			else {
				globalSelect = selectBox;
				eval(selectBox.onchange.replace(/this/g, "globalSelect"));
			}
		}
		
		if (el.backupCss != null)
			el.style.cssText = el.backupCss;
		copySelected(selectBox);
		toggleDropDown(selectBox);
		highlightSelected(selectBox, true);
	}
}

function optionOver() {
	var toEl = getReal(window.event.toElement, "className", "option");
	var fromEl = getReal(window.event.fromElement, "className", "option");
	if (toEl == fromEl) return;
	var el = toEl;
	
	if (el.className == "option") {
		if (el.backupCss == null)
			el.backupCss = el.style.cssText;
		highlightSelected(el.parentElement.parentElement, false);
		el.style.cssText = el.backupCss + "; " + overOptionCss;
		this.highlighted = true;
	}
}

function optionOut() {
	var toEl = getReal(window.event.toElement, "className", "option");
	var fromEl = getReal(window.event.fromElement, "className", "option");

	if (fromEl == fromEl.parentElement.children[findSelected(fromEl.parentElement.parentElement)]) {
		if (toEl == null)
			return;
		if (toEl.className != "option")
			return;
	}
	
	if (toEl != null) {
		if (toEl.className != "option") {
			if (fromEl.className == "option")
				highlightSelected(fromEl.parentElement.parentElement, true);
		}
	}
	
	if (toEl == fromEl) return;
	var el = fromEl;

	if (el.className == "option") {
		if (el.backupCss != null)
			el.style.cssText = el.backupCss;
	}

}

function highlightSelected(el,add) {
	var selectedIndex = findSelected(el);
	
	selected = el.children[1].children[selectedIndex];
	
	if (add) {
		if (selected.backupCss == null)
			selected.backupCss = selected.style.cssText;
		selected.style.cssText = selected.backupCss + "; " + overOptionCss;
	}
	else if (!add) {
		if (selected.backupCss != null)
			selected.style.cssText = selected.backupCss;
	}
}

function hideShownDropDowns() {
	var el = getReal(window.event.srcElement, "className", "select");
	
	var spans = document.all.tags("SPAN");
	var selects = new Array();
	var index = 0;
	
	for (var i=0; i<spans.length; i++) {
		if ((spans[i].className == "select") && (spans[i] != el)) {
			dropdown = spans[i].children[1];
			if ((spans[i].size == 1) && (dropdown.style.visibility == "visible"))
				selects[index++] = dropdown;
		}
	}
	
	for (var j=0; j<selects.length; j++) {
		hideDropDown(selects[j]);
	}	

}

function hideDropDown(el) {
	if (typeof(fade) == "function")
		fade(el, false);
	else
		el.style.visibility = "hidden";
}

function showDropDown(el) {
	if (typeof(fade) == "function")
		fade(el, true);
	else if (typeof(swipe) == "function")
		swipe(el, 2);
	else
		el.style.visibility = "visible";
}

function initSelectBoxes() {
	var spans = document.all.tags("SPAN");
	var selects = new Array();
	var index = 0;
	
	for (var i=0; i<spans.length; i++) {
		if (spans[i].className == "select")
			selects[index++] = spans[i];
	}
	
	for (var j=0; j<selects.length; j++) {
		initSelectBox(selects[j]);
	}	
}

function getReal(el, type, value) {
	temp = el;
	while ((temp != null) && (temp.tagName != "BODY")) {
		if (eval("temp." + type) == value) {
			el = temp;
			return el;
		}
		temp = temp.parentElement;
	}
	return el;
}

if (ie4) {
	window.onload = initSelectBoxes;
	document.onclick = hideShownDropDowns;
}
