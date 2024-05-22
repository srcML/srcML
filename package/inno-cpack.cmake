# SPDX-License-Identifier: GPL-3.0-only
#
# @file inno.cpack
#
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
#
# CPack configuration for Inno Setup installers

# Exclude other platforms
if(NOT WIN32)
    return()
endif()

# Update the generator list
list(APPEND CPACK_GENERATOR "INNOSETUP")
list(REMOVE_DUPLICATES CPACK_GENERATOR)

# Package naming
set(CPACK_SYSTEM_NAME "windows-x86_64")

# Use modern wizard
set(CPACK_INNOSETUP_USE_MODERN_WIZARD ON)

# Enable welcome page
set(CPACK_INNOSETUP_SETUP_DisableWelcomePage OFF)

# The path to a custom installer .ico file. This is displayed in the file explorer
set(CPACK_INNOSETUP_ICON_FILE ${CMAKE_SOURCE_DIR}/package/srcml_icon.ico)

# A branding image that will be displayed inside the installer (used by GUI installers).
# This is displayed in the upper right-hand corner of each page
set(CPACK_PACKAGE_ICON ${CMAKE_SOURCE_DIR}/package/srcml_icon_inno.bmp)

# The welcome page display image
set(CPACK_INNOSETUP_SETUP_WizardImageFile ${CMAKE_SOURCE_DIR}/package/background_new.bmp)

# Make the client required
set(CPACK_COMPONENT_SRCML_REQUIRED TRUE)

# Setup will always show the selected directory in the list of settings on the Ready to Install wizard page
set(CPACK_INNOSETUP_SETUP_AlwaysShowDirOnReadyPage ON)

# Specify a particular icon file (either an executable or an .ico file) to display for the Uninstall entry in the
# Add/Remove Programs Control Panel applet
set(CPACK_INNOSETUP_SETUP_UninstallDisplayIcon ${CMAKE_SOURCE_DIR}/package/srcml_icon.ico)

# Extend functionality with custom script, inno_extension_script.iss
set(CPACK_INNOSETUP_EXTRA_SCRIPTS ${CMAKE_BINARY_DIR}/inno_extension_script.iss)

# Generate inno_extension_script.iss
file(WRITE ${CMAKE_BINARY_DIR}/inno_extension_script.iss
[=[
[Setup]
ChangesEnvironment=yes

[Tasks]
Name: "addtosystempath"; Description: "Add srcML to the system PATH for all users"; Flags: exclusive
Name: "addtouserpath"; Description: "Add srcML to the system PATH for the current user"; Flags: exclusive
Name: "dontaddtopath"; Description: "Do not add srcML to the system PATH"; Flags: exclusive

[Registry]
// Adding to system path
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}\bin"; Check: ShouldAddToSystemPath
// Adding to current user path
Root: HKCU; Subkey: "Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}\bin"; Check: ShouldAddToUserPath
// Adding CMake Registry Key
Root: HKLM; Subkey: "Software\Kitware\CMake\Packages\srcML"; ValueType: string; ValueName: "InstalledDir"; ValueData: ExpandConstant('{app}'); Check: WizardIsComponentSelected('DEVLIBS'); Flags: uninsdeletekey;

[Code]
function ShouldAddToSystemPath: Boolean;
var
  RegPath: String;
  RegValue: String;
  ResultStr: String;
begin
  RegPath := 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment';
  RegValue := 'Path';

  // Check if the value already exists in the registry
  Result := WizardIsTaskSelected('addtosystempath') and (RegQueryStringValue(HKLM, RegPath, RegValue, ResultStr) and (Pos(ExpandConstant('{app}\bin'), ResultStr) = 0));

end;

function ShouldAddToUserPath: Boolean;
var
  RegPath: String;
  RegValue: String;
  ResultStr: String;
begin
  RegPath := 'Environment';
  RegValue := 'Path';

  // Check if the value already exists in the registry
  Result := WizardIsTaskSelected('addtouserpath') and (RegQueryStringValue(HKCU, RegPath, RegValue, ResultStr) and (Pos(ExpandConstant('{app}\bin'), ResultStr) = 0));
end;

var
InfoLabel: TLabel;

procedure InitializeWizard;
begin

  // Select All Components For Install
  WizardSelectComponents('SRCML,SRCMLDEV\DEVLIBS,SRCMLDEV\EXAMPLES');

  InfoLabel := TLabel.Create(WizardForm);
  InfoLabel.Parent := WizardForm.FinishedPage;
  InfoLabel.Left := WizardForm.FinishedLabel.Left;
  InfoLabel.Top := WizardForm.FinishedLabel.Top + WizardForm.FinishedLabel.Height;
  InfoLabel.WordWrap := True;
end;

procedure CurPageChanged(CurPageID: Integer);
begin
  if CurPageID = wpFinished then
  begin
    InfoLabel.Caption :=
    'The srcml command-line program is at ' + ExpandConstant('{app}') + '\bin\srcml.exe'#13#10#13#10 +
    '# Convert from a source file to srcML'#13#10 +
    'srcml main.cpp -o main.cpp.xml'#13#10 +
    '# Convert from srcML back to source code'#13#10 +
    'srcml main.cpp.xml -o main.cpp'#13#10 +
    '# Convert a code fragment to srcML'#13#10 +
    'srcml --text="int a = b;" -l C++ -o fragment.cpp.xml'#13#10 +
    '# Convert entire project to srcML'#13#10 +
    'srcml --verbose project -o project.xml'#13#10#13#10;

    if WizardIsComponentSelected('DEVLIBS') then
    begin
      InfoLabel.Caption := InfoLabel.Caption +
      'The libsrcml development package installed:'#13#10#13#10 +
      '* The include file srcml.h in ' + ExpandConstant('{app}') + '\include'#13#10 +
      '* The static library libsrcml_static.lib in ' + ExpandConstant('{app}') + '\lib'#13#10 +
      '* The debug file libsrcml.pdb in ' + ExpandConstant('{app}') + '\bin'#13#10 +
      '* The libsrcml examples in ' + ExpandConstant('{app}') + '\share\srcml'#13#10 +
      '* CMake configuration files registered enabling the use of find_package(srcML REQUIRED)';
    end

    else
    begin
      InfoLabel.Caption := InfoLabel.Caption +
      'The libsrcml development package was not installed. To install, rerun the installer and select the libsrcml development package.';
    end;

  end;
end;
]=])
