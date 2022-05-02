window.onload = function () { window.focus(); }
window.onclick = function () { window.focus(); }

var Module = {
  print: (function() {
    return function(text) {
      if(arguments.length > 1)
        text = Array.prototype.slice.call(arguments).join(" ");
      console.log(text);
    };
  })(),
  printErr: (function(text) {
    return function(text) {
      if(arguments.length > 1)
        text = Array.prototype.slice.call(arguments).join(" ");
      console.error(text);
    };
  })(),
  canvas: (function() {
    var canvas = document.getElementById("canvas");
    return canvas;
  })()
};

function display_error(err, len) {
  var errmsg = UTF8ToString(err,len);
  alert(errmsg);
}

function get_lua_string(out_str, max_bytes) {
  var text = ace.edit("editor").getValue();
  stringToUTF8(text, out_str, max_bytes);
}

function set_error_message(err, len) {
  var element = document.getElementById("error");
  if(len !== 0) {
    var errmsg = UTF8ToString(err,len);
    element.textContent = errmsg;
    element.style.display = "flex";
  } else {
    element.style.display = "none";
  }
}
