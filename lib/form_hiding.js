var browserType;

if (document.layers) {browserType = "nn4"}
if (document.all) {browserType = "ie"}
if (window.navigator.userAgent.toLowerCase().match("gecko")) {browserType= "gecko"}

function c_hide(el) {
  if (browserType == "gecko" )
     x = eval('document.getElementById(el)');
  else if (browserType == "ie")
     x = eval('document.all[el]');
  else
     x = eval('document.layers[el]');

  x.style.visibility = "hidden";
}

function c_show(el) {
  if (browserType == "gecko" )
     x = eval('document.getElementById(el)');
  else if (browserType == "ie")
     x = eval('document.all[el]');
  else
     x = eval('document.layers[el]');
  x.style.visibility = "visible";
}

function c_disable(el)
{
var x=document.getElementById(el)
x.disabled=true
}
function c_enable()
{
var x=document.getElementById(el)
x.disabled=false
}