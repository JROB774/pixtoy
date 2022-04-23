window.onload = function () { window.focus(); }
window.onclick = function () { window.focus(); }

var Module = {
  print: (function() {
    var element = document.getElementById('output');
    if(element) element.value = ''; // Clear browser cache.
    return function(text) {
      if(arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
      console.log(text);
      if(element) {
        element.value += text + "\n";
        element.scrollTop = element.scrollHeight; // Focus on bottom.
      }
    };
  })(),
  printErr: function(text) {
  },
  canvas: (function() {
    var canvas = document.getElementById('canvas');
    return canvas;
  })()
};

function get_lua_string(out_str, max_bytes) {
  var element = document.getElementById('textedit');
  stringToUTF8(element.value, out_str, max_bytes);
}

// Hijack tab handling so that the user can insert them in code: https://stackoverflow.com/a/18303822
document.getElementById("textedit").addEventListener('keydown', function(e) {
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
