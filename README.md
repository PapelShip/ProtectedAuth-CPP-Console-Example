# qPapel built-in protection auth service

<div align="left"><figure><img src="https://1823333039-files.gitbook.io/~/files/v0/b/gitbook-x-prod.appspot.com/o/spaces%2FQDoe25r2JbXimDtXxNVy%2Fuploads%2FxRTv1jF1HHtzPxWJcOf3%2Fpapelship.png?alt=media&#x26;token=5e3aabd8-c683-414a-bd98-3ae90ec3de8f" alt="" width="309"><figcaption></figcaption></figure></div>
# Installation

Complete guide to integrate qPapel Auth library into your Visual Studio C++ project.

{% hint style="info" %}
This library requires Windows 10/11 (x64) and Visual Studio 2019 or later.
{% endhint %}

## Requirements

| Requirement      | Version                        |
| ---------------- | ------------------------------ |
| Operating System | Windows 10/11 (x64)            |
| IDE              | Visual Studio 2019 or later    |
| C++ Standard     | C++17 or later                 |
| Network          | Internet connectivity required |

## Download Library Files

You will receive the following files:

{% tabs %}
{% tab title="Header File" %}
**qPapel.h**

* Main library header
* Contains all function declarations
* Place in your Include directory
  {% endtab %}

{% tab title="Library File" %}
**qPapel-ProtectedAuth.lib**

* Compiled static library
* Links with your project
* Place in your Lib directory

**libsodium.lib**

* Compiled static library
* Place in your Lib directory
  {% endtab %}
  {% endtabs %}

## Project Setup

{% stepper %}
{% step %}

### Create Project Structure

Create the following folder structure in your project:

```
YourProject/
├── Include/
│   └── qPapel.h
├── Lib/
│   ├── qPapel-ProtectedAuth.lib
│   ├── libsodium.lib
└── Source/
    └── main.cpp
```

{% endstep %}

{% step %}

### Configure Include Directory

{% tabs %}
{% tab title="Visual Studio GUI" %}

1. Right-click your project in **Solution Explorer**
2. Select **Properties**
3. Navigate to **Configuration Properties → C/C++ → General**
4. Click **Additional Include Directories**
5. Add your Include folder path:

   ```
   $(ProjectDir)Include
   ```

   Or absolute path:

   ```
   C:\MyProject\Include
   ```
6. Click **OK**
   {% endtab %}

{% tab title="Project File (.vcxproj)" %}
Add to your `.vcxproj` file:

```xml
<ItemDefinitionGroup>
  <ClCompile>
    <AdditionalIncludeDirectories>$(ProjectDir)Include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
  </ClCompile>
</ItemDefinitionGroup>
```

{% endtab %}
{% endtabs %}
{% endstep %}

{% step %}

### Configure Library Directory

{% tabs %}
{% tab title="Visual Studio GUI" %}

1. In **Properties**, navigate to **Configuration Properties → Linker → General**
2. Click **Additional Library Directories**
3. Add your Lib folder path:

   ```
   $(ProjectDir)Lib
   ```

   Or absolute path:

   ```
   C:\MyProject\Lib
   ```
4. Click **OK**
   {% endtab %}

{% tab title="Project File (.vcxproj)" %}
Add to your `.vcxproj` file:

```xml
<ItemDefinitionGroup>
  <Link>
    <AdditionalLibraryDirectories>$(ProjectDir)Lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
  </Link>
</ItemDefinitionGroup>
```

{% endtab %}
{% endtabs %}
{% endstep %}

{% step %}

### Link the Library

{% tabs %}
{% tab title="Visual Studio GUI" %}

1. In **Properties**, navigate to **Configuration Properties → Linker → Input**
2. Click **Additional Dependencies**
3. Add:

   ```
   qPapel-ProtectedAuth.lib
   libsodium.lib
   ```
4. Click **OK**
   {% endtab %}

