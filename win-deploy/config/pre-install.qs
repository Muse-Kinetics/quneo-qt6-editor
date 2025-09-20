function preInstall() {
    console.log("running pre-install");
    var targetDir = installer.value("TargetDir");


    if (installer.fileExists(targetDir)) {
        installer.messageBox(installerResources + "The directory you selected already exists and contains an installation. " +
                             "Please uninstall the existing installation first or choose a different directory.",
                             "Error", 0x10 /*QMessageBox::Critical*/, 0x04 /*QMessageBox::Ok*/);
        installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
        installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
        installer.setDefaultPageVisible(QInstaller.Introduction, false);
        installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
        installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
        installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
        installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
        gui.rejectWithoutPrompt();
    }
}
