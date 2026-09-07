{
  "targets": [
    {
      "target_name": "secure_memory",
      "sources": ["secure_memory.cpp"],
      "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "msvs_settings": {
        "VCCLCompilerTool": {
          "ExceptionHandling": 1,
          "AdditionalOptions": ["/std:c++17"]
        },
        "VCLinkerTool": {
          "AdditionalOptions": ["/FORCE:MULTIPLE"]
        }
      }
    }
  ]
}
