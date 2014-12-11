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

function FillFileList(e, filelist)
{
	for(i = filelist.length - 1; i >= 0; --i)
	{
		var f = filelist[i];
		var optElem = document.createElement("option");
		optElem.innerHTML = f;
		e.appendChild(optElem);
	}
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


function Attr_LogLevel(row, value, options)
{
	var color = GetLogLevelColor(value);
	row.style.backgroundColor = color;
	var cell = row.insertCell();
	cell.innerHTML = value;
	cell.className = value + "_TAGC tagc";
}

function Attr_Cat(row, value, options)
{
	var cell = row.insertCell();
	cell.innerHTML = value;
	cell.className = "hascat";
}

function ApplyTagFunctorList(list, msg, row, options)
{
	list.forEach(function (paf)
	{
		var tagVal = null;
		var tags = msg.getElementsByTagName("tag");
		for(j = 0; j < tags.length; ++j)
		{
			var tag = tags[j];
			
			if(tag.attributes.getNamedItem("name").nodeValue == paf[0])
			{
				found = true;
				tagVal = tag.childNodes[0].nodeValue;
				break;
			}
		}
		
		paf[1](row, tagVal, options);
	});
}

function FillContextDiv(context, xmllog, options)
{
	context.innerHTML = "";
	
	var tbl = 	document.createElement('table');
	tbl.setAttribute('class','log_table');
	var rnode = xmllog.getElementsByTagName("log")[0];
	var entries = rnode.getElementsByTagName("entry");
	
	
	var pre_attribute_functors = [
		["log_level", Attr_LogLevel],
		["cat", Attr_Cat]
	];
	
	for(i = 0; i < entries.length; ++i)
	{
		var entry = entries[i];
		var row = tbl.insertRow();
		
		
		ApplyTagFunctorList(pre_attribute_functors, entry, row, options);
		
		var cell = row.insertCell();
		var logmsg = entry.getElementsByTagName("msg")[0].childNodes[0].nodeValue;
		cell.className = "msgcol";
		
		//ApplyTagFunctorList
		
		cell.appendChild(document.createTextNode(logmsg));
	}
	
	context.appendChild(tbl);
}