# electron-updater-tools
Tools for building native addons compatible with electron-updater

# Usage
```
$ npm install electron-updater-tools -g
$ npm install electron-prebuilt -g
$ npm install bindings --save
$ elb configure
```

To build binaries compatible with node/iojs and electron on windows add the following to your `binding.gyp` file:
```
'include_dirs': [
  "<!(node -e \"require('electron-updater-tools')\")"        
],
```
And
```
'conditions' : [
  ['OS == "win"', {
    'libraries': [
      '-lShlwapi.lib'
    ],
    'msvs_settings': {
      'VCLinkerTool': {
          'DelayLoadDLLs': [ 'node.dll', 'iojs.exe', 'node.exe' ],
          # Don't print a linker warning when no imports from either .exe
          # are used.
          'AdditionalOptions': [ '/ignore:4199' ],
      }
    }
  }]
]
```
In your source somewhere add this line:
```
#include <delay_load_hook.h>
```

Now you can load your binaries into node/iojs or electron:
```
$ elb configure
$ iojs -p "require('bindings')('your-module-name')"
```
