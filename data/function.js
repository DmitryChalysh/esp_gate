var xmlHttp=createXmlHttpObject();
function createXmlHttpObject(){
 if(window.XMLHttpRequest){
  xmlHttp=new XMLHttpRequest();
 }else{
  xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
 }
 return xmlHttp;
}
function load(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
  xmlHttp.open('GET','/configs.json',true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
   jsonResponse=JSON.parse(xmlHttp.responseText);
   loadBlock();
  }
 }
}

function loadBlock(data2) {
 data2 = JSON.parse(xmlHttp.responseText);
 data = document.getElementsByTagName('body')[0].innerHTML;
 var new_string;
for (var key in data2) {
 new_string = data.replace(new RegExp('{{'+key+'}}', 'g'), data2[key]);
 data = new_string;
}
 document.getElementsByTagName('body')[0].innerHTML = new_string;
 handleServerResponse();
}

function val(id){
 var v = document.getElementById(id).value;
 return v;
}
function send_request(submit,server){
 request = new XMLHttpRequest();
 request.open("GET", server, true);
 request.send();
 save_status(submit,request);
}
function save_status(submit,request){
 old_submit = submit.value;
 request.onreadystatechange = function() {
  if (request.readyState != 4) return;
  submit.value = request.responseText;
  setTimeout(function(){
   submit.value=old_submit;
   submit_disabled(false);
  }, 1000);
 }
 submit.value = 'Подождите...';
 submit_disabled(true);
}
function submit_disabled(request){
 var inputs = document.getElementsByTagName("input");
 for (var i = 0; i < inputs.length; i++) {
  if (inputs[i].type === 'submit') {inputs[i].disabled = request;}
 }
}
function toggle(target) {
 var curVal = document.getElementById(target).className;
 document.getElementById(target).className = (curVal === 'hidden') ? 'show' : 'hidden';
}
function set_ssid(submit) {
      server = "/ssid?ssid=" + val('ssid') + "&password=" + encodeURIComponent(val('password'));
      send_request(submit, server);
      alert("Измененя вступят в силу после перезагрузки. Пожалуйта перезагрузите устройство.");
    }
    function set_ssid_ap(submit) {
      server = "/ssidap?ssidAP=" + val('ssidap') + "&passwordAP=" + encodeURIComponent(val('passwordAp'));
      send_request(submit, server);
      alert("Измененя вступят в силу после перезагрузки. Пожалуйта перезагрузите устройство.");
    }
    function set_ssdp(submit) {
      server = "/ssdp?ssdp=" + val('ssdp');
      send_request(submit, server);
      document.getElementById('ssdp_t').innerHTML = val('ssdp');
    }
	function set_min_hum(submit) {
      server = "/settins?min_hum=" + val('min_Hum');
      send_request(submit, server);
      alert("Измененя вступят в силу после перезагрузки. Пожалуйта перезагрузите устройство.");
    }
	function set_min_light(submit) {
      server = "/settins?min_light=" + val('min_Light');
      send_request(submit, server);
      alert("Измененя вступят в силу после перезагрузки. Пожалуйта перезагрузите устройство.");
    }
	 function set_time_watering(submit) {
      server = "/settins?time_watering=" + val('timeWatering');
      send_request(submit, server);
      alert("Измененя вступят в силу после перезагрузки. Пожалуйта перезагрузите устройство.");
    }
	 function set_time_hum(submit) {
      server = "/settins?time_hum=" + val('timeHum');
      send_request(submit, server);
      alert("Измененя вступят в силу после перезагрузки. Пожалуйта перезагрузите устройство.");
    }
	 function set_time_light(submit) {
      server = "/settins?time_light=" + val('timeLight');
      send_request(submit, server);
      alert("Измененя вступят в силу после перезагрузки. Пожалуйта перезагрузите устройство.");
    }
    function restart(submit, texts) {
      if (confirm(texts)) {
        server = "/restart?device=ok";
        send_request(submit, server);
        return true;
      } else {
        return false;
      }
    }
