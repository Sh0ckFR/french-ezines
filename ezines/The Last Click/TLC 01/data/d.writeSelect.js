function writeSelectBox(matrix, id, size, onchange, css) {
	var ie4 = (document.all != null);

	if (ie4) {
		createIEString(matrix, id, size, onchange, css);
	}

	else {
		createXString(matrix, id, size, onchange, css);
	}
}

function createIEString(matrix, id, size, onchange, css) {
		var d = window.document;
	// Span startTag	
		d.write('<span class="select"');
		if (size == null)
			size = 1;
		d.write(' size="' + size + '"');	
		if (id != null)
			d.write(' id="' + id + '"');
		if (onchange != null)
			d.write(' onchange="' + onchange + '"');
		if (css != null)
			d.write(' style="' + css + '"');
		d.write('>');
	
	// Table Tag
		d.write('<table class="selectTable" cellspacing="0" cellpadding="0"\
				 onclick="toggleDropDown(this.parentElement)">\
				<tr>\
					<td class="selected">&nbsp;</td>\
					<td align="CENTER" valign="MIDDLE" class="Button"\
						onmousedown="this.style.border=\'2 inset buttonhighlight\'"\
						onmouseup="this.style.border=\'2 outset buttonhighlight\'">\
						<span style="position: relative; left: 0; top: -2; width: 100%;">6</span></td>\
				</tr>\
		</table>');
		
	// DropDown startTag
		d.write('<div class="dropDown" onclick="optionClick()" onmouseover="optionOver()" onmouseout="optionOut()">');
		
		for (var i=0; i<matrix.length; i++) {
			html     = matrix[i].html;
			value    = matrix[i].value;
			css      = matrix[i].css;
			selected = matrix[i].selected;
			
		// Write option starttag
			d.write('<div class="option"');
			if (value != null)
				d.write(' value="' + value + '"');
			if (css != null)
				d.write(' style="' + css + '"');
			if (selected != null)
				d.write(' selected');
			d.write('>');
			
		// Write HTML contents
			d.write(html);
		// Write end tag
			d.write('</div>');
		}
	
	//DropDown endtag
		d.write('</div>');
		
	// Span endTag
		d.write('</span>');
}

function createXString(matrix, id, size, onchange, css) {
	var d = window.document;
	// form startTag
	d.write('<form>\n');
	// Select startTag
	d.write('<select');
	if (size == null)
		size = 1;
	d.write(' size="' + size + '"');	
	if (id != null)
		d.write(' id="' + id + '"');
	if (onchange != null)
		d.write(' onchange="' + onchange + '"');
	if (css != null)
		d.write(' style="' + css + '"');
	d.write('>\n');
	// write options
	for (var i=0; i<matrix.length; i++) {
		html     = matrix[i].html;
		value    = matrix[i].value;
		css      = matrix[i].css;
		selected = matrix[i].selected;
		
	// Write option starttag
		d.write('<option');
		if (value != null)
			d.write(' value="' + value + '"');
		if (css != null)
			d.write(' style="' + css + '"');
		if (selected != null)
			d.write(' selected');
		d.write('>');
		
	// Write HTML contents
		d.write(html);
	// Write end tag
		d.write('</option>\n');
	}
	d.write('</select>\n');
	d.write('</form>\n');
}
function Option(html, value, css, selected) {
	this.html = html;
	this.value = value;
	this.css = css;
	this.selected = selected;
}