var editor = ace.edit("editor");
ace.config.set("basePath", ".");

// Configure the Ace editor with our desired settings.
editor         .setOption("navigateWithinSoftTabs", false);
editor         .setOption("useSoftTabs",            true);
editor.renderer.setOption("theme",                  "ace/theme/cobalt");
editor.renderer.setOption("showPrintMargin",        false);
editor.renderer.setOption("displayIndentGuides",    false);
editor.session .setOption("mode",                   "ace/mode/lua");
editor.session .setOption("tabSize",                2);
