function writeSelectBox(matrix, id, size, onchange, css) {
	var d = window.document;

	var ie4 = (document.all != null);

	if (ie4) {
//		alert("Before!");
		var s = createIEString(matrix, id, size, onchange, css);
		document.write(s);
//		alert("After!");
//		alert(s);
	}

	else {
		document.write(createXString(matrix, id, size, onchange, css));
	}
}

function createIEString(matrix, id, size, onchange, css) {
	var str = "";
	// Span startTag	
		str += '<span class="select"';
		if (size == null)
			size = 1;
		str += ' size="' + size + '"';	
		if (id != null)
			str += ' id="' + id + '"';
		if (onchange != null)
			str += ' onchange="' + onchange + '"';
		if (css != null)
			str += ' style="' + css + '"';
		str += '>\n';
	
	// Table Tag
		str += '<table class="selectTable" cellspacing="0" cellpadding="0"\n';
		str += ' onclick="toggleDropDown(this.parentElement)">\n';
		str += '<tr>\n';
		str += '<td class="selected">&nbsp;</td>\n';
		str += '<td align="CENTER" valign="MIDDLE" class="Button"\n';
		str += ' onmousedown="this.style.border=\'2 inset buttonhighlight\'"\n';
		str += ' onmouseup="this.style.border=\'2 outset buttonhighlight\'">\n';
		str += '<span style="position: relative; left: 0; top: -2; width: 100%;">6</span></td>\n';
		str += '</tr>\n';
		str += '</table>\n';
		
	// DropDown startTag
		str += '<div class="dropDown" onclick="optionClick()" onmouseover="optionOver()" onmouseout="optionOut()">\n';
		
		for (var i=0; i<matrix.length; i++) {
			html     = matrix[i].html;
			value    = matrix[i].value;
			css      = matrix[i].css;
			selected = matrix[i].selected;
			
		// Write option starttag
			str += '<div class="option"';
			if (value != null)
				str += ' value="' + value + '"';
			if (css != null)
				str += ' style="' + css + '"';
			if (selected != null)
				str += ' selected';
			str += '>\n';
			
		// Write HTML contents
			str += html;
		// Write end tag
			str += '</div>\n';
		}
	
	//DropDown endtag
		str += '</div>\n';
		
	// Span endTag
		str += '</span>\n';
		
	return str;
}

function createXString(matrix, id, size, onchange, css) {
//	var str = "\n";
	// form startTag
	var str = '<form>\n';
	// Select startTag
	str += '<select';
	if (size == null)
		size = 1;
	str += ' size="' + size + '"';	
	if (id != null)
		str += ' id="' + id + '"';
	if (onchange != null)
		str += ' onchange="' + onchange + '"';
//	if (css != null)
//		str += ' style="' + css + '"';
	str += '>\n';
	// write options
	for (var i=0; i<matrix.length; i++) {
		html     = matrix[i].html;
		value    = matrix[i].value;
		css      = matrix[i].css;
		selected = matrix[i].selected;
		
	// Write option starttag
		str += '\n<option';
		if (value != null)
			str += ' value="' + value + '"';
//		if (css != null)
//			str += ' style="' + css + '"';
		if (selected != null)
			str += ' selected';
		str += '>';
		
	// Write HTML contents
		str += stripTags(html);
	// Write end tag
		str += '</option>\n';
	}
	str += '\n</select>\n';
	str += '</form>\n';

	return str;
}

function stripTags(str) {
	var s = 0;
	var e = -1;
	var r = "";

	s = str.indexOf("<",e);	

	do {
		r += str.substring(e + 1,s);
		e = str.indexOf(">",s);
		s = str.indexOf("<",e);
	}
	while ((s != -1) && (e != -1))

	r += str.substring(e + 1,str.length);

	return r;
}

function Option(html, value, css, selected) {
	this.html = html;
	this.value = value;
	this.css = css;
	this.selected = selected;
}