{% tab title="Project File (.vcxproj)" %}
Add to your `.vcxproj` file:

```xml
<ItemDefinitionGroup>
  <Link>
    <AdditionalDependencies>qPapel-ProtectedAuth.lib;%(AdditionalDependencies)</AdditionalDependencies>
  </Link>
</ItemDefinitionGroup>
```

{% endtab %}

{% tab title="Code (Pragma)" %}
Add to your source file:

```cpp
#pragma comment(lib, "qPapel-ProtectedAuth.lib")
```

{% endtab %}
{% endtabs %}
{% endstep %}

{% step %}

### Set C++ Standard

{% hint style="warning" %}
The library requires C++17 or later.
{% endhint %}

1. In **Properties**, navigate to **Configuration Properties → C/C++ → Language**
2. Set **C++ Language Standard** to **ISO C++17 Standard (/std:c++17)** or later
3. Click **OK**
   {% endstep %}
   {% endstepper %}

## Verify Installation

Create a test file to verify the setup:

```cpp
#include <iostream>
#include "qPapel.h"

int main() {
    qPapel::ProtectedAuth auth;
    std::cout << "qPapel Auth library loaded successfully!" << std::endl;
    return 0;
}
```

{% tabs %}
{% tab title="Build" %}
Press **F7** or click **Build → Build Solution**

Expected output:

```
Build succeeded.
```

{% endtab %}

{% tab title="Run" %}
Press **Ctrl+F5** or click **Debug → Start Without Debugging**

Expected output:

```
qPapel Auth library loaded successfully!
```

{% endtab %}
{% endtabs %}

{% hint style="success" %}
If you see the success message, installation is complete!
{% endhint %}

## Troubleshooting

<details>

<summary>Error: Cannot open include file 'qPapel.h'</summary>

**Cause:** Include directory not configured correctly

**Solutions:**

1. Verify `qPapel.h` exists in Include folder
2. Check Include path in project properties
3. Use absolute path if relative path doesn't work
4. Ensure path has no typos

**Verify:**

```cpp
// This should work after fixing
#include "qPapel.h"
```

</details>

<details>

<summary>Error: Unresolved external symbol</summary>

**Cause:** Library not linked properly

**Solutions:**

1. Verify `qPapel-ProtectedAuth.lib` exists in Lib folder
2. Check Lib path in project properties
3. Ensure library is added to Additional Dependencies
4. Check platform is x64 (not x86)

**Verify in Linker settings:**

* Additional Library Directories: Contains Lib path
* Additional Dependencies: Contains `qPapel-ProtectedAuth.lib`

</details>

<details>

<summary>Error: LNK1104 cannot open file</summary>

**Cause:** Library file not found

**Solutions:**

1. Check library file exists at specified path
2. Verify filename is exactly `qPapel-ProtectedAuth.lib`
3. Check for extra spaces in path
4. Use absolute path

</details>

<details>

<summary>Error: C++ standard not supported</summary>

**Cause:** C++ standard is below C++17

**Solutions:**

1. Open project properties
2. Go to C/C++ → Language
3. Set C++ Language Standard to `/std:c++17` or later
4. Rebuild project

</details>

<details>

<summary>Platform mismatch (x86 vs x64)</summary>

**Cause:** Project platform doesn't match library

**Solutions:**

1. Library is compiled for **x64**
2. Set your project platform to **x64**:
   * Build → Configuration Manager
   * Active solution platform → x64
3. Rebuild project

</details>

## Configuration for Release Build

{% hint style="info" %}
The same setup works for both Debug and Release configurations.
{% endhint %}

To configure for Release:

1. Switch to **Release** configuration
2. Repeat Steps 2-4 above
3. Or select **All Configurations** in Properties to apply to both

## Next Steps

{% hint style="success" %}
Installation complete! Now configure your API keys.
{% endhint %}

* Configuration - Get your API keys from dashboard
* Quick Start - Start using the library
* Complete Example - Full application example
###
