[Setup]
ChangesEnvironment=yes

[Tasks]
Name: "addtosystempath"; Description: "Add srcML to the system PATH for all users"; Flags: exclusive
Name: "addtouserpath"; Description: "Add srcML to the system PATH for the current user"; Flags: exclusive
Name: "dontaddtopath"; Description: "Do not add srcML to the system PATH"; Flags: exclusive

[Registry]
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}"; Check: IsTaskSelected('addtosystempath')
Root: HKLM; Subkey: "HKEY_CURRENT_USER\Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}"; Check: IsTaskSelected('addtouserpath')
