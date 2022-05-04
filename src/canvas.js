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
