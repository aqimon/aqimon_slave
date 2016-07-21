function geid(id){
	return document.getElementById(id);
}

function changeIPInputState(b){
	if (b)
		geid("static-ip").style.display="initial";
	else
		geid("static-ip").style.display="none";
}

geid("static").onclick=function(){
	changeIPInputState(geid("static").checked);	
}

geid("save").onclick=function(){
	geid("save").disabled=true;
	ip=geid("ip").value;
	subnet=geid("subnet").value;
	gateway=geid("gateway").value;
	ssidname=geid("ssid").value;
	ssidPassword=geid("password").value;
	host=geid("host").value;
	port=geid("port").value;
	clientid=geid("clientid").value;
	apikey=geid("apikey").value;
	if (geid("static").checked)
		static=1;
	else
		static=0;

	data = "\nbegin\ni="+ip
			+"\ns="+subnet
			+"\ng="+gateway
			+"\nd="+ssidname
			+"\np="+ssidPassword
			+"\nh="+host
			+"\no="+port
			+"\nc="+clientid
			+"\na="+apikey
			+"\nx="+static
			+"\nend\n";
			console.log(data);
	ajax=new XMLHttpRequest();
	ajax.open("POST", "/save");
	ajax.onreadystatechange=function(){
		if (ajax.readyState == 4 && ajax.status == 200){
			geid("save").innerHTML="Save settings";
			geid("save").disabled=false;
		}
	}
	ajax.send(data);
	geid("save").innerHTML="Saving";
}
function loadData(){
	ajax=new XMLHttpRequest();
	ajax.responseType="json";
	ajax.open("GET", "/currSetting");
	ajax.onreadystatechange=function(){
	  	if (ajax.readyState == 4 && ajax.status==200){
		    data=ajax.response;
		    geid("ip").value=data.ip;
		    geid("subnet").value=data.subnet;
		    geid("gateway").value=data.gateway;
		    geid("ssid").value=data.ssid;
		    geid("password").value=data.password;
		    geid("host").value=data.host;
		    geid("port").value=data.port;
		    geid("clientid").value=data.clientID;
		    geid("apikey").value=data.apiKey;
		    if (data.static==1){
		    	geid("static").checked=true;
		    } else {
		    	geid("static").checked=false;
		    }
		    changeIPInputState(geid("static").checked);
		  }
	}
	ajax.send();
}
setTimeout(loadData, 50);