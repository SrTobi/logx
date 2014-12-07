if (window.XMLHttpRequest) {
   xhttp = new XMLHttpRequest();
} else {    // IE 5/6
   xhttp = new ActiveXObject("Microsoft.XMLHTTP");
}



var LogLevelColorMapping = {
	"TRACE":	"#AAAAAA",
	"DEBUG":	"#BBBBBB",
	"DETAIL":	"#EEEEEE",
	"INFO":		"#55FF55",
	"WARN":		"#DDDD00",
	"ERR":		"#FF0000",
	"FATAL":	"#BB0000"
};

function GetLogLevelColor(lvl)
{
	var color = LogLevelColorMapping[lvl];
	if(color === null)
	{
		color = "#FFFFFF";
	}
	
	return color;
}


function LoadXmlLog(filename)
{
		// load xml file
	xhttp.overrideMimeType('text/xml');
	xhttp.open("GET", filename, false);
	xhttp.send(null);
	var xmlDoc = xhttp.responseXML; 

	/*var uurloon = xmlDoc.getElementsByTagName("uurloon")[0].childNodes[0].text;
	var setloon = xmlDoc.getElementsByTagName("setloon")[0].childNodes[0].text
	alert('end');*/
	return xmlDoc;
}

function FillContextDiv(context, xmllog, options)
{
	context.innerHTML = "";
	
	var tbl = 	document.createElement('table');
	tbl.style.width='100%';
	tbl.setAttribute('border','1');
	var rnode = xmllog.getElementsByTagName("log")[0];
	var entries = rnode.getElementsByTagName("entry");
	
	for(i = 0; i < entries.length; ++i)
	{
		var entry = entries[i];
		var row = tbl.insertRow();
		var cell = row.insertCell();
		
		var logmsg = entry.getElementsByTagName("msg")[0].childNodes[0].nodeValue;
		
		
		var tags = entry.getElementsByTagName("tag");
		for(j = 0; j < tags.length; ++j)
		{
			var tag = tags[j];
			
			if(tag.attributes.getNamedItem("name").nodeValue == "log_level")
			{
				var color = GetLogLevelColor(tag.childNodes[0].nodeValue);
				cell.style.backgroundColor = color;
			}
		}
		
		cell.appendChild(document.createTextNode(logmsg));
	}
	
	context.appendChild(tbl);
}