function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/ShaderNinja.exe", "@DesktopDir@/ShaderNinja.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/sn.ico",
            "iconId=0");
    }
}
