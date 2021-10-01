function Component() {
    installer.finishButtonClicked.connect(this, Component.prototype.installationFinished);
}

Component.prototype.createOperations = function() {
    component.createOperations();
    console.log("sufuk");
    	var desktopFileTarget = "@HomeDir@/.local/share/applications";
    	console.log(desktopFileTarget);
    	component.addOperation("CreateDesktopEntry", 
        	                          "MRADSIM.desktop", 
        	                          "Type=Application\nTerminal=false\nExec= " + "@TargetDir@/mradsim-gui.sh\nName=MRADSIM\nIcon= " + "@TargetDir@/mradsim.svg");
    	component.addOperation("CreateDesktopEntry", 
        	                          "@HomeDir@/.config/autostart/MRADSIM.desktop", 
        	                          "Type=Application\nTerminal=false\nExec= " + "@TargetDir@/mradsim-gui.sh\nName=MRADSIM\nIcon= " + "@TargetDir@/mradsim.svg");
}

Component.prototype.installationFinished = function() {
	console.log("sufuk");
    	var desktopFileTarget = "@HomeDir@/.local/share/applications";
    	console.log(desktopFileTarget);
    	component.addOperation("CreateDesktopEntry", 
        	                          "MRADSIM.desktop", 
        	                          "Type=Application\nTerminal=false\nExec= " + "@TargetDir@/mradsim-gui.sh\nName=MRADSIM\nIcon= " + "@TargetDir@/mradsim.svg");
    	component.addOperation("CreateDesktopEntry", 
        	                          "@HomeDir@/.config/autostart/MRADSIM.desktop", 
        	                          "Type=Application\nTerminal=false\nExec= " + "@TargetDir@/mradsim-gui.sh\nName=MRADSIM\nIcon= " + "@TargetDir@/mradsim.svg");
}

