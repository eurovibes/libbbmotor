# libbbmotor - A Beaglebone Motor Bridge Cape support library

The Motor Bridge Cape features bi-directional motor control using two
TB6612FNG integrated dual H-bridge, so it can control two stepper motors or
four brushed DC motors with 6~15V DC power and about 1A current draw per
motor. The cape provides 5V regulated power to SeeedStudio BeagleBone Green
or BeagleBone Black with a max input voltage of 15V. It also has six servo
control interface and six expand IO. All the features are provided by the on
board STM32F0 coprocessor. The coprocessor can communicate with BeagleBone
by I2C or UART interface.

libbbmotor provides a C API for a convenient access and handling.

## libbbmotor licensing rules

libbbmotor is provided under the terms of the GNU Lesser General Public
License version 2.1 only (LGPL-2.1) for the library parts and GNU General
Public License version 2 only (GPL-2.0) for the example applications.
The licenses are provided in LICENSES/LGPL-2.1 and LICENSES/GPL-2.0.

This documentation file provides a description of how each source file should
be annotated to make its license clear and unambiguous. It doesn’t replace the
libbbmotor license.

The common way of expressing the license of a source file is to add the
matching boilerplate text into the top comment of the file. Due to formatting,
typos etc. these “boilerplates” are hard to validate for tools which are used
in the context of license compliance.

An alternative to boilerplate text is the use of Software Package Data
Exchange (SPDX) license identifiers in each source file. SPDX license
identifiers are machine parsable and precise shorthands for the license under
which the content of the file is contributed. SPDX license identifiers are
managed by the SPDX Workgroup at the Linux Foundation and have been agreed on
by partners throughout the industry, tool vendors, and legal teams. For
further information see https://spdx.org/

libbbmotor requires the precise SPDX identifier in all source files. The valid
identifiers used in libbbmotor are explained in the section License
identifiers and have been retrieved from the official SPDX license list at
https://spdx.org/licenses/ along with the license texts.

### License identifier syntax

1. Placement:
The SPDX license identifier in kernel files shall be added at the first
possible line in a file which can contain a comment. For the majority of files
this is the first line, except for scripts which require the
‘#!PATH_TO_INTERPRETER’ in the first line. For those scripts the SPDX
identifier goes into the second line.

2. Style:
The SPDX license identifier is added in form of a comment. The comment style
depends on the file type:

```
C source: // SPDX-License-Identifier: <SPDX License Expression>
C header: /* SPDX-License-Identifier: <SPDX License Expression> */
scripts:  # SPDX-License-Identifier: <SPDX License Expression>
```

If a specific tool cannot handle the standard comment style, then the
appropriate comment mechanism which the tool accepts shall be used. This is
the reason for having the “/* */” style comment in C header files. There was
build breakage observed with generated .lds files where ‘ld’ failed to parse
the C++ comment. This has been fixed by now, but there are still older
assembler tools which cannot handle C++ style comments.

3. Syntax:
A <SPDX License Expression> is an SPDX short form license identifier found on
the SPDX License List. License identifiers for licenses like [L]GPL with the
‘or later’ option are constructed by using a “+” for indicating the ‘or later’
option.:

```
// SPDX-License-Identifier: GPL-2.0+
// SPDX-License-Identifier: LGPL-2.1+
```

### License identifiers
The licenses currently used, as well as the licenses for code added to
libbbmotor, can be broken down into GPL-2.0 and LGPL-2.1.

These licenses are available from the directory:

```
LICENSES/
```

in the libbbmotor source tree.

The files in this directory contain the full license text and Metatags. The
file names are identical to the SPDX license identifier which shall be used
for the license in source files.
Examples:

```
LICENSES/GPL-2.0
```

Contains the GPL version 2 license text and the required metatags:

```
LICENSES/LGPL-2.1
```

Contains the LGPL version 2.1 license text and the required metatags:

Metatags:
The following meta tags must be available in a license file:

* Valid-License-Identifier:
One or more lines which declare which License Identifiers are valid inside the
project to reference this particular license text. Usually this is a single
valid identifier.

* SPDX-URL:
The URL of the SPDX page which contains additional information related to the
license.

* Usage-Guidance:
Freeform text for usage advice. The text must include correct examples for the
SPDX license identifiers as they should be put into source files according to
the License identifier syntax guidelines.

* License-Text:
All text after this tag is treated as the original license text
File format examples:

```
Valid-License-Identifier: GPL-2.0
Valid-License-Identifier: GPL-2.0+
SPDX-URL: https://spdx.org/licenses/GPL-2.0.html
Usage-Guide:
  To use this license in source code, put one of the following SPDX
  tag/value pairs into a comment according to the placement
  guidelines in the licensing rules documentation.
  For 'GNU General Public License (GPL) version 2 only' use:
    SPDX-License-Identifier: GPL-2.0
  For 'GNU General Public License (GPL) version 2 or any later version' use:
    SPDX-License-Identifier: GPL-2.0+
License-Text:
  Full license text
```


```
Valid-License-Identifier: LGPL-2.1
Valid-License-Identifier: LGPL-2.1+
SPDX-URL: https://spdx.org/licenses/LGPL-2.1.html
Usage-Guide:
  To use this license in source code, put one of the following SPDX
  tag/value pairs into a comment according to the placement
  guidelines in the licensing rules documentation.
  For 'GNU Lesser General Public License (LGPL) version 2.1 only' use:
    SPDX-License-Identifier: LGPL-2.1
  For 'GNU Lesser General Public License (LGPL) version 2.1 or any later
  version' use:
    SPDX-License-Identifier: LGPL-2.1+
License-Text:
  Full license text
```
