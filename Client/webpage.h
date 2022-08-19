//=====================
//HTML code for webpage
//=====================
const char index_html[] PROGMEM = 
R"=====(
<!DOCTYPE HTML>
<html>
<title>ESP32 WebSocket Server</title>
<!---------------------------CSS-------------------------->
<style>
    #dynRectangle
    {
        width:0px;
        height:12px;
        top: 9px;
        background-color: red;
        z-index: -1;
        margin-top:8px;
    }
    body   {background-color:rgba(128,128,128,0.322); font-family:calibri}
    h1     {font-size: 40px; color: black; text-align: center}
    h2     {font-size: 30px; color: blue}
    h3     {font-size: 17px; color:blue}
    div.h1 {background-color: whitesmoke;}
</style>
<!--------------------------HTML-------------------------->
<body>
    <h1><div class="h1">ESP32 WebSocket Server</div></h1>
        <h2>
        Time: <span style="color:rgb(216, 3, 3)" id="Timevalue">0</span> V  
    </h2>
    <h2>
        POT voltage: <span style="color:rgb(216, 3, 3)" id="POTvalue">0</span> V  
    </h2>
    <h3>
        0V &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
        &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; 3.5V
        <div id="dynRectangle"></div>
    </h3>
</body>
<!----------------------JavaScript------------------------>
<script>
  InitWebSocket()
  function InitWebSocket()
  {
    websock = new WebSocket('ws://'+window.location.hostname+':81/');
    websock.onmessage=function(evt)
    {
       JSONobj = JSON.parse(evt.data);
       document.getElementById('Timevalue').innerHTML = JSONobj.T;
       document.getElementById('POTvalue').innerHTML = JSONobj.L;
       var pot = parseInt(JSONobj.POT * 135);
       document.getElementById("dynRectangle").style.width = pot+"px";
    }
  }
</script>
</html>
)=====";
