function geid(id){
	return document.getElementById(id);
}

geid("save").onclick=function(){
	ip=geid("ip").value;
	subnet=geid("subnet").value;
	gateway=geid("gateway").value;
	ssidname=geid("ssidname").value;
	ssidPassword=geid("password").value;
	host=geid("host").value;
	port=geid("port").value;
	clientid=geid("clientid").value;
	apikey=geid("apikey").value;

	data = "i"+ip
			+"\ns="+subnet
			+"\ng="+gateway
			+"\nd="+ssidname
			+"\np="+ssidPassword
			+"\nh="+host
			+"\no="+port
			+"\nc="+clientid
			+"\na="+apikey;

	ajax=new XMLHttpRequest();
	ajax.open("POST", "/save");
	ajax.onreadystatechange=function(){
		if (ajax.readyState == 4 && ajax.status == 200){
			geid("save").innerHTML="Save settings";
		}
	}
	ajax.send(data);
	geid("save").innerHTML="Saving";
}

geid("restart").onclick=function(){
	ajax=new XMLHttpRequest();
	ajax.open("GET", "/restart");
	ajax.onreadystatechange=function(){
		if (ajax.readyState == 4 && ajax.status == 200){
			setTimeout(function(){
				geid("restart").innerHTML="Restart";			
				geid("restart").disabled=false;
			}, 20000)
		}
	}
	ajax.send();
	geid("restart").innerHTML="Restarting";
	geid("restart").disabled=true;
}