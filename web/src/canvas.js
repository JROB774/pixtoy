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

function get_lua_string(out_str, max_bytes) {
  var text = ace.edit("textedit").getValue();
  stringToUTF8(text, out_str, max_bytes);
}

function set_error_message(err, len) {
  var element = document.getElementById("error");
  if(len !== 0) {
    var errmsg = UTF8ToString(err,len);
    element.value = errmsg;
    element.style.display = "block";
  } else {
    element.style.display = "none";
  }
}

// Hijack tab handling so that the user can insert them in code: https://stackoverflow.com/a/18303822
document.getElementById("textedit").addEventListener("keydown", function(e) {
  if(e.keyCode === 9) {
    var start = this.selectionStart;
    var end = this.selectionEnd;
    var target = e.target;
    var value = target.value;
    target.value = value.substring(0, start) + "  " + value.substring(end);
    this.selectionStart = this.selectionEnd = start + 2; // +2 == length of inserted text.
    e.preventDefault();
  }
},false);